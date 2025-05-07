/**
 * @file i2c_master.c
 * @brief I2C master source file
 *
 * This file contains definitions of functions decleared in
 * i2c_master.h
 */

#include <xc.h>
#include <stdbool.h>
#include <stdlib.h>
#include "i2c_master.h"
#include "i2c_types.h"
#include "i2c1_driver.h"
#include "../inc/inc.h"
#include <xc.h>

/***************************************************************************/
// I2C STATES
typedef enum {
    I2C_IDLE = 0,
    I2C_SEND_ADR_READ,
    I2C_SEND_ADR_WRITE,
    I2C_TX,
    I2C_RX,
    I2C_RCEN,
    I2C_TX_EMPTY,
    I2C_SEND_RESTART_READ,
    I2C_SEND_RESTART_WRITE,
    I2C_SEND_RESTART,
    I2C_SEND_STOP,
    I2C_RX_DO_ACK,
    I2C_RX_DO_NACK_STOP,
    I2C_RX_DO_NACK_RESTART,
    I2C_RESET,
    I2C_ADDRESS_NACK
} i2c_fsm_states_t;

// I2C Event Callback List
typedef enum
{
    i2c_dataComplete = 0,
    i2c_writeCollision,
    i2c_addressNACK,
    i2c_dataNACK,
    i2c_timeOut,
    i2c_NULL
} i2c_callbackIndex;

// I2C Status Structure
typedef struct
{
    unsigned busy:1;
    unsigned inUse:1;
    unsigned bufferFree:1;
    unsigned addressNACKCheck:1;
    i2c_address_t address;       /// The I2C Address
    uint8_t *data_ptr;           /// pointer to a data buffer
    size_t data_length;          /// Bytes in the data buffer
    uint16_t time_out;           /// I2C Timeout Counter between I2C Events.
    uint16_t time_out_value;     /// Reload value for the timeouts
    i2c_fsm_states_t state;      /// Driver State
    i2c_error_t error;
    i2c_callback callbackTable[6];
    void *callbackPayload[6];    ///  each callback can have a payload
} i2c_status_t;

i2c_status_t i2c_status = {0};

/**
 * @brief Sets a callback function and its associated payload for a specific I2C event.
 * 
 * If the provided callback is not NULL, it is stored in the callback table along with its payload.
 * If NULL, a default STOP operation is assigned to ensure safe fallback behavior.
 * 
 * @param idx Index in the callback table indicating which I2C event this callback corresponds to.
 * @param cb Pointer to the callback function to be invoked during that event.
 * @param p Pointer to the payload to be passed to the callback when invoked.
 */
static void setCallBack(i2c_callbackIndex idx, i2c_callback cb, void *p);
/**
 * @brief Default callback function that returns a STOP operation.
 * 
 * Used as a fallback when no user-defined callback is provided.
 * 
 * @param p Unused payload parameter.
 * @return i2c_operations_t Operation code indicating STOP.
 */
static i2c_operations_t returnStop(void *p);
/**
 * @brief Default callback function that returns a RESET operation.
 * 
 * Typically used to indicate that the I2C link should be reset.
 * 
 * @param p Unused payload parameter.
 * @return i2c_operations_t Operation code indicating RESET_LINK.
 */
static i2c_operations_t returnReset(void *p);
/**
 * @brief Polling loop for I2C operations.
 * 
 * This function enters a loop while the I2C bus is marked as busy.
 * It waits for I2C events and invokes the I2C interrupt service routine manually.
 * Used when IRQs are disabled or unavailable.
 */
void i2c_poller(void);


void i2c_setDataCompleteCallback(i2c_callback cb, void *p)
{
    setCallBack(i2c_dataComplete,cb,p);
}
void i2c_setWriteCollisionCallback(i2c_callback cb, void *p)
{
    setCallBack(i2c_writeCollision,cb,p);
}

void i2c_setAddressNACKCallback(i2c_callback cb, void *p)
{
    setCallBack(i2c_addressNACK,cb,p);
}

void i2c_setDataNACKCallback(i2c_callback cb, void *p)
{
    setCallBack(i2c_dataNACK,cb,p);
}

void i2c_setTimeOutCallback(i2c_callback cb, void *p)
{
    setCallBack(i2c_timeOut,cb,p);    
}

// when you call open, you supply a device address.
// if you get the bus, the function returns true
i2c_error_t i2c_open(i2c_address_t address)
{
    i2c_error_t ret = I2C_BUSY;
    
    if(!i2c_status.inUse)
    {
        i2c_status.address = address;
        i2c_status.busy = 0;
        i2c_status.inUse = 1;
        i2c_status.addressNACKCheck = 0;
        i2c_status.state = I2C_RESET;
        i2c_status.time_out_value = 500; // MCC should determine a reasonable starting value here.
        i2c_status.bufferFree = 1;

        // set all the call backs to a default of sending stop
        i2c_status.callbackTable[i2c_dataComplete]=returnStop;
        i2c_status.callbackPayload[i2c_dataComplete] = NULL;
        i2c_status.callbackTable[i2c_writeCollision]=returnStop;
        i2c_status.callbackPayload[i2c_writeCollision] = NULL;
        i2c_status.callbackTable[i2c_addressNACK]=returnStop;
        i2c_status.callbackPayload[i2c_addressNACK] = NULL;
        i2c_status.callbackTable[i2c_dataNACK]=returnStop;
        i2c_status.callbackPayload[i2c_dataNACK] = NULL;
        i2c_status.callbackTable[i2c_timeOut]=returnReset;
        i2c_status.callbackPayload[i2c_timeOut] = NULL;
        
        i2c1_driver_driver_open();
        i2c1_clearIRQ();
        
        i2c1_driver_setBusCollisionISR(i2c_busCollisionISR);
        i2c1_driver_setMasterI2cISR(i2c_ISR);
        i2c1_driver_setSlaveI2cISR(i2c_ISR);
        
        // comment the IRQ enable for a polled driver.
        i2c1_enableIRQ();

        ret = I2C_NOERR;
    }
    return ret;
}

void i2c_setAddress(i2c_address_t address)
{
    i2c_status.address = address;
}

// close the bus if it is not busy and it is my address
i2c_error_t i2c_close(void)
{
    i2c_error_t ret = I2C_BUSY;
    if(!i2c_status.busy)
    {
        i2c_status.inUse = 0;
        // close it down
        i2c_status.address = 0xff; // 8-bit address is invalid so this is FREE
        i2c1_clearIRQ();
        i2c1_disableIRQ();
        ret = i2c_status.error;
    }
    return ret;
}

void i2c_setTimeOut(uint8_t to)
{
    i2c1_disableIRQ();
    i2c_status.time_out_value = to;
    i2c1_enableIRQ();
}

void i2c_setBuffer(void *buffer, size_t bufferSize)
{
    if(i2c_status.bufferFree)
    {
        i2c_status.data_ptr = buffer;
        i2c_status.data_length = bufferSize;
        i2c_status.bufferFree = false;
    }
}
i2c_error_t i2c_masterOperation(bool read)
{
    i2c_error_t ret = I2C_BUSY;
    if(!i2c_status.busy)
    {
        i2c_status.busy = true;
        ret = I2C_NOERR;
        
        if(read)
        {
            i2c_status.state = I2C_SEND_ADR_READ;
        }
        else
        {
            i2c_status.state = I2C_SEND_ADR_WRITE;
        }
        i2c1_driver_start();
        
        if(! i2c1_IRQisEnabled())
            i2c_poller();
    }
    return ret;
}

i2c_error_t i2c_masterRead(void)
{
    return i2c_masterOperation(true);
}

i2c_error_t i2c_masterWrite(void)
{
    return i2c_masterOperation(false);
}

/************************************************************************/
/* Helper Functions                                                     */
/************************************************************************/
void i2c_poller(void)
{
    while(i2c_status.busy)
    {
        i2c1_waitForEvent(NULL);
        i2c_ISR();
    }
}
/**
 * @brief Resets the I2C bus and FSM.
 * 
 * This function performs a hardware bus reset using the low-level driver,
 * clears the busy flag, and sets the error to `I2C_NOERR`.
 * 
 * @return i2c_fsm_states_t Returns `I2C_RESET` to park the FSM.
 */
static i2c_fsm_states_t do_I2C_RESET(void)
{
    i2c1_driver_resetBus();
    i2c_status.busy = false; // Bus Free
    i2c_status.error = I2C_NOERR;
    return I2C_RESET; // park the FSM on reset
}
/**
 * @brief Handles the I2C idle state.
 * 
 * Marks the I2C bus as free and clears any error.
 * 
 * @return i2c_fsm_states_t Returns `I2C_RESET` to park the FSM.
 */
static i2c_fsm_states_t do_I2C_IDLE(void)
{
    i2c_status.busy = false; // Bus Free
    i2c_status.error = I2C_NOERR;
    return I2C_RESET; // park the FSM on reset
}
/**
 * @brief Initiates a repeated START condition for a READ operation.
 * 
 * Sends a repeated start signal and transitions to the address read phase.
 * 
 * @return i2c_fsm_states_t Returns `I2C_SEND_ADR_READ` to send the slave address.
 */
static i2c_fsm_states_t do_I2C_SEND_RESTART_READ(void)
{
    i2c1_driver_restart();
    return I2C_SEND_ADR_READ;
}
/**
 * @brief Initiates a repeated START condition for a WRITE operation.
 * 
 * Sends a repeated start signal and transitions to the address write phase.
 * 
 * @return i2c_fsm_states_t Returns `I2C_SEND_ADR_WRITE` to send the slave address.
 */
static i2c_fsm_states_t do_I2C_SEND_RESTART_WRITE(void)
{
    i2c1_driver_restart();
    return I2C_SEND_ADR_WRITE;
}
/**
 * @brief Initiates a general repeated START condition.
 * 
 * Sends a repeated start and assumes the next step is a read operation.
 * 
 * @return i2c_fsm_states_t Returns `I2C_SEND_ADR_READ`.
 */
static i2c_fsm_states_t do_I2C_SEND_RESTART(void)
{
    i2c1_driver_restart();
    return I2C_SEND_ADR_READ;
}
/**
 * @brief Sends a STOP condition to the I2C bus.
 * 
 * Terminates the current I2C transaction and transitions the FSM to idle.
 * 
 * @return i2c_fsm_states_t Returns `I2C_IDLE`.
 */
static i2c_fsm_states_t do_I2C_SEND_STOP(void)
{
    i2c1_driver_stop();
    return I2C_IDLE;
}
/**
 * @brief Sends the I2C slave address for a READ operation.
 * 
 * Transmits the address with the read bit set and enables reception.
 * 
 * @return i2c_fsm_states_t Returns `I2C_RCEN` to enable receiving data.
 */
static i2c_fsm_states_t do_I2C_SEND_ADR_READ(void)
{
    i2c_status.addressNACKCheck = 1;
    i2c1_driver_TXData(i2c_status.address << 1 | 1);
    return I2C_RCEN;
}
/**
 * @brief Sends the I2C slave address for a WRITE operation.
 * 
 * Transmits the address with the write bit set and transitions to TX state.
 * 
 * @return i2c_fsm_states_t Returns `I2C_TX` to send data.
 */
static i2c_fsm_states_t do_I2C_SEND_ADR_WRITE(void)
{
    i2c_status.addressNACKCheck = 1;
    i2c1_driver_TXData(i2c_status.address << 1);
    return I2C_TX;
}
/**
 * @brief Enables receive mode on the I2C bus.
 * 
 * Clears the address NACK check flag and starts the receive process via the I2C driver.
 * 
 * @return I2C_RX The state indicating the system should proceed with receiving data.
 */
static i2c_fsm_states_t do_I2C_RCEN(void)
{
    i2c_status.addressNACKCheck = 0;
    i2c1_driver_startRX();
    return I2C_RX;
}
/**
 * @brief Sends an ACK (acknowledge) to the I2C slave device.
 * 
 * Informs the slave that data was received successfully and prepares to receive the next byte.
 * 
 * @return I2C_RCEN The next state for continuing data reception.
 */
static i2c_fsm_states_t do_I2C_DO_ACK(void)
{
    i2c1_driver_sendACK();
    return I2C_RCEN;
}
/**
 * @brief Sends a NACK (not acknowledge) and stops the I2C communication.
 * 
 * Used when no further data is expected or in case of an error.
 * 
 * @return I2C_SEND_STOP The state indicating that the STOP condition should be sent.
 */
static i2c_fsm_states_t do_I2C_DO_NACK_STOP(void)
{
    i2c1_driver_sendNACK();
    return I2C_SEND_STOP;
}
/**
 * @brief Sends a NACK (not acknowledge) and initiates a RESTART on the I2C bus.
 * 
 * Used when communication should continue with a repeated START, possibly with a different operation (e.g., read/write).
 * 
 * @return I2C_SEND_RESTART The state to initiate a repeated START condition.
 */
static i2c_fsm_states_t do_I2C_DO_NACK_RESTART(void)
{
    i2c1_driver_sendNACK();
    return I2C_SEND_RESTART;
}

// TODO: probably need 2 addressNACK's one from read and one from write.
//       the do NACK before RESTART or STOP is a special case that a new state simplifies.
/**
 * @brief Handles a NACK received after sending an address.
 * 
 * Resets the address NACK check flag, sets an error status, and executes a user-defined callback to determine the next action.
 * Depending on the callback response, the function either restarts the communication or stops it.
 * 
 * @return Next I2C state based on callback response (RESTART or STOP).
 */
static i2c_fsm_states_t do_I2C_DO_ADDRESS_NACK(void)
{
    i2c_status.addressNACKCheck = 0;
    i2c_status.error = I2C_FAIL;
    switch(i2c_status.callbackTable[i2c_addressNACK](i2c_status.callbackPayload[i2c_addressNACK]))
    {
        case i2c_restart_read:
        case i2c_restart_write:
            return do_I2C_SEND_RESTART();
        default:
            return do_I2C_SEND_STOP();
    }
}
/**
 * @brief Handles data transmission on the I2C bus.
 * 
 * If a NACK is received after sending a byte, a callback determines whether to restart or stop.
 * If ACK is received, sends the next byte or transitions to the TX_EMPTY state if all data is sent.
 * 
 * @return Next I2C state depending on the ACK/NACK and remaining data.
 */
static i2c_fsm_states_t do_I2C_TX(void)
{
    if(i2c1_driver_isNACK())
    {
        switch(i2c_status.callbackTable[i2c_dataNACK](i2c_status.callbackPayload[i2c_dataNACK]))
        {
            case i2c_restart_read:
                return do_I2C_SEND_RESTART_READ();
            case i2c_restart_write:
                return do_I2C_SEND_RESTART_WRITE();
            default:
            case i2c_continue:
            case i2c_stop:
                return do_I2C_SEND_STOP();
        }
    }
    else
    {
        i2c_status.addressNACKCheck = 0;
        i2c1_driver_TXData(*i2c_status.data_ptr++);
        return (--i2c_status.data_length)?I2C_TX:I2C_TX_EMPTY;
    }
}
/**
 * @brief Handles data reception on the I2C bus.
 * 
 * Reads a byte from the bus into the buffer. If more data is expected, sends an ACK and continues.
 * Otherwise, triggers a completion callback and sends a NACK followed by STOP or RESTART based on the callback result.
 * 
 * @return Next I2C state depending on the remaining data and callback result.
 */
static i2c_fsm_states_t do_I2C_RX(void)
{
    *i2c_status.data_ptr++ = i2c1_driver_getRXData();
    if(--i2c_status.data_length)
    {
        i2c1_driver_sendACK();
        return I2C_RCEN;
    }
    else
    {
        i2c_status.bufferFree = true;
        switch(i2c_status.callbackTable[i2c_dataComplete](i2c_status.callbackPayload[i2c_dataComplete]))
        {
            case i2c_restart_write:
            case i2c_restart_read:
                return do_I2C_DO_NACK_RESTART();
            default:
            case i2c_continue:
            case i2c_stop:
                return do_I2C_DO_NACK_STOP();
        }

    }
}
/**
 * @brief Handles the I2C state when all data has been transmitted.
 * 
 * Marks the transmit buffer as free and invokes the data complete callback to determine the next step:
 * - If a RESTART is requested, initiates a RESTART condition.
 * - If CONTINUE is requested, sets the IRQ and returns to transmit state.
 * - Otherwise, sends a STOP condition.
 * 
 * @return Next I2C state based on callback decision.
 */
static i2c_fsm_states_t do_I2C_TX_EMPTY(void)
{
    i2c_status.bufferFree = true;
    switch(i2c_status.callbackTable[i2c_dataComplete](i2c_status.callbackPayload[i2c_dataComplete]))
    {
        case i2c_restart_read:
        case i2c_restart_write:
            return do_I2C_SEND_RESTART();
        case i2c_continue:
            i2c1_setIRQ();
            return I2C_TX;
        default:
        case i2c_stop:
            return do_I2C_SEND_STOP();
    }
}
/**
 * @brief Function pointer typedef for I2C state handler functions.
 * 
 * Each state handler returns the next state in the I2C FSM.
 */
typedef i2c_fsm_states_t (stateHandlerFunction)(void);
/**
 * @brief Table of I2C FSM state handler functions.
 * 
 * Maps each state enum value to its corresponding handler function, allowing dynamic state execution.
 */
const stateHandlerFunction *fsmStateTable[] = {
    do_I2C_IDLE,                //I2C_IDLE
    do_I2C_SEND_ADR_READ,       //I2C_SEND_ADR_READ
    do_I2C_SEND_ADR_WRITE,      //I2C_SEND_ADR_WRITE
    do_I2C_TX,                  //I2C_TX
    do_I2C_RX,                  //I2C_RX
    do_I2C_RCEN,                //I2C_RCEN
    do_I2C_TX_EMPTY,            //I2C_TX_EMPTY
    do_I2C_SEND_RESTART_READ,   //I2C_SEND_RESTART_READ
    do_I2C_SEND_RESTART_WRITE,  //I2C_SEND_RESTART_WRITE
    do_I2C_SEND_RESTART,        //I2C_SEND_RESTART
    do_I2C_SEND_STOP,           //I2C_SEND_STOP
    do_I2C_DO_ACK,              //I2C_RX_DO_ACK
    do_I2C_DO_NACK_STOP,        //I2C_RX_DO_NACK_STOP
    do_I2C_DO_NACK_RESTART,     //I2C_RX_DO_NACK_RESTART
    do_I2C_RESET,               //I2C_RESET
    do_I2C_DO_ADDRESS_NACK      //I2C_ADDRESS_NACK
};
void i2c_ISR(void)
{       
    i2c1_clearIRQ();
    
    // NOTE: We are ignoring the Write Collision flag.
    // the write collision is when SSPBUF is written prematurely (2x in a row without sending)

    // NACK After address override Exception handler
    if(i2c_status.addressNACKCheck && i2c1_driver_isNACK())
    {
        i2c_status.state = I2C_ADDRESS_NACK; // State Override
    }

    i2c_status.state = fsmStateTable[i2c_status.state]();
}
void i2c_busCollisionISR(void)
{
    i2c1_driver_clearBusCollision();
}

/************************************************************************/
/* Helper Functions                                                     */
/************************************************************************/
static i2c_operations_t returnStop(void *p)
{
    return i2c_stop;
}

static i2c_operations_t returnReset(void *p)
{
    return i2c_reset_link;
}

static void setCallBack(i2c_callbackIndex idx, i2c_callback cb, void *p)
{
    if(cb)
    {
        i2c_status.callbackTable[idx] = cb;
        i2c_status.callbackPayload[idx] = p;
    }
    else
    {
        i2c_status.callbackTable[idx] = returnStop;
        i2c_status.callbackPayload[idx] = NULL;
    }
}
