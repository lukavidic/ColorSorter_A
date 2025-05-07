/**
 * @file i2c_types.c
 * @brief I2C types source file
 *
 * This file contains definitions of functions decleared in
 * i2c_types.h
 */

#include "i2c_types.h"
#include "../inc/inc.h"
#include <xc.h>
i2c_operations_t i2c_returnStop(void *p)
{
    return i2c_stop;
}

i2c_operations_t i2c_returnReset(void *p)
{
    return i2c_reset_link;
}

i2c_operations_t i2c_restartWrite(void *p)
{
    return i2c_restart_write;
}

i2c_operations_t i2c_restartRead(void *p)
{
    return i2c_restart_read;
}

