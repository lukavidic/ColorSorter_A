#ifndef I2C_MASTER_H
#define	I2C_MASTER_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "i2c_types.h"

// These functions are the I2C API

i2c_error_t i2c_open(i2c_address_t address);
void        i2c_setAddress(i2c_address_t address);
i2c_error_t i2c_close(void);
i2c_error_t i2c_masterOperation(bool read);
i2c_error_t i2c_masterWrite(void); // to be depreciated
i2c_error_t i2c_masterRead(void);  // to be depreciated

void i2c_setTimeOut(uint8_t to);
void i2c_setBuffer(void *buffer, size_t bufferSize);

// Event Callback functions.
void i2c_setDataCompleteCallback(i2c_callback cb, void *p);
void i2c_setWriteCollisionCallback(i2c_callback cb, void *p);
void i2c_setAddressNACKCallback(i2c_callback cb, void *p);
void i2c_setDataNACKCallback(i2c_callback cb, void *p);
void i2c_setTimeOutCallback(i2c_callback cb, void *p);

// Interrupt Stuff : only needed when building the Interrupt driven driver
void i2c_ISR(void);
void i2c_busCollisionISR(void);

#endif	/* I2C_MASTER_H */

