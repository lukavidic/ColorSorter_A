/**
 * @file spi1.c
 * @brief SPI protocol source file
 *
 * This file contains definitions of functions decleared in
 * spi1.h
 */

#include "spi1.h"
#include "xc.h"
/**
  SPI1 Transfer Mode Enumeration

  @Summary
    Defines the Transfer Mode enumeration for SPI1.

  @Description
    This defines the Transfer Mode enumeration for SPI1.
 */
typedef enum {
    SPI1_TRANSFER_MODE_32BIT  = 2,
    SPI1_TRANSFER_MODE_16BIT = 1,
    SPI1_TRANSFER_MODE_8BIT = 0
}SPI1_TRANSFER_MODE;

inline __attribute__((__always_inline__)) SPI1_TRANSFER_MODE SPI1_TransferModeGet(void);
void SPI1_Exchange( uint8_t *pTransmitData, uint8_t *pReceiveData );
uint16_t SPI1_ExchangeBuffer(uint8_t *pTransmitData, uint16_t byteCount, uint8_t *pReceiveData);

void SPI1_Initialize (void)
{
    
    // AUDEN disabled; FRMEN disabled; AUDMOD I2S; FRMSYPW One clock wide; AUDMONO stereo; FRMCNT 0; MSSEN disabled; FRMPOL disabled; IGNROV disabled; SPISGNEXT not sign-extended; FRMSYNC disabled; URDTEN disabled; IGNTUR disabled; 
    SPI1CON1H = 0x00;
    // WLENGTH 0; 
    SPI1CON2L = 0x00;
    // SPIROV disabled; FRMERR disabled; 
    SPI1STATL = 0x00;
    // SPI1BRGL 0; 
    SPI1BRGL = 0x00;
    // SPITBFEN disabled; SPITUREN disabled; FRMERREN disabled; SRMTEN disabled; SPIRBEN disabled; BUSYEN disabled; SPITBEN disabled; SPIROVEN disabled; SPIRBFEN disabled; 
    SPI1IMSKL = 0x00;
    // RXMSK 0; TXWIEN disabled; TXMSK 0; RXWIEN disabled; 
    SPI1IMSKH = 0x00;
    // SPI1URDTL 0; 
    SPI1URDTL = 0x00;
    // SPI1URDTH 0; 
    SPI1URDTH = 0x00;
    // SPIEN enabled; DISSDO disabled; MCLKEN FOSC/2; CKP Idle:Low, Active:High; SSEN disabled; MSTEN Master; MODE16 disabled; SMP Middle; DISSCK disabled; SPIFE Frame Sync pulse precedes; CKE Active to Idle; MODE32 disabled; SPISIDL disabled; ENHBUF enabled; DISSDI disabled; 
    SPI1CON1L = 0x8121;

}

void SPI1_Exchange( uint8_t *pTransmitData, uint8_t *pReceiveData )
{

    while( SPI1STATLbits.SPITBF == true )
    {

    }
        
    SPI1BUFL = *((uint8_t*)pTransmitData);

    while ( SPI1STATLbits.SPIRBE == true)
    {
    
    }

    *((uint8_t*)pReceiveData) = SPI1BUFL;
}

uint16_t SPI1_ExchangeBuffer(uint8_t *pTransmitData, uint16_t byteCount, uint8_t *pReceiveData)
{

    uint16_t dataSentCount = 0;
    uint16_t dataReceivedCount = 0;
    uint16_t dummyDataReceived = 0;
    uint16_t dummyDataTransmit = SPI1_DUMMY_DATA;

    uint8_t  *pSend, *pReceived;
    uint16_t addressIncrement;
    uint16_t receiveAddressIncrement, sendAddressIncrement;

    addressIncrement = 1;

    // set the pointers and increment delta 
    // for transmit and receive operations
    if (pTransmitData == NULL)
    {
        sendAddressIncrement = 0;
        pSend = (uint8_t*)&dummyDataTransmit;
    }
    else
    {
        sendAddressIncrement = addressIncrement;
        pSend = (uint8_t*)pTransmitData;
    }
        
    if (pReceiveData == NULL)
    {
       receiveAddressIncrement = 0;
       pReceived = (uint8_t*)&dummyDataReceived;
    }
    else
    {
       receiveAddressIncrement = addressIncrement;        
       pReceived = (uint8_t*)pReceiveData;
    }


    while( SPI1STATLbits.SPITBF == true )
    {

    }

    while (dataSentCount < byteCount)
    {
        if ( SPI1STATLbits.SPITBF != true )
        {

            SPI1BUFL = *pSend;

            pSend += sendAddressIncrement;
            dataSentCount++;

        }

        if (SPI1STATLbits.SPIRBE == false)
        {

            *pReceived = SPI1BUFL;

            pReceived += receiveAddressIncrement;
            dataReceivedCount++;
        }

    }
    while (dataReceivedCount < byteCount)
    {
        if (SPI1STATLbits.SPIRBE == false)
        {

            *pReceived = SPI1BUFL;

            pReceived += receiveAddressIncrement;
            dataReceivedCount++;
        }
    }

    return dataSentCount;
}

uint8_t SPI1_Exchange8bit( uint8_t data )
{
    uint8_t receiveData;
    
    SPI1_Exchange(&data, &receiveData);

    return (receiveData);
}


uint16_t SPI1_Exchange8bitBuffer(uint8_t *dataTransmitted, uint16_t byteCount, uint8_t *dataReceived)
{
    return (SPI1_ExchangeBuffer(dataTransmitted, byteCount, dataReceived));
}

inline __attribute__((__always_inline__)) SPI1_TRANSFER_MODE SPI1_TransferModeGet(void)
{
    if (SPI1CON1Lbits.MODE32 == 1)
        return SPI1_TRANSFER_MODE_32BIT;
    else if (SPI1CON1Lbits.MODE16 == 1)
        return SPI1_TRANSFER_MODE_16BIT;
    else
        return SPI1_TRANSFER_MODE_8BIT;
}

SPI1_STATUS SPI1_StatusGet()
{
    return(SPI1STATL);
}

/**
 End of File
*/
