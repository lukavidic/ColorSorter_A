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
 * Function called only one time at the start of the program
 */
void wifi_init();
void wifi_init2();
void wifi_send_character2(uint8_t character);
void wifi_send_string2(uint8_t* string);
void __attribute__((interrupt(auto_psv))) _U2ErrInterrupt(void);
void __attribute__((interrupt(auto_psv))) _U2RXInterrupt(void);
void __attribute__((interrupt(auto_psv))) _U2TXInterrupt(void);
void wifi_sendpls2();
void wifi_setup_connection();
void clean_buffer();
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
 *
 *
 */
uint8_t wifi_read_character();

/**
 * @brief interrupt routine for UART1 ERROR
 *
 * Addess: 000096h
 */

void __attribute__((interrupt(auto_psv))) _U1ErrInterrupt(void);

/**
 * @brief interrupt routine for UART1 Reciever
 *
 * Address: 00002Ah
 */

void __attribute__((interrupt(auto_psv))) _U1RXInterrupt(void);

/**
 * @brief interrupt routine for UART1 Transmitter
 *
 * Address: 00002Ch
 */

void __attribute__((interrupt(auto_psv))) _U1TXInterrupt(void);

#endif
