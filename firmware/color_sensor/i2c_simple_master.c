/**
 * @file i2c_simple_master.c
 * @brief I2C simple master source file
 *
 * This file contains definitions of functions decleared in
 * i2c_simple_master.h
 */

#include "i2c_master.h"
#include "i2c_simple_master.h"
#include "../inc/inc.h"
#include <xc.h>
/****************************************************************/
/**
 * @brief Callback invoked when the first byte (register address) has been transmitted in a write transaction.
 *
 * This function sets up the next byte (actual data to be written to the register) in the buffer,
 * and disables further data complete callbacks by passing NULLs.
 * 
 * @param p Pointer to the data byte to be written.
 * @return i2c_operations_t Returns `i2c_continue` to keep the I2C FSM running.
 */
static i2c_operations_t wr1RegCompleteHandler(void *p)
{
    i2c_setBuffer(p,1);
    i2c_setDataCompleteCallback(NULL,NULL);
    return i2c_continue;
}
void i2c_write1ByteRegister(i2c_address_t address, uint8_t reg, uint8_t data)
{
    while(!i2c_open(address)); // sit here until we get the bus..
    i2c_setDataCompleteCallback(wr1RegCompleteHandler,&data);
    i2c_setBuffer(&reg,1);
    i2c_setAddressNACKCallback(i2c_restartWrite,NULL); //NACK polling?
    i2c_masterWrite();
    while(I2C_BUSY == i2c_close()); // sit here until finished.
}

void i2c_writeNBytes(i2c_address_t address, void* data, size_t len)
{
    while(!i2c_open(address)); // sit here until we get the bus..
    i2c_setBuffer(data,len);
    i2c_setAddressNACKCallback(i2c_restartWrite,NULL); //NACK polling?
    i2c_masterWrite();
    while(I2C_BUSY == i2c_close()); // sit here until finished.
}

/****************************************************************/
/**
 * @brief Callback invoked after the register address has been sent during a read operation.
 *
 * Sets the buffer to receive a single byte and schedules a restart-read operation.
 *
 * @param p Pointer to the buffer where the read byte will be stored.
 * @return i2c_operations_t Returns `i2c_restart_read` to initiate a read transaction.
 */
static i2c_operations_t rd1RegCompleteHandler(void *p)
{
    i2c_setBuffer(p,1);
    i2c_setDataCompleteCallback(NULL,NULL);
    return i2c_restart_read;
}
uint8_t i2c_read1ByteRegister(i2c_address_t address, uint8_t reg)
{
    uint8_t    d2=42;
    i2c_error_t e;
    int x;

    for(x = 2; x != 0; x--)
    {
        while(!i2c_open(address)); // sit here until we get the bus..
        i2c_setDataCompleteCallback(rd1RegCompleteHandler,&d2);
        i2c_setBuffer(&reg,1);
        i2c_setAddressNACKCallback(i2c_restartWrite,NULL); //NACK polling?
        i2c_masterWrite();
        while(I2C_BUSY == (e = i2c_close())); // sit here until finished.
        if(e==I2C_NOERR) break;
    }
    

    return d2;
}

/****************************************************************/
/**
 * @brief Callback handler for completing a 2-byte I2C read operation.
 *
 * This function sets a 2-byte read buffer and disables further callbacks.
 * It is used after writing the register address to initiate the read phase.
 *
 * @param p Pointer to the memory location where the result should be stored.
 * @return i2c_operations_t Operation indicating that a restart read should occur.
 */
static i2c_operations_t rd2RegCompleteHandler(void *p)
{
    i2c_setBuffer(p,2);
    i2c_setDataCompleteCallback(NULL,NULL);
    return i2c_restart_read;
}

uint16_t i2c_read2ByteRegister(i2c_address_t address, uint8_t reg)
{
    // result is little endian
    uint16_t    result;

    while(!i2c_open(address)); // sit here until we get the bus..
    i2c_setDataCompleteCallback(rd2RegCompleteHandler,&result);
    i2c_setBuffer(&reg,1);
    i2c_setAddressNACKCallback(i2c_restartWrite,NULL); //NACK polling?
    i2c_masterWrite();
    while(I2C_BUSY == i2c_close()); // sit here until finished.
    
    return (result << 8 | result >> 8);
}

/****************************************************************/
/**
 * @brief Callback handler for completing a 2-byte I2C write operation.
 *
 * This function sets a 2-byte write buffer and disables further callbacks.
 *
 * @param p Pointer to the data to be written.
 * @return i2c_operations_t Operation indicating the write should continue.
 */
static i2c_operations_t wr2RegCompleteHandler(void *p)
{
    i2c_setBuffer(p,2);
    i2c_setDataCompleteCallback(NULL,NULL);
    return i2c_continue;
}

void i2c_write2ByteRegister(i2c_address_t address, uint8_t reg, uint16_t data)
{
    //LATAbits.LATA2 = 1;
    uint8_t buffer[3]; 
    /*buffer[0] = reg;
    buffer[1] = data & 0xFF;
    buffer[2] = (data >> 8);*/
    while(!i2c_open(address)); // sit here until we get the bus..
    /*__delay32(4000000);
    LATAbits.LATA2 = 0;*/
    i2c_setDataCompleteCallback(wr2RegCompleteHandler,&data); 
    //i2c_setBuffer(buffer,3); 
    i2c_setBuffer(&reg,1);
    i2c_setAddressNACKCallback(i2c_restartWrite,NULL); //NACK polling?
    i2c_masterWrite();
    while(I2C_BUSY == i2c_close()); // sit here until finished.
}

/****************************************************************/

/**
 * @brief Internal structure for buffered I2C block reads.
 */
typedef struct
{
    size_t len;
    char *data;
}buf_t;
/**
 * @brief Callback handler for completing a block I2C read operation.
 *
 * Sets up the read buffer and disables further callbacks.
 *
 * @param p Pointer to a buf_t structure containing the buffer and its length.
 * @return i2c_operations_t Operation indicating that a restart read should occur.
 */
static i2c_operations_t rdBlkRegCompleteHandler(void *p)
{
    i2c_setBuffer(((buf_t *)p)->data,((buf_t*)p)->len);
    i2c_setDataCompleteCallback(NULL,NULL);
    return i2c_restart_read;
}

void i2c_readDataBlock(i2c_address_t address, uint8_t reg, void *data, size_t len)
{
    // result is little endian
    buf_t    d;
    d.data = data;
    d.len = len;

    while(!i2c_open(address)); // sit here until we get the bus..
    i2c_setDataCompleteCallback(rdBlkRegCompleteHandler,&d);
    i2c_setBuffer(&reg,1);
    i2c_setAddressNACKCallback(i2c_restartWrite,NULL); //NACK polling?
    i2c_masterWrite();
    while(I2C_BUSY == i2c_close()); // sit here until finished.
}

void i2c_readNBytes(i2c_address_t address, void *data, size_t len)
{
    while(!i2c_open(address)); // sit here until we get the bus..
    i2c_setBuffer(data,len);
    i2c_masterRead();
    while(I2C_BUSY == i2c_close()); // sit here until finished.
}
