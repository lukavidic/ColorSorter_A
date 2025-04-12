#include "../servo/servo.h"
#include "../color_sensor/color_sensor.h"
#include "../led/led.h"
#include "../wifi_module/wifi_module.h"
#include "../button/button.h"

#pragma config FNOSC = FRCPLL    // Interni oscilator
#pragma config JTAGEN = OFF   // IskljuCen JTAG
#pragma config FWDTEN = OFF   // IskljuCen Watchdog timer

void pins_init();
void clear_buffer();

int main() {
	pins_init();
	servo_init();
	button_init();
	wifi_init();
	wifi_init2();
	led_init();
	wifi_setup_connection();

    while(1) {
		if(strstr(buffer, "Left"))
			servo_left();
		if(strstr(buffer, "Right"))
			servo_right();
		clean_buffer();
		/* wifi_send_app("AT+CAOO\r\n"); */
		/* __delay_ms(100); */
		/* __delay_ms(1000); */
		/* wifi_send_string("AT\r\n"); */
		/* /1* wifi_sendpls2(); *1/ */
		/* for(int i=0;i<BUFF_SIZE-1;i++) */
		/* 	if(buffer[i] == 'O' && buffer[i+1] == 'K') */
		/* 		led_on(); */
    }
}

void pins_init() {
	ANSB = 0;
	ANSA = 0;
}
