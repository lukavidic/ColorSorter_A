#ifndef WIFI_H
#define WIFI_H

#include "../inc/inc.h"

/**
 * @brief initialise wifi module
 *
 * Function called only one time at the start of the program
 */
void wifi_init();

/**
 * @brief send character to wifi module
 *
 * Uses UART protocol to send a signle character to wifi module
 * @param[in] character we want to send
 */
void wifi_send_character(uint8_t character);

#endif
