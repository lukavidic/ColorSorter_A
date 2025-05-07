#ifndef __I2C1_DRIVER_H
#define __I2C1_DRIVER_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
 
typedef void (*interruptHandler)(void);

/* arbitration interface */
/**
 * @brief Disables the I2C1 peripheral by clearing the enable bit.
 *
 * This function turns off the I2C1 hardware module.
 */
void i2c1_driver_close(void);

/* Interrupt interfaces */

/**
 * @brief Enables the I2C1 master and slave interrupt sources.
 *
 * Sets the interrupt enable bits for both master and slave I2C1 events.
 */
void i2c1_enableIRQ(void);
/**
 * @brief Checks whether I2C1 interrupts are currently enabled.
 *
 * @return true if either master or slave interrupt is enabled; false otherwise.
 */
bool i2c1_IRQisEnabled(void);
/**
 * @brief Disables the I2C1 master and slave interrupt sources.
 *
 * Clears the interrupt enable bits for both master and slave I2C1 events.
 */
void i2c1_disableIRQ(void);
/**
 * @brief Clears the I2C1 interrupt flags.
 *
 * Resets the master and slave interrupt flags to 0.
 */
void i2c1_clearIRQ(void);

/**
 * @brief Sets the I2C1 interrupt flags.
 *
 * Forces both the master and slave interrupt flags to 1.
 */
void i2c1_setIRQ(void);
/**
 * @brief Waits for an I2C1 interrupt event with an optional timeout.
 *
 * This function waits in a loop until either the master or slave interrupt flag is set.
 * If both are clear initially, it continuously polls for a change.
 * 
 * @param timeout Pointer to a user-defined timeout value (currently unused).
 */
void i2c1_waitForEvent(uint16_t*);

/* I2C interfaces */
/**
 * @brief Opens the I2C1 driver and initializes the hardware.
 *
 * This function checks if the I2C1 module is disabled and, if so, initializes the hardware
 * by configuring control and status registers, setting the baud rate, and enabling the module.
 *
 * @return true if the I2C1 module was successfully initialized, false if it is already enabled.
 */
bool  i2c1_driver_driver_open(void);
/**
 * @brief Reads a received byte of data from I2C1.
 *
 * This function retrieves the received data from the I2C1RCV register after enabling the RX mode.
 *
 * @return The byte of data received on the I2C1 bus.
 */
char i2c1_driver_getRXData(void);
/**
 * @brief Transmits a byte of data over I2C1.
 *
 * This function sends the specified byte of data over the I2C1 bus by writing it to the
 * I2C1TRN register.
 *
 * @param d The byte of data to be transmitted.
 */
void i2c1_driver_TXData(uint8_t);
/**
 * @brief Resets the I2C1 bus.
 *
 * This function is intended to reset the I2C bus, but it is currently not implemented.
 */
void i2c1_driver_resetBus(void);
/**
 * @brief Starts a new I2C1 communication.
 *
 * This function generates a start condition (SEN) to initiate communication with the slave device.
 */
void i2c1_driver_start(void);
/**
 * @brief Sends a restart condition on I2C1 bus.
 *
 * This function generates a restart condition (RSEN) to resume communication with the slave device
 * without releasing the bus.
 */
void i2c1_driver_restart(void);
/**
 * @brief Sends a stop condition on I2C1 bus.
 *
 * This function generates a stop condition (PEN) to end communication with the slave device and
 * release the bus.
 */
void i2c1_driver_stop(void);
/**
 * @brief Checks if the last I2C1 operation was a NACK (Not Acknowledge).
 *
 * This function checks the ACKSTAT bit in the I2C1STAT register to determine whether the
 * last transmission was acknowledged by the slave.
 *
 * @return true if a NACK was received, false otherwise.
 */
bool i2c1_driver_isNACK(void);

/**
 * @brief Starts receiving data on I2C1.
 *
 * This function enables the receive mode (RCEN) to allow the I2C1 module to receive data
 * from the master device.
 */
void i2c1_driver_startRX(void);
/**
 * @brief Sends an ACK (Acknowledge) signal on the I2C1 bus.
 *
 * This function sets the ACKDT bit to 0 and enables the ACKEN bit to initiate the transmission
 * of an ACK signal to the slave device.
 */
void i2c1_driver_sendACK(void);
/**
 * @brief Sends a NACK (Not Acknowledge) signal on the I2C1 bus.
 *
 * This function sets the ACKDT bit to 1 and enables the ACKEN bit to initiate the transmission
 * of a NACK signal to the slave device.
 */
void i2c1_driver_sendNACK(void);
/**
 * @brief Clears the I2C1 bus collision flag.
 *
 * This function clears the Bus Collision (BCL) flag in the I2C1STAT register. 
 * This is necessary after handling a bus collision event to reset the flag.
 */
void i2c1_driver_clearBusCollision(void);

/**
 * @brief Initializes the I2C1 hardware in slave mode.
 *
 * This function configures the I2C1 module for slave operation by enabling the address
 * and data hold features and setting the strength of the slave pull-ups. The module is then enabled.
 *
 * @return true if the I2C1 module was successfully initialized in slave mode, false if the module is already enabled.
 */
bool  i2c1_driver_initSlaveHardware(void);
/**
 * @brief Releases the I2C1 clock.
 *
 * This function sets the SCLREL bit in the I2C1CONL register to release the I2C1 clock.
 * It is used to free the clock if it has been held by the I2C controller.
 */
void i2c1_driver_releaseClock(void);
/**
 * @brief Checks if the I2C1 buffer is full.
 *
 * This function checks if either the receive buffer (RBF) or transmit buffer (TBF)
 * in the I2C1STAT register is full, indicating that data is ready to be read or written.
 *
 * @return true if the buffer is full, false otherwise.
 */
bool i2c1_driver_isBuferFull(void);

/**
 * @brief Checks if the I2C1 bus is in the start condition.
 *
 * This function checks the value of the S bit in the I2C1STAT register to determine
 * if the I2C bus is currently in the start condition (S). This bit is set when a start condition
 * is detected on the bus.
 *
 * @return true if the bus is in the start condition, false otherwise.
 */
bool i2c1_driver_isStart(void);
/**
 * @brief Checks if the I2C1 bus is in the stop condition.
 *
 * This function checks the value of the P bit in the I2C1STAT register to determine
 * if the bus is currently in the stop condition (P), which indicates the end of a communication.
 *
 * @return true if the bus is in the stop condition, false otherwise.
 */
bool i2c1_driver_isStop(void);

/**
 * @brief Checks if the current I2C1 operation is addressing.
 *
 * This function checks the value of the D_A bit in the I2C1STAT register. If D_A is 0,
 * it indicates that the current I2C operation is addressing, meaning that the master
 * is addressing the slave device.
 *
 * @return true if the current operation is addressing, false otherwise.
 */
bool i2c1_driver_isAddress(void);
/**
 * @brief Checks if the current I2C1 operation is data transfer.
 *
 * This function checks the value of the D_A bit in the I2C1STAT register. If D_A is 1,
 * it indicates that the current operation is data transfer, meaning that the I2C bus is
 * currently transferring data (not addressing).
 *
 * @return true if the current operation is data transfer, false otherwise.
 */
bool i2c1_driver_isData(void);
/**
 * @brief Checks if the current I2C1 operation is a read.
 *
 * This function checks the value of the R_W bit in the I2C1STAT register. If R_W is 1,
 * it indicates that the current operation is a read operation, meaning that the master
 * is reading data from the slave device.
 *
 * @return true if the current operation is a read, false if it is a write.
 */
bool i2c1_driver_isRead(void);

/**
 * @brief Enables the I2C1 start condition interrupt.
 *
 * This function enables the interrupt for the start condition detected on the I2C1 bus
 * by setting the SCIE bit in the I2C1CONH register. When this interrupt is enabled, the
 * I2C controller will generate an interrupt on the start condition.
 */
void i2c1_driver_enableStartIRQ(void);

/**
 * @brief Disables the I2C1 start condition interrupt.
 *
 * This function disables the interrupt for the start condition detected on the I2C1 bus
 * by clearing the SCIE bit in the I2C1CONH register. This prevents the I2C controller
 * from generating interrupts on the start condition.
 */
void i2c1_driver_disableStartIRQ(void);
/**
 * @brief Enables the I2C1 stop condition interrupt.
 *
 * This function enables the interrupt for the stop condition detected on the I2C1 bus
 * by setting the PCIE bit in the I2C1CONH register. When this interrupt is enabled, the
 * I2C controller will generate an interrupt on the stop condition.
 */
void i2c1_driver_enableStopIRQ(void);
/**
 * @brief Disables the I2C1 stop condition interrupt.
 *
 * This function disables the interrupt for the stop condition detected on the I2C1 bus
 * by clearing the PCIE bit in the I2C1CONH register. This prevents the I2C controller
 * from generating interrupts on the stop condition.
 */
void i2c1_driver_disableStopIRQ(void);

/**
 * @brief Sets the interrupt service routine (ISR) for I2C1 bus collision.
 *
 * This function allows the user to set a custom interrupt handler for bus collision events
 * on the I2C1 bus. The handler is called when a bus collision occurs.
 *
 * @param handler The interrupt handler function to be set for the bus collision ISR.
 */
void i2c1_driver_setBusCollisionISR(interruptHandler handler);
/**
 * @brief Sets the interrupt service routine (ISR) for I2C1 master mode.
 *
 * This function allows the user to set a custom interrupt handler for master mode events
 * on the I2C1 bus. The handler is called when the I2C1 controller is in master mode and 
 * the interrupt is triggered.
 *
 * @param handler The interrupt handler function to be set for the master mode ISR.
 */
void i2c1_driver_setMasterI2cISR(interruptHandler handler);
/**
 * @brief Sets the interrupt service routine (ISR) for I2C1 slave mode.
 *
 * This function allows the user to set a custom interrupt handler for slave mode events
 * on the I2C1 bus. The handler is called when the I2C1 controller is in slave mode and 
 * the interrupt is triggered.
 *
 * @param handler The interrupt handler function to be set for the slave mode ISR.
 */
void i2c1_driver_setSlaveI2cISR(interruptHandler handler);

#endif // __I2C1_DRIVER_H
