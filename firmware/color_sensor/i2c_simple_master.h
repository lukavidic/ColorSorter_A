/**
 * @file i2c_simple_master.h
 * @brief I2C simple master header file
 */

#ifndef I2C_SIMPLE_MASTER_H
#define	I2C_SIMPLE_MASTER_H

#include <stdint.h>
#include <stdio.h>
#include "i2c_types.h"
/**
 * @brief Reads a single byte from a specific register of a slave I2C device.
 *
 * Sends the register address in a write transaction and then performs a repeated start to read one byte.
 * Retries the operation up to 2 times if an error occurs.
 *
 * @param address The I2C address of the slave device.
 * @param reg     The register within the slave device to read from.
 * @return The byte value read from the specified register.
 */
uint8_t i2c_read1ByteRegister(i2c_address_t address, uint8_t reg);
/**
 * @brief Reads a 2-byte value from a specific register of a slave I2C device.
 *
 * Performs a write of the register address followed by a repeated start and read of 2 bytes.
 * The result is interpreted as little endian and returned in swapped byte order.
 *
 * @param address The I2C address of the slave device.
 * @param reg     The register address to read from.
 * @return uint16_t The 2-byte value read from the register.
 */
uint16_t i2c_read2ByteRegister(i2c_address_t address, uint8_t reg);
/**
 * @brief Writes a single byte to a specific register of a slave I2C device.
 *
 * Combines the register address and the value into a two-step write process using a data complete callback.
 *
 * @param address The I2C address of the slave device.
 * @param reg     The register within the slave device to write to.
 * @param data    The byte of data to write to the register.
 */
void i2c_write1ByteRegister(i2c_address_t address, uint8_t reg, uint8_t data);
/**
 * @brief Writes a 2-byte value to a specific register of a slave I2C device.
 *
 * Sends the register address followed by 2 bytes of data using a callback-based write sequence.
 *
 * @param address The I2C address of the slave device.
 * @param reg     The register address to write to.
 * @param data    The 2-byte data to be written to the register.
 */
void i2c_write2ByteRegister(i2c_address_t address, uint8_t reg, uint16_t data);

/**
 * @brief Writes multiple bytes to a slave I2C device in one transaction.
 *
 * This function sends the given data array directly using the I2C master write operation.
 *
 * @param address The I2C address of the slave device.
 * @param data    Pointer to the data buffer to write.
 * @param len     The number of bytes to write.
 */
void i2c_writeNBytes(i2c_address_t address, void* data, size_t len);
/**
 * @brief Reads a block of data starting from a specific register of a slave I2C device.
 *
 * Performs a write of the register address followed by a repeated start and read of `len` bytes.
 * The data is stored in the provided buffer. This function is non-blocking internally but blocks until completion.
 *
 * @ingroup doc_driver_i2c_code
 *
 * @param address The I2C address of the slave device.
 * @param reg     The starting register address to read from.
 * @param data    Pointer to a buffer where the read data will be stored.
 * @param len     Number of bytes to read.
 */
void i2c_readDataBlock(i2c_address_t address, uint8_t reg, void *data, size_t len);
/**
 * @brief Reads a specified number of bytes directly from a slave I2C device.
 *
 * This function performs a direct read of `len` bytes from the device without writing a register address.
 * It is assumed that the device is already prepared to send the desired data.
 *
 * @ingroup doc_driver_i2c_code
 *
 * @param address The I2C address of the slave device.
 * @param data    Pointer to a buffer where the read data will be stored.
 * @param len     Number of bytes to read.
 */
void i2c_readNBytes(i2c_address_t address, void *data, size_t len);

#endif	/* I2C_SIMPLE_MASTER_H */

