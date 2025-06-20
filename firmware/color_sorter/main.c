/**
 * @file main.c
 * @brief File that contains main function of project
 *
 * This file contains main function that calls initialize functions
 * of all modules needed for proper functioning of project
 */

#include "../servo/servo.h"
#include "../color_sensor/color_sensor.h"
#include "../color_sensor/i2c1_driver.h"
#include "../color_sensor/i2c_master.h"
#include "../color_sensor/i2c_simple_master.h"
#include "../color_sensor/i2c_types.h"
#include "../color_sensor/spi1.h"
#include "../led/led.h"
#include "../inc/inc.h"
#include "../wifi_module/wifi_module.h"
#include "../button/button.h"

#pragma config FNOSC = FRCPLL    // Interni oscilator
#pragma config JTAGEN = OFF   // IskljuCen JTAG
#pragma config FWDTEN = OFF   // IskljuCen Watchdog timer

void pins_init();
void INTERRUPT_Initialize (void);

int main() {
	pins_init();
    ANSA =0;
    ANSB =0;
	servo_init();
	button_init();
	wifi_init();
	led_init();
	wifi_setup_connection();

    i2c_write2ByteRegister(VEML3328_SLAVE_ADD, CONF, 0x8011);
	__delay_ms(1000);
    i2c_write2ByteRegister(VEML3328_SLAVE_ADD, CONF, 0x0430);
	__delay_ms(2000);
	__delay32(32000000);
    char* detected_color = "Nepoznata boja";
    while(1){
        if (reset_counter == 1) {
            WS2812_SetColor(128, 128, 128);
            detected_color = read_colors();
            if(strcmp(detected_color, "CRVENA")==0){
				if(color_sides[0] == 1)
					servo_right();
				else servo_left();
				wifi_send_app("000\r\n");
            }else if(strcmp(detected_color, "ZELENA")==0){
				if(color_sides[1] == 1)
					servo_right();
				else servo_left();
				wifi_send_app("111\r\n");
            }else if(strcmp(detected_color, "PLAVA")==0){
				if(color_sides[2] == 1)
					servo_right();
				else servo_left();
				wifi_send_app("222\r\n");
            }else if(strcmp(detected_color, "ZUTA")==0){
				if(color_sides[3] == 1)
					servo_right();
				else servo_left();
				wifi_send_app("333\r\n");
            }else if(strcmp(detected_color, "NARANDZASTA")==0){
				if(color_sides[4] == 1)
					servo_right();
				else servo_left();
				wifi_send_app("444\r\n");
            }else if(strcmp(detected_color, "ROZA")==0){
				if(color_sides[5] == 1)
					servo_right();
				else servo_left();
				wifi_send_app("555\r\n");
            }else if(strcmp(detected_color, "BIJELA")==0 || strcmp(detected_color, "OBIJELA")==0){
				if(color_sides[6] == 1)
					servo_right();
				else servo_left();
				wifi_send_app("666\r\n");
			}else if(strcmp(detected_color, "CRNA")==0 || strcmp(detected_color, "OCRNA")==0){
				if(color_sides[7] == 1)
					servo_right();
				else servo_left();
				wifi_send_app("777\r\n");
            }else if(strcmp(detected_color, "NO")==0){
                servo_center();
            }
            __delay_ms(400);
            servo_center();
        }else{
            __delay_ms(500);
        }
    }
}

void pins_init() {
    
    LATA = 0x0000;
    LATB = 0x0000;

    TRISBbits.TRISB8  = 1; // SCL - i2c
    TRISBbits.TRISB9  = 1; // SDA - i2c
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
   
	OSCCONbits.IOLOCK = 0;

    RPOR3bits.RP6R = 0x0008;    //RB6->SPI1:SCK1OUT
    RPOR3bits.RP7R = 0x0007;    //RB7->SPI1:SDO1
    RPINR0bits.INT1R = 5;    	//RB5->Button interrupt

	OSCCONbits.IOLOCK = 1;
    
    INTERRUPT_Initialize();
    SPI1_Initialize();
    
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
