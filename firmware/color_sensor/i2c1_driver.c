#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "i2c1_driver.h"
#include "../inc/inc.h"
#include <xc.h>

#include <libpic30.h>

void (*i2c1_driver_busCollisionISR)(void);
void (*i2c1_driver_Masteri2cISR)(void);
void (*i2c1_driver_Slavei2cISR)(void);

void i2c1_driver_close(void)
{
    I2C1CONLbits.I2CEN = 0;
}

/* Interrupt Control */
void i2c1_enableIRQ(void)
{
    IEC1bits.MI2C1IE = 1;
    IEC1bits.SI2C1IE = 1;
}

bool i2c1_IRQisEnabled(void)
{
    return IEC1bits.MI2C1IE || IEC1bits.SI2C1IE;
}

void i2c1_disableIRQ(void)
{
    IEC1bits.MI2C1IE = 0;
    IEC1bits.SI2C1IE = 0;
}

void i2c1_clearIRQ(void)
{
    IFS1bits.MI2C1IF = 0;
    IFS1bits.SI2C1IF = 0;
}

void i2c1_setIRQ(void)
{
    IFS1bits.MI2C1IF = 1;
    IFS1bits.SI2C1IF = 1;
}

void i2c1_waitForEvent(uint16_t *timeout)
{
    //uint16_t to = (timeout!=NULL)?*timeout:100;
    //to <<= 8;
    if((IFS1bits.MI2C1IF == 0) && (IFS1bits.SI2C1IF == 0))
    {
        while(1)// to--)
        {
            if(IFS1bits.MI2C1IF || IFS1bits.SI2C1IF) break;
            __delay_us(100);
        }
    }
}

bool i2c1_driver_driver_open(void)
{
    if(!I2C1CONLbits.I2CEN)
    {
        // initialize the hardware
        // STAT Setting 
        I2C1STAT = 0x0;
        
        // CON Setting
        I2C1CONL = 0x8000;
        
        // Baud Rate Generator Value: I2CBRG 100000;   
        I2C1BRG = 78;
        
        return true;
    }
    else
        return false;
}
bool i2c1_driver_initSlaveHardware(void)
{
    if(!I2C1CONLbits.I2CEN)
    {
        I2C1CONHbits.AHEN = 1;
        I2C1CONHbits.DHEN = 1;
        I2C1CONLbits.STREN = 1;
        
        I2C1CONLbits.I2CEN = 1;
        return true;
    }
    return false;
}

void i2c1_driver_resetBus(void)
{
    
}

void i2c1_driver_start(void)
{
    I2C1CONLbits.SEN = 1;
}

void i2c1_driver_restart(void)
{
    I2C1CONLbits.RSEN = 1;
}

void i2c1_driver_stop(void)
{
    I2C1CONLbits.PEN = 1;
}

bool i2c1_driver_isNACK(void)
{
    return I2C1STATbits.ACKSTAT;
}

void i2c1_driver_startRX(void)
{
    I2C1CONLbits.RCEN = 1;
}

char i2c1_driver_getRXData(void)
{
    return I2C1RCV;
}

void i2c1_driver_TXData(uint8_t d)
{
    I2C1TRN = d;
}

void i2c1_driver_sendACK(void)
{
    I2C1CONLbits.ACKDT = 0;
    I2C1CONLbits.ACKEN = 1; // start the ACK/NACK
}

void i2c1_driver_sendNACK(void)
{
    I2C1CONLbits.ACKDT = 1;
    I2C1CONLbits.ACKEN = 1; // start the ACK/NACK
}

void i2c1_driver_releaseClock(void)
{
    I2C1CONLbits.SCLREL = 1;
}

bool i2c1_driver_isBuferFull(void)
{
    return I2C1STATbits.RBF || I2C1STATbits.TBF;
}

bool i2c1_driver_isStart(void)
{
    return I2C1STATbits.S;
}

bool i2c1_driver_isAddress(void)
{
    return !I2C1STATbits.D_A;
}

bool i2c1_driver_isStop(void)
{
    return I2C1STATbits.P;
}

bool i2c1_driver_isData(void)
{
    return I2C1STATbits.D_A;
}

bool i2c1_driver_isRead(void)
{
    return I2C1STATbits.R_W;
}

void i2c1_driver_clearBusCollision(void)
{
    I2C1STATbits.BCL = 0; // clear the bus collision.
}

void i2c1_driver_enableStartIRQ(void)
{
    I2C1CONHbits.SCIE = 1;
}

void i2c1_driver_disableStartIRQ(void)
{
    I2C1CONHbits.SCIE = 0;
}

void i2c1_driver_enableStopIRQ(void)
{
    I2C1CONHbits.PCIE = 1;
}

void i2c1_driver_disableStopIRQ(void)
{
    I2C1CONHbits.PCIE = 0;
}

void i2c1_driver_setBusCollisionISR(interruptHandler handler){
    i2c1_driver_busCollisionISR = handler;
}

void i2c1_driver_setMasterI2cISR(interruptHandler handler){
    i2c1_driver_Masteri2cISR = handler;
}

void i2c1_driver_setSlaveI2cISR(interruptHandler handler){
    i2c1_driver_Slavei2cISR = handler;
}



/**
 * @brief Interrupt service routine for I2C1 master interrupt.
 *
 * This interrupt handler is called when an I2C1 master mode interrupt occurs. It invokes
 * the user-defined ISR set by the `i2c1_driver_setMasterI2cISR` function.
 */
void __attribute__ ( ( interrupt, no_auto_psv ) ) _MI2C1Interrupt ( void )
{
    (*i2c1_driver_Masteri2cISR)();
}

/**
 * @brief Interrupt service routine for I2C1 slave interrupt.
 *
 * This interrupt handler is called when an I2C1 slave mode interrupt occurs. It invokes
 * the user-defined ISR set by the `i2c1_driver_setSlaveI2cISR` function.
 */
void __attribute__ ( ( interrupt, no_auto_psv ) ) _SI2C1Interrupt ( void )
{
    (*i2c1_driver_Slavei2cISR)();
}
