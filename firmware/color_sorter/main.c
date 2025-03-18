#include "button.h";
#include "servo.h";
#include "color_sensor.h";
#include "led.h";
#include "wifi_module.h";

void pins_init();

int main() {
	pins_init();
	servo_init();
	button_init();
	wifi_init();
	led_init();
}

void pins_init() {
	ANSB = 0;
	ANSA = 0;
}
