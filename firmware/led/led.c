/**
 * @file led.c
 * @brief LED module source file
 *
 * Contains definitions of functions decleared in led.h
 */

#include "led.h"

void led_init() {
	/* Sets LED pin as output */
	TRISAbits.TRISA2 = 0;
}

void led_on() {
	/* Sets LED state as high */
	LATAbits.LATA2 = 1;
}

void led_off() {
	/* Sets LED state as low */
	LATAbits.LATA2 = 0;
}
