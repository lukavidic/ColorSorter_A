#include "led.h"

void led_init() {
	TRISAbits.TRISA2 = 0;
}

void led_on() {
	LATAbits.LATA2 = 1;
}

void led_off() {
	LATAbits.LATA2 = 0;
}
