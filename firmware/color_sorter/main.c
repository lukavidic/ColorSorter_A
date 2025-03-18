#include "../inc/inc.h"
#include "../servo/servo.h"
#include "../color_sensor/color_sensor.h"
#include "../led/led.h"
#include "../wifi_module/wifi_module.h"
#include "../button/button.h"

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
