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
    
    while(1) {
        servo_set_angle(800);
        __delay_ms(1000);
        servo_set_angle(1000);
        __delay_ms(1000);
        servo_set_angle(400);
        __delay_ms(1000);
    }
}

void pins_init() {
	ANSB = 0;
	ANSA = 0;
}