#pragma config FNOSC = FRC    // Interni oscilator
#pragma config JTAGEN = OFF   // IskljuCen JTAG
#pragma config FWDTEN = OFF   // IskljuCen Watchdog timer

#include "xc.h"
#define FCY 4000000UL
#include <libpic30.h>
#define PRESCALER 256

#include <libpic30.h>

#include "servo.h"
#include "led.h"
#include "button.h"
#include "wifi_module.h"
#include "color_sensor.h"

void pins_init();

int main() {
	pins_init();
	pwm_init();
	servo_init();
	button_init();
	wifi_init();
	led_init();
}

void pins_init() {
	ANSB = 0;
	ANSA = 0
}
