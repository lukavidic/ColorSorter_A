#ifndef I2C_MASTER_H
#define	I2C_MASTER_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "i2c_types.h"

// These functions are the I2C API
/**
 * @brief Opens the I2C bus for communication with a specific device.
 * 
 * This function attempts to take control of the I2C bus and set the device address
 * for communication. It initializes internal state and callback handlers for common I2C events.
 * 
 * @note If the bus is already in use (`inUse` flag is set), the function will return `I2C_BUSY`.
 * 
 * @param address The 7-bit I2C address of the target device.
 * @return i2c_error_t `I2C_NOERR` if the bus was successfully opened, otherwise `I2C_BUSY`.
 */
i2c_error_t i2c_open(i2c_address_t address);
/**
 * @brief Sets the target device I2C address.
 * 
 * This function updates the internal `i2c_status` structure with a new I2C address.
 * Useful when changing the device target without reopening the bus.
 * 
 * @param address The new I2C address to be set.
 */
void        i2c_setAddress(i2c_address_t address);
/**
 * @brief Closes the I2C bus if it is not currently busy.
 * 
 * If the bus is not actively in use and the current address matches,
 * this function resets the `inUse` flag, disables interrupts, and marks the address as free.
 * 
 * @return i2c_error_t Returns the last error status (`i2c_status.error`) if successfully closed,
 *                     otherwise returns `I2C_BUSY` if the bus is still active.
 */
i2c_error_t i2c_close(void);
/**
 * @brief Initiates an I2C master operation (read or write).
 * 
 * This function begins either a master read or write transaction, depending on the
 * `read` parameter. It sets the appropriate internal state, starts the I2C transfer, and
 * chooses between interrupt-based or polled operation depending on IRQ availability.
 * 
 * @param read If `true`, initiates a read operation; if `false`, initiates a write.
 * @return i2c_error_t `I2C_NOERR` if operation started successfully, otherwise `I2C_BUSY`.
 */
i2c_error_t i2c_masterOperation(bool read);
/**
 * @brief Initiates an I2C master write operation.
 * 
 * This is a convenience wrapper around `i2c_masterOperation(false)`.
 * 
 * @return i2c_error_t `I2C_NOERR` if operation started successfully, otherwise `I2C_BUSY`.
 */
i2c_error_t i2c_masterWrite(void); // to be depreciated
/**
 * @brief Initiates an I2C master read operation.
 * 
 * This is a convenience wrapper around `i2c_masterOperation(true)`.
 * 
 * @return i2c_error_t `I2C_NOERR` if operation started successfully, otherwise `I2C_BUSY`.
 */
i2c_error_t i2c_masterRead(void);  // to be depreciated
/**
 * @brief Sets the I2C timeout value.
 * 
 * This function safely sets the timeout threshold for I2C operations. Interrupts
 * are disabled during the update to avoid race conditions.
 * 
 * @param to The new timeout value to set.
 */
void i2c_setTimeOut(uint8_t to);
/**
 * @brief Sets the data buffer for an I2C transaction.
 * 
 * This function configures the data buffer and its size for the upcoming I2C
 * operation, if the buffer is currently available. Once set, the buffer is marked as used.
 * 
 * @param buffer Pointer to the data buffer.
 * @param bufferSize Size of the data buffer in bytes.
 */
void i2c_setBuffer(void *buffer, size_t bufferSize);

// Event Callback functions.
/**
 * @brief Sets the callback function to be called when I2C data transfer is complete.
 * 
 * @param cb Pointer to the callback function that will be invoked upon data completion.
 * @param p  Pointer to user-defined data that will be passed to the callback function.
 */
void i2c_setDataCompleteCallback(i2c_callback cb, void *p);
/**
 * @brief Sets the callback function to be called when a write collision occurs on the I2C bus.
 * 
 * @param cb Pointer to the callback function that will be invoked on write collision.
 * @param p  Pointer to user-defined data that will be passed to the callback function.
 */
void i2c_setWriteCollisionCallback(i2c_callback cb, void *p);
/**
 * @brief Sets the callback function to be called when the I2C address is not acknowledged (NACK).
 * 
 * @param cb Pointer to the callback function that will be invoked when the address is NACKed.
 * @param p  Pointer to user-defined data that will be passed to the callback function.
 */
void i2c_setAddressNACKCallback(i2c_callback cb, void *p);
/**
 * @brief Sets the callback function to be called when data is not acknowledged (NACK) on the I2C bus.
 * 
 * @param cb Pointer to the callback function that will be invoked when data is NACKed.
 * @param p  Pointer to user-defined data that will be passed to the callback function.
 */
void i2c_setDataNACKCallback(i2c_callback cb, void *p);

/**
 * @brief Sets the callback function to be called when an I2C timeout occurs.
 * 
 * @param cb Pointer to the callback function that will be invoked on timeout.
 * @param p  Pointer to user-defined data that will be passed to the callback function.
 */
void i2c_setTimeOutCallback(i2c_callback cb, void *p);

// Interrupt Stuff : only needed when building the Interrupt driven driver
/**
 * @brief Main I2C interrupt service routine.
 * 
 * Clears the I2C interrupt flag, checks for address NACK conditions, and invokes the appropriate
 * state handler from the FSM table based on the current state. 
 * 
 * Note: Write collision flags are ignored; premature SSPBUF writes are not handled here.
 */
void i2c_ISR(void);
/**
 * @brief I2C bus collision interrupt service routine.
 * 
 * Clears the bus collision condition flag to resume normal I2C operation.
 */
void i2c_busCollisionISR(void);

#endif	/* I2C_MASTER_H */

