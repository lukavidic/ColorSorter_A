#include "../servo/servo.h"
#include "../color_sensor/color_sensor.h"
#include "../color_sensor/i2c1_driver.h"
#include "../color_sensor/i2c_master.h"
#include "../color_sensor/uart1.h"
#include "../color_sensor/i2c_simple_master.h"
#include "../color_sensor/i2c_types.h"
#include "../color_sensor/spi1.h"
#include "../led/led.h"
#include "../wifi_module/wifi_module.h"
#include "../button/button.h"

#pragma config FNOSC = FRCPLL    // Interni oscilator
#pragma config JTAGEN = OFF   // IskljuCen JTAG
#pragma config FWDTEN = OFF   // IskljuCen Watchdog timer

void pins_init();
void INTERRUPT_Initialize (void);

int main() {
	pins_init();
	servo_init();
	button_init();
	wifi_init();
	led_init();
    led_on();
    
    i2c_write2ByteRegister(VEML3328_SLAVE_ADD, CONF, 0x8011);
    __delay32(16000000);
    i2c_write2ByteRegister(VEML3328_SLAVE_ADD, CONF, 0x0400);
    __delay32(16000000);
    while(1){
        
        WS2812_SetColor(128, 128, 128);
        read_colors();
        //uart_send_string("baba");
        __delay_ms(1000);
        
    }

    
    
	/* while(1) { */
	/* 	i2c_send_data(0x05); */
	/* 	__delay_ms(100); */
	/* 	i2c_get_data(&redv); */
	/* 	__delay_ms(100); */
	/* 	i2c_send_data(0x06); */
	/* 	__delay_ms(100); */
	/* 	i2c_get_data(&greenv); */
	/* 	__delay_ms(100); */
	/* 	i2c_send_data(0x07); */
	/* 	__delay_ms(100); */
	/* 	i2c_get_data(&bluev); */
	/* 	__delay_ms(100); */
	/* 	red = (redv[0] << 8) | redv[1]; */
	/* 	green = (greenv[0] << 8) | greenv[1]; */
	/* 	blue = (bluev[0] << 8) | bluev[1]; */
	/* 	if(red > green && red > blue) */
	/* 		led_on(); */
	/* } */

}

void pins_init() {
    
    LATA = 0x0000;
    LATB = 0x0000;

    TRISBbits.TRISB8  = 1; // SCL - i2c
    TRISBbits.TRISB9  = 1; // SDA - i2c
    //TRISBbits.TRISB10 = 0; // PWM - Servo SG90 communication
    TRISBbits.TRISB5  = 0; // UART1
    TRISBbits.TRISB7  = 0; // OCM1A - WS2812 communication
    TRISBbits.TRISB6  = 0; // SPI1-CLOCK
    
    //TRISA = 0x0013;
    //TRISB = 0xFF1F;

    IOCPDA = 0x0000;
    IOCPDB = 0x0000;
    IOCPUA = 0x0000;
    IOCPUB = 0x0000;

    ODCA = 0x0000;
    ODCB = 0x0000;

    ANSA = 0;//ANSA = 0x000B;
    ANSB = 0;//ANSB = 0xF00C;
   
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS

    RPOR3bits.RP6R = 0x0008;    //RB6->SPI1:SCK1OUT
    RPOR2bits.RP5R = 0x0003;    //RB5->UART1:U1TX
    RPOR3bits.RP7R = 0x0007;    //RB7->SPI1:SDO1

    __builtin_write_OSCCONL(OSCCON | 0x40); // lock PPS
    
	/*ANSB = 0;
	ANSA = 0;
    
    ODCA = 0x0000;
    ODCB = 0x0000;
 
    LATA = 0x0000;
    LATB = 0x0000;
    
    TRISBbits.TRISB8  = 1; // SCL - i2c
    TRISBbits.TRISB9  = 1; // SDA - i2c
    //TRISBbits.TRISB10 = 0; // PWM - Servo SG90 communication
    TRISBbits.TRISB5  = 0; // UART1
    TRISBbits.TRISB7  = 0; // OCM1A - WS2812 communication
    TRISBbits.TRISB6  = 0; // SPI1-CLOCK*/
    
    INTERRUPT_Initialize();
    SPI1_Initialize();
    UART1_Initialize();
    
}
void INTERRUPT_Initialize (void)
{
    //    MICI: MI2C1 - I2C1 Master Events
    //    Priority: 1
        IPC4bits.MI2C1IP = 1;
    //    SICI: SI2C1 - I2C1 Slave Events
    //    Priority: 1
        IPC4bits.SI2C1IP = 1;
}