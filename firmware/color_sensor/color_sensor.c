#include "color_sensor.h"

void color_sensor_init() {
	I2C1BRG = 0x12;

	I2C1CONLbits.I2CEN = 1;
	I2C1CONLbits.STRICT = 0;
	I2C1CONLbits.A10M = 0;
	I2C1CONLbits.GCEN = 0;
	I2C1CONLbits.STREN = 0;
	I2C1CONLbits.ACKDT = 0;
	I2C1CONLbits.RCEN = 0; // Kad zelimo da primamo bajtove, ovaj pin postavljamo na 1
	I2C1CONLbits.PEN = 0; // Kad smo zavrsili sa slanjem bajtova, PEN - Stop condition enable bit stavljamo na 1
	I2C1CONLbits.SEN = 0; // Kad zelimo da saljemo bajtove, Start enable bit SEN stavljamo na 1

	I2C1MSK = 0x20;

	/* I2C interrupt setup */
	IEC1bits.MI2C1IE = 1;
	IFS1bits.MI2C1IF = 0;
	IPC4bits.MI2C1IP = 3;

}

void color_write_data(int data) {
}

int color_get_data() {

}
