/** @file i2c_config.h
 *
 * @brief Configuration for I2C mock device.
 */

#ifndef I2C_MOCK_CONFIG_H
#define I2C_MOCK_CONFIG_H

#include <stdint.h>

enum
{
    END_OF_REG_WRAP, /* Wrap around to register 0 */
    END_OF_REG_HOLD  /* Hold at last register */
};

typedef struct
{
    uint8_t addr;  /* Register address seen by master */
    uint8_t value; /* Current contents */
} mock_reg_t;

/* 7-bit I2C slave address */
#define MOCK_I2C_ADDRESS 0x3A

/* Number of registers */
#define MOCK_REG_COUNT 4

/* End-of-register behavior */
#define END_OF_REG_BEHAVIOUR END_OF_REG_WRAP

/* Declare register table (definition comes later in same header with inline trick) */
extern mock_reg_t mock_registers[MOCK_REG_COUNT];

/* Provide definition guarded by a macro to prevent multiple copies */
#ifdef I2C_CONFIG_DEFINE
mock_reg_t mock_registers[MOCK_REG_COUNT] = {
    {0x00, 0x00},
    {0x01, 0x00},
    {0x02, 0x55},
    {0x03, 0x01},
};
#endif

#endif /* I2C_MOCK_CONFIG_H */
