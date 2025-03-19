#ifndef COLOR_H
#define COLOR_H

#include "../inc/inc.h"

/* vectors where RGB values of color sensor will be stored */
//uint8_t red[2];
//uint8_t green[2];
//uint8_t blue[2];

/* 
 * i2c init function
 * called only one time, at the start of the program
 */
void i2c_init();

/* i2c start function
 * called every time we want to transmitt message
 * to color sensor
 */
void i2c_start();

/* i2c receive function
 * called by i2c_write function
 * to start receiving
 * from color sensor
 */
void i2c_receive();

/* i2c stop function
 * called when we want to stop receiving message
 * from color sensor
 */
void i2c_stop();

/* i2c send data function
 * used to send data
 * to color sensor
 */
void i2c_send_data(uint8_t data);

/* i2c write
 * write byte
 * to I2C1TRN register
 */
void i2c_write(uint8_t data);

/* i2c get data function
 * called when we want to get data from color sensor
 * data is written to receive_data vector
 */
void i2c_get_data(uint8_t receive_data[2]);

#endif
