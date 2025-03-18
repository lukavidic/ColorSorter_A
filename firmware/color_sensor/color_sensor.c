#include "color_sensor.h"

void color_sensor_init() {
	I2C1BRG = 0x12;

	I2C1CONLbits.I2CEN = 1;
	I2C1CONLbits.STRICT = 0;
	I2C1CONLbits.A10M = 0;
	I2C1CONLbits.GCEN = 0;
	I2C1CONLbits.STREN = 0;
	I2C1CONLbits.ACKDT = 1;
	I2C1CONLbits.ACKEN = 1;
	I2C1CONLbits.RCEN = 1;
	I2C1CONLbits.PEN = 1;
	I2C1CONLbits.SEN = 1;

	I2C1MSK = 0x20;

	/* I2C interrupt setup */
	IEC1bits.MI2C1IE = 1;
	IFS1bits.MI2C1IF = 0;
	IPC4bits.MI2C1IP = 3;
}
