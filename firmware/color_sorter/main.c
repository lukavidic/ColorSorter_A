#include "../inc/inc.h"
#include "../servo/servo.h"
#include "../color_sensor/color_sensor.h"
#include "../led/led.h"
#include "../wifi_module/wifi_module.h"
#include "../button/button.h"

#pragma config FNOSC = FRC    // Interni oscilator
#pragma config JTAGEN = OFF   // IskljuCen JTAG
#pragma config FWDTEN = OFF   // IskljuCen Watchdog timer

void pins_init();

int main() {
	pins_init();
	servo_init();
	button_init();
	wifi_init();
	led_init();
    led_on();
    __delay_ms(1000);
	i2c_init();
    led_off();
    __delay_ms(1000);
    
    i2c_send_data(0x00);
    
    uint8_t redv[2];
    uint8_t greenv[2];
    uint8_t bluev[2];

	uint16_t red;
	uint16_t green;
	uint16_t blue;
    while(1);

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
	ANSB = 0;
	ANSA = 0;
}
