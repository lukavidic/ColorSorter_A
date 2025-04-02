#ifndef I2C_TYPES_H
#define	I2C_TYPES_H

#include <stdint.h>
#include <stdio.h>

typedef enum {
    I2C_NOERR, // The message was sent.
    I2C_BUSY,  // Message was NOT sent, bus was busy.
    I2C_FAIL   // Message was NOT sent, bus failure
               // If you are interested in the failure reason,
               // Sit on the event call-backs.
} i2c_error_t;

typedef enum
{
    i2c_stop=1,
    i2c_restart_read,
    i2c_restart_write,
    i2c_continue,
    i2c_reset_link
} i2c_operations_t;

typedef i2c_operations_t (*i2c_callback)(void *p);

typedef uint8_t i2c_address_t;

// common callback responses
i2c_operations_t i2c_returnStop(void *p);
i2c_operations_t i2c_returnReset(void *p);
i2c_operations_t i2c_restartWrite(void *p);
i2c_operations_t i2c_restartRead(void *p);

#endif	/* I2C_TYPES_H */

