/**
 * @file led.h
 * @brief LED header file
 *
 * This file contains function definitions for LED control
 */

#ifndef LED_H
#define LED_H

#include "../inc/inc.h"

/**
 * Initializes LED module
 *
 * Takes no parameters and returns void
 */
void led_init();

/**
 * Turns LED ON
 *
 * Takes no parameters and returns void
 */
void led_on();

/** Turns LED OFF
 *
 * Takes no parameters and returns void
 */
void led_off();

#endif
