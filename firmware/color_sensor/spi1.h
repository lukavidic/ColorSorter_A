/**
 * @file spi1.h
 * @brief SPI comunnication protocol header file
 */

#ifndef _SPI1_H
#define _SPI1_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif       
/**
  SPI1_DUMMY_DATA 

  @Summary
    Dummy data to be sent. 

  @Description
    Dummy data to be sent, when no input buffer is specified in the buffer APIs.
 */
#define SPI1_DUMMY_DATA 0x0
        
/**
  SPI1_FIFO_FILL_LIMIT

  @Summary
    FIFO fill limit for data transmission. 

  @Description
    The amount of data to be filled in the FIFO during transmission. The maximum limit allowed is 8.
 */
#define SPI1_FIFO_FILL_LIMIT 0x8

//Check to make sure that the FIFO limit does not exceed the maximum allowed limit of 8
#if (SPI1_FIFO_FILL_LIMIT > 8)

    #define SPI1_FIFO_FILL_LIMIT 8

#endif

/**
  SPI1 Status Enumeration

  @Summary
    Defines the status enumeration for SPI1.

  @Description
    This defines the status enumeration for SPI1.
 */
typedef enum {
    SPI1_SHIFT_REGISTER_EMPTY  = 1 << 7,
    SPI1_RECEIVE_OVERFLOW = 1 << 6,
    SPI1_RECEIVE_FIFO_EMPTY = 1 << 5,
    SPI1_TRANSMIT_BUFFER_FULL = 1 << 1,
    SPI1_RECEIVE_BUFFER_FULL = 1 << 0
}SPI1_STATUS;

/**
  @Summary
    Initializes the SPI instance : 1

  @Description
    This routine initializes the spi1 driver instance for : 1
    index, making it ready for clients to open and use it.

    This routine must be called before any other SPI1 routine is called.
    This routine should only be called once during system initialization.
 
  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t   myWriteBuffer[MY_BUFFER_SIZE];
    uint16_t   myReadBuffer[MY_BUFFER_SIZE];
    uint16_t writeData;
    uint16_t readData;
    SPI1_STATUS status;
    unsigned int    total;
    SPI1_Initialize;
 
    total = 0;
    numberOfBytesFactor = 2;
    do
    {
        total  = SPI1_Exchange16bitBuffer( &myWriteBuffer[total], (MY_BUFFER_SIZE - total)*numberOfBytesFactor, &myReadBuffer[total]);

        // Do something else...

    } while( total < MY_BUFFER_SIZE );

    readData = SPI1_Exchange16bit( writeData);

    status = SPI1_StatusGet();

    </code>

*/

void SPI1_Initialize (void);




/**
  @Summary
    Exchanges one byte of data from SPI1

  @Description
    This routine exchanges one byte of data from the SPI1.
    This is a blocking routine.

  @Preconditions
    The SPI1_Initialize routine must have been called for the specified
    SPI1 driver instance.

  @Returns
    Data read from SPI1

  @Param
    data         - Data to be written onto SPI1.

  @Example 
    Refer to SPI1_Initialize() for an example    
*/
        
uint8_t SPI1_Exchange8bit( uint8_t data );

/**
  @Summary
    Exchanges data from a buffer of size one byte from SPI1

  @Description
    This routine exchanges data from a buffer of size one byte from the SPI1.
    This is a blocking routine.

  @Preconditions
    The SPI1_Initialize routine must have been called for the specified
    SPI1 driver instance.

  @Returns
    Number of 8bit data written/read.

  @Param
    dataTransmitted         - Buffer of data to be written onto SPI1.
 
  @Param
    byteCount         - Number of bytes to be exchanged.
 
  @Param
    dataReceived         - Buffer of data to be read from SPI1.

  @Example 
    Refer to SPI1_Initialize() for an example    
 
*/

uint16_t SPI1_Exchange8bitBuffer(uint8_t *dataTransmitted, uint16_t byteCount, uint8_t *dataReceived);

/**
  @Summary
    Returns the value of the status register of SPI instance : 1

  @Description
    This routine returns the value of the status register of SPI1 driver instance : 1

  @Preconditions
    None.

  @Returns
    Returns the value of the status register.

  @Param
    None.

  @Example 
    Refer to SPI1_Initialize() for an example    
 
*/

SPI1_STATUS SPI1_StatusGet(void);


#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif //_SPI1_H

