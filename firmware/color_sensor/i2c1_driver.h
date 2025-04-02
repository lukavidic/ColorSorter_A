#ifndef __I2C1_DRIVER_H
#define __I2C1_DRIVER_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
 
typedef void (*interruptHandler)(void);

/* arbitration interface */
void i2c1_driver_close(void);

/* Interrupt interfaces */
void i2c1_enableIRQ(void);
bool i2c1_IRQisEnabled(void);
void i2c1_disableIRQ(void);
void i2c1_clearIRQ(void);
void i2c1_setIRQ(void);
void i2c1_waitForEvent(uint16_t*);

/* I2C interfaces */
bool  i2c1_driver_driver_open(void);
char i2c1_driver_getRXData(void);
void i2c1_driver_TXData(uint8_t);
void i2c1_driver_resetBus(void);
void i2c1_driver_start(void);
void i2c1_driver_restart(void);
void i2c1_driver_stop(void);
bool i2c1_driver_isNACK(void);
void i2c1_driver_startRX(void);
void i2c1_driver_sendACK(void);
void i2c1_driver_sendNACK(void);
void i2c1_driver_clearBusCollision(void);

bool  i2c1_driver_initSlaveHardware(void);
void i2c1_driver_releaseClock(void);
bool i2c1_driver_isBuferFull(void);
bool i2c1_driver_isStart(void);
bool i2c1_driver_isStop(void);
bool i2c1_driver_isAddress(void);
bool i2c1_driver_isData(void);
bool i2c1_driver_isRead(void);
void i2c1_driver_enableStartIRQ(void);
void i2c1_driver_disableStartIRQ(void);
void i2c1_driver_enableStopIRQ(void);
void i2c1_driver_disableStopIRQ(void);

void i2c1_driver_setBusCollisionISR(interruptHandler handler);
void i2c1_driver_setMasterI2cISR(interruptHandler handler);
void i2c1_driver_setSlaveI2cISR(interruptHandler handler);

#endif // __I2C1_DRIVER_H
