#include "color_sensor.h"
#include "../led/led.h"
#include "../inc/inc.h"
#include "../color_sensor/uart1.h"
#include "../color_sensor/i2c_simple_master.h"
#include "../color_sensor/i2c_master.h"

#include "xc.h"
#include <stdio.h>
#include <math.h>
#include <stdint.h>

uint16_t sensor_value_clear;
uint16_t sensor_value_red;
uint16_t sensor_value_green;
uint16_t sensor_value_blue;

typedef struct {
    const char* name;
    uint16_t r, g, b;
} Color;

const Color colors[] = {
    {"ZELENA",     6500, 29000, 28000},
    {"PLAVA",      8800, 45100, 65500},
    {"CRVENA",    40500, 21800, 24400},
    {"ZUTA",      45000, 65535, 40500},
    {"NARANDZASTA",40700, 18800, 14500},
    {"ROZA",      34100, 20200, 29800},
    {"CRNA",       6000, 12300, 16500},
    {"BIJELA",    48900, 65535, 65535},
  //{"SMEDJA",     8000, 11800, 14700},
    {"NO",         15700,  19000,  21000}
};

char* detect_color(){
    
    uint16_t r = sensor_value_red;
    uint16_t g = sensor_value_green;
    uint16_t b = sensor_value_blue;

    float min_distance = 1e9; 
    const char* detected_color = "NEPOZNATA BOJA";

    for (int i = 0; i < NUM_COLORS; i++) {
        float distance = color_distance(r, g, b, colors[i].r, colors[i].g, colors[i].b);
        if (distance < min_distance) {
            min_distance = distance;
            detected_color = colors[i].name;
        }
    }

    uart_send_string("Detektovana: ");
    uart_send_string(detected_color);
    uart_send_string("\r\n");
    
    return detected_color;
    
}

void WS2812_Send_Byte(uint8_t byte){
    for (int i = 7; i >= 0; i--)  
    {
        uint8_t spi_data = (byte & (1 << i)) ? 0b11110000 : 0b11000000; 
        SPI1_Exchange8bit(spi_data);
    }
}

void WS2812_SetColor(uint8_t red, uint8_t green, uint8_t blue){
  
    SPI1CON1Lbits.SPIEN = 0;
    __delay_us(50);
    SPI1CON1Lbits.SPIEN = 1;
    
    WS2812_Send_Byte(green);
    WS2812_Send_Byte(red);
    WS2812_Send_Byte(blue);
     
    SPI1CON1Lbits.SPIEN = 0;
    __delay_us(50);
    SPI1CON1Lbits.SPIEN = 1;
      
}

uint16_t VEML3328_readConfig(uint16_t r){    
    uint16_t reg = i2c_read2ByteRegister(VEML3328_SLAVE_ADD, r); 
    return ((reg & 0xFF) << 8) | (reg >> 8);
}

char* read_colors(){
    
    char buff[100];
    uint16_t values_red[NUM_SAMPLES];
    uint16_t values_green[NUM_SAMPLES];
    uint16_t values_blue[NUM_SAMPLES];

    uint32_t sum_red = 0, sum_green = 0, sum_blue = 0;
    uint16_t avg_red, avg_green, avg_blue;

    __delay_ms(1000);

    
    for (int i = 0; i < NUM_SAMPLES; i++) {
        
        sum_red += VEML3328_readConfig(R_DATA);
        sum_green += VEML3328_readConfig(G_DATA);
        sum_blue += VEML3328_readConfig(B_DATA);

        __delay_ms(100);
     
    }

    avg_red = sum_red / NUM_SAMPLES;
    avg_green = sum_green / NUM_SAMPLES;
    avg_blue = sum_blue / NUM_SAMPLES;

    sensor_value_red = avg_red;
    sensor_value_blue = avg_blue;
    sensor_value_green = avg_green;

    sprintf(buff, "AVG Red = %u, AVG Green = %u, AVG Blue = %u\r\n", sensor_value_red, sensor_value_green, sensor_value_blue);
    uart_send_string(buff);
    
    return detect_color();
    
} 

float color_distance(uint16_t r1, uint16_t g1, uint16_t b1, uint16_t r2, uint16_t g2, uint16_t b2){
    
    return sqrtf(powf(r1 - r2, 2) + powf(g1 - g2, 2) + powf(b1 - b2, 2));
    
}

//Used for debugging
void uart_send_char(uint8_t c) {
    while (U1STAbits.UTXBF);  
    U1TXREG = c;
}

void uart_send_string(const char *str) {
    while (*str) {
        //UART1_Write(*str++);
        uart_send_char(*str++);
    }
}


/*void i2c_init() {
    
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS

    RPOR2bits.RP5R = 0x0003;    //RB5->UART1:U1TX

    __builtin_write_OSCCONL(OSCCON | 0x40); // lock PPS
    
	/*TRISBbits.TRISB9 = 0; // set SDA line as output
	TRISBbits.TRISB8 = 0; // set SCL line as output

	LATBbits.LATB8 = 1; // force SDA line to high level
	LATBbits.LATB9 = 1; // force SCL line to high level

	I2C1BRG = 0x12; // set I2C clock rate to 100kHz, when FCY = 4MHz

	I2C1CONLbits.I2CEN = 1; // Enable I2C communication
	__delay_ms(100);
	I2C1CONLbits.A10M = 0; // set I2C1ADD as 7-bit slave address
    
	IEC1bits.MI2C1IE = 1;
	IFS1bits.MI2C1IF = 0;
	IPC4bits.MI2C1IP = 3;

}
void i2c_start() {
    
    led_on();
    __delay_ms(500);
    while (I2C1STATbits.P == 0);
    led_off();
    __delay_ms(500);
	I2C1CONLbits.SEN = 1;
	while(!IFS1bits.MI2C1IF);
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
}*/