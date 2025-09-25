/** @file app_main.h
 *
 * @brief Header file for app_main.c, containing application initialization and loop function prototypes.
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2025 DalMAST.  All rights reserved.
 */

#ifndef APP_MAIN_H
#define APP_MAIN_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "main.h"
#include "i2c_config.h"

void app_init(void);
void app_loop(void);

#endif /* APP_MAIN_H */