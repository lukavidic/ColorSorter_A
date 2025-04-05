#ifndef COLOR_H
#define COLOR_H

#include "../inc/inc.h"

char* read_colors(); 

char* detect_color();

float color_distance(uint16_t r1, uint16_t g1, uint16_t b1, uint16_t r2, uint16_t g2, uint16_t b2);

void WS2812_Send_Byte(uint8_t byte);

void WS2812_SetColor(uint8_t red, uint8_t green, uint8_t blue);

uint16_t VEML3328_readConfig(uint16_t r);

void uart_send_char(uint8_t c);

void uart_send_string(const char *str);

#endif
