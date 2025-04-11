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

    while(1) {
		/* clear_buffer(); */
		wifi_send_string("AT\r\n");
        __delay_ms(1000);
		wifi_sendpls2();
		for(int i=0;i<BUFF_SIZE-1;i++)
			if(buffer[i] == 'O' && buffer[i+1] == 'K')
				led_on();
    }
}

void clear_buffer() {
	for(int i=0;i<BUFF_SIZE;i++){
		buffer[i] = '0';
	}
}

void pins_init() {
	ANSB = 0;
	ANSA = 0;
}
