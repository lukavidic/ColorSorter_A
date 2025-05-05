/**
 * @file wifi_module.h
 * @brief File for wifi communication
 *
 * This file contains code needed to interract with
 * WIFI module ESP8266EX. It contains functions for initialising
 * wifi module, sending characters and strings to WIFI
 * and user application and receiving data from wifi and application
 */ 

#ifndef WIFI_H
#define WIFI_H

#include "../inc/inc.h"

#define BUFF_SIZE 80

extern volatile char buffer[BUFF_SIZE];
extern volatile unsigned head, tail;
extern unsigned char counter;

/* colors current direction - (1 - right, 0 - left)
 * Order of colors: red, green, blue, yellow, orange, pink, white, black
 */
extern int color_sides[8];

/**
 * @brief initialise wifi module
 *
 * Function called only one time at the start of the program.
 * Enables UART communication with wifi module, configures
 * other UART bits and enables needed interrupts
 */
void wifi_init();

/** 
 * Function called only one time at the start of the program.
 * Sends AT commands to WIFI needed to connect WIFI module to
 * local network and establish connection with running software
 * application
 */
void wifi_setup_connection();

/**
 * @brief Function that cleares buffer[BUFF_SIZE]
 */
void clean_buffer();

/**
 * @brief Send data to software application
 *
 * Function used for sending data to software application
 * @param[in] array of characters we want to send
 */
void wifi_send_app(char* data);

/**
 * @brief send character to wifi module
 *
 * Uses UART protocol to send a signle character to wifi module
 * @param[in] character we want to send
 */
void wifi_send_character(uint8_t character);

/**
 * @brief senc string to wifi module
 * calls wifi_secn_character function for each character
 */
void wifi_send_string(uint8_t* string);

/**
 * @brief get character from wifi module
 */
uint8_t wifi_read_character();

/**
 * @brief interrupt routine for UART1 ERROR
 */

void __attribute__((interrupt(auto_psv))) _U1ErrInterrupt(void);

/**
 * @brief interrupt routine for UART1 Reciever
 */
void __attribute__((interrupt(auto_psv))) _U1RXInterrupt(void);

/**
 * @brief interrupt routine for UART1 Transmitter
 */
void __attribute__((interrupt(auto_psv))) _U1TXInterrupt(void);

#endif
