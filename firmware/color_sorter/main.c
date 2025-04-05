#include "../servo/servo.h"
#include "../color_sensor/color_sensor.h"
#include "../led/led.h"
#include "../wifi_module/wifi_module.h"
#include "../button/button.h"

#pragma config FNOSC = FRCPLL    // Interni oscilator
#pragma config JTAGEN = OFF   // IskljuCen JTAG
#pragma config FWDTEN = OFF   // IskljuCen Watchdog timer

void pins_init();

int main() {
	pins_init();
	servo_init();
	button_init();
	wifi_init();
	wifi_init2();
	led_init();
    led_on();

	wifi_send_string("AT+UART_CUR=115200,8,1,0,0");
	__delay_ms(2000);
    while(1) {
        wifi_send_string("AT");
        __delay_ms(1000);
		wifi_sendpls2();
    }
}

void pins_init() {
	ANSB = 0;
	ANSA = 0;
}
