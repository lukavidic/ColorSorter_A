
#ifndef I2C_SIMPLE_MASTER_H
#define	I2C_SIMPLE_MASTER_H

#include <stdint.h>
#include <stdio.h>
#include "i2c_types.h"

uint8_t i2c_read1ByteRegister(i2c_address_t address, uint8_t reg);
uint16_t i2c_read2ByteRegister(i2c_address_t address, uint8_t reg);
void i2c_write1ByteRegister(i2c_address_t address, uint8_t reg, uint8_t data);
void i2c_write2ByteRegister(i2c_address_t address, uint8_t reg, uint16_t data);

void i2c_writeNBytes(i2c_address_t address, void* data, size_t len);
void i2c_readDataBlock(i2c_address_t address, uint8_t reg, void *data, size_t len);
void i2c_readNBytes(i2c_address_t address, void *data, size_t len);

#endif	/* I2C_SIMPLE_MASTER_H */

