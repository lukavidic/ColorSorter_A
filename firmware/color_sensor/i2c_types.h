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
/**
 * @brief Callback function that returns the I2C stop operation.
 * 
 * This function can be used as a default or user-defined callback that instructs
 * the I2C state machine to issue a stop condition on the bus.
 * 
 * @param p Pointer to user-defined data (unused in this implementation).
 * @return i2c_operations_t Operation code indicating a stop condition.
 */
i2c_operations_t i2c_returnStop(void *p);
/**
 * @brief Callback function that returns the I2C reset operation.
 * 
 * This function signals the I2C state machine to reset the I2C link,
 * often used after an error or timeout condition.
 * 
 * @param p Pointer to user-defined data (unused in this implementation).
 * @return i2c_operations_t Operation code indicating a link reset.
 */
i2c_operations_t i2c_returnReset(void *p);
/**
 * @brief Callback function that returns the I2C restart write operation.
 * 
 * This function is used to indicate that a repeated start condition should be
 * issued, followed by a write operation.
 * 
 * @param p Pointer to user-defined data (unused in this implementation).
 * @return i2c_operations_t Operation code indicating a repeated start with write.
 */
i2c_operations_t i2c_restartWrite(void *p);

/**
 * @brief Callback function that returns the I2C restart read operation.
 * 
 * This function is used to indicate that a repeated start condition should be
 * issued, followed by a read operation.
 * 
 * @param p Pointer to user-defined data (unused in this implementation).
 * @return i2c_operations_t Operation code indicating a repeated start with read.
 */
i2c_operations_t i2c_restartRead(void *p);

#endif	/* I2C_TYPES_H */

