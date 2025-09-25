/** @file app_main.c
 *
 * @brief Main application code for I2C mock device.
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2025 DalMAST. All rights reserved.
 */

#define I2C_CONFIG_DEFINE

#include "app_main.h"
#include "stm32f4xx_hal_i2c.h"

#define MAX_READ_LEN MOCK_REG_COUNT
#define MAX_WRITE_LEN MOCK_REG_COUNT

static int current_index = 0; /* index into mock_registers[] */
static uint8_t rx_buf[MAX_WRITE_LEN];
static uint8_t tx_buf[MAX_READ_LEN];
static uint16_t rx_expected_len = 0;
static uint16_t rx_received_len = 0;

extern I2C_HandleTypeDef hi2c1;

static int find_reg_index(uint8_t addr);

/**
 * @brief Initialize the application.
 */
void app_init(void)
{
    hi2c1.Init.OwnAddress1 = MOCK_I2C_ADDRESS << 1; /* STM32 expects left-shifted */
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        Error_Handler();
    }

    HAL_I2C_EnableListen_IT(&hi2c1); /* Enable address match listening */
}

/**
 * @brief Main application loop.
 */
void app_loop(void)
{
    /* Nothing required here — I2C slave is entirely interrupt/DMA driven */
}

/**
 * @brief I2C Address Match callback.
 *        Sets up DMA for receive or transmit based on transfer direction.
 *
 * @param hi2c Pointer to I2C handle.
 * @param TransferDirection Direction of transfer: I2C_DIRECTION_TRANSMIT or I2C_DIRECTION_RECEIVE.
 * @param AddrMatchCode Matched address (not used here).
 */
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c,
                          uint8_t TransferDirection,
                          uint16_t AddrMatchCode)
{
    if (hi2c->Instance == I2C1)
    {
        if (TransferDirection == I2C_DIRECTION_TRANSMIT)
        {
            /* Master writing */
            rx_expected_len = sizeof(rx_buf);
            rx_received_len = 0;
            HAL_I2C_Slave_Receive_DMA(hi2c, rx_buf, rx_expected_len);
        }
        else
        {
            /* Master reading */
            if (current_index >= 0 && current_index < MOCK_REG_COUNT)
            {
                uint8_t remaining = MOCK_REG_COUNT - current_index;
                uint8_t len = (remaining < MAX_READ_LEN) ? remaining : MAX_READ_LEN;

                for (uint8_t i = 0; i < len; i++)
                {
                    tx_buf[i] = mock_registers[current_index + i].value;
                }

                HAL_I2C_Slave_Transmit_DMA(hi2c, tx_buf, len);
            }
        }
    }
}

/**
 * @brief I2C Slave Rx Complete callback.
 *        Updates mock_registers with received data.
 *
 * @param hi2c Pointer to I2C handle.
 */
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1)
    {
        uint16_t dma_remaining = __HAL_DMA_GET_COUNTER(hi2c->hdmarx);
        rx_received_len = rx_expected_len - dma_remaining;

        if (rx_received_len >= 1)
        {
            int reg_index = find_reg_index(rx_buf[0]); /* first byte = register address */
            if (reg_index >= 0)
            {
                for (uint16_t i = 1; i < rx_received_len; i++)
                {
                    if (reg_index < MOCK_REG_COUNT)
                    {
                        mock_registers[reg_index++].value = rx_buf[i];
                    }
                }
                current_index = reg_index; /* next read starts after last written */
            }
        }
    }
}

/**
 * @brief I2C Slave Tx Complete callback.
 *        Advances current_index to next register after transmission.
 *
 * @param hi2c Pointer to I2C handle.
 */
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1)
    {
        if (current_index < MOCK_REG_COUNT - 1)
        {
            current_index++;
        }
        else
        {
#if END_OF_REG_BEHAVIOUR == END_OF_REG_WRAP
            current_index = 0;
#elif END_OF_REG_BEHAVIOUR == END_OF_REG_HOLD
            /* stay at last register */
#endif
        }
    }
}

/**
 * @brief I2C Listen Complete callback.
 *        Re-enables listening for address match.
 *
 * @param hi2c Pointer to I2C handle.
 */
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1)
    {
        HAL_I2C_EnableListen_IT(hi2c); /* re-arm listening */
    }
}

/**
 * @brief Find index of register by address.
 *
 * @param addr Register address to find.
 * @return Index of register in mock_registers array, or -1 if not found.
 */
static int find_reg_index(uint8_t addr)
{
    for (int i = 0; i < MOCK_REG_COUNT; i++)
    {
        if (mock_registers[i].addr == addr)
        {
            return i;
        }
    }
    return -1; /* Not found */
}
