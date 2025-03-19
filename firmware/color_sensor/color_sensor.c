#include "color_sensor.h"
#include "../led/led.h"
void i2c_init() {
	TRISBbits.TRISB9 = 0; // set SDA line as output
	TRISBbits.TRISB8 = 0; // set SCL line as output

	LATBbits.LATB8 = 1; // force SDA line to high level
	LATBbits.LATB9 = 1; // force SCL line to high level

	I2C1BRG = 0x12; // set I2C clock rate to 100kHz, when FCY = 4MHz

	I2C1CONLbits.I2CEN = 1; // Enable I2C communication
	__delay_ms(100);
	I2C1CONLbits.A10M = 0; // set I2C1ADD as 7-bit slave address

	/* I2C interrupt setup */
	IEC1bits.MI2C1IE = 1;
	IFS1bits.MI2C1IF = 0;
	IPC4bits.MI2C1IP = 3;

}

void i2c_start() {
    
    led_on();
    __delay_ms(1000);
	I2C1CONLbits.SEN = 1;
	while(!IFS1bits.MI2C1IF);
    led_off();
    __delay_ms(1000);
	IFS1bits.MI2C1IF = 0;
}

void i2c_stop() {
	I2C1CONLbits.PEN = 1;
	while(!IFS1bits.MI2C1IF);
	IFS1bits.MI2C1IF = 0;
}

void i2c_write(uint8_t data) {
	I2C1TRN = data;
	while(I2C1STATbits.ACKSTAT);
}

void i2c_send_data(uint8_t data) {

    
    i2c_start();
    __delay_ms(1000);
    
    
	i2c_write((0x10 << 1) | 0);
	i2c_write(data);
	i2c_write((0x10 << 1) | 1);
    
}

void i2c_receive() {
	I2C1CONLbits.RCEN = 1;
	while(!IFS1bits.MI2C1IF);
	IFS1bits.MI2C1IF = 0;
}

void i2c_get_data(uint8_t receive_data[2]) {
	i2c_receive();
	
	receive_data[0] = I2C1RCV;

	I2C1CONLbits.ACKDT = 0;
	I2C1CONLbits.ACKEN = 1;
	while(I2C1CONLbits.ACKEN);

	i2c_receive();
	receive_data[1] = I2C1RCV;

	I2C1CONLbits.ACKDT = 0;
	I2C1CONLbits.ACKEN = 1;
	while(I2C1CONLbits.ACKEN);

	i2c_stop();
}
