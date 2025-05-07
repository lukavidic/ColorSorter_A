#ifndef COLOR_H
#define COLOR_H

#include "../inc/inc.h"

/**
 *  @brief Reads the color data from the VEML3328 sensor and detects the closest matching color.
 *  
 *  This function collects multiple color samples (red, green, blue) from the VEML3328 sensor, averages the readings, 
 *  and uses the `detect_color` function to identify the closest matching color.
 *  
 *  @return The name of the closest matching color as a string.
 */
char* read_colors(); 
/**
 *  @brief Detects the color based on sensor readings.
 *  
 *  This function compares the RGB values of the detected color from the sensor to a list of predefined colors
 *  and calculates the Euclidean distance to find the closest match.
 *  
 *  @return The name of the closest matching color as a string.
 */
char* detect_color();
/**
 *  @brief Calculates the Euclidean distance between two RGB colors.
 *  
 *  This function calculates the Euclidean distance between two colors in the RGB color space. The smaller the distance,
 *  the closer the two colors are.
 *  
 *  @param r1 The red component of the first color.
 *  @param g1 The green component of the first color.
 *  @param b1 The blue component of the first color.
 *  @param r2 The red component of the second color.
 *  @param g2 The green component of the second color.
 *  @param b2 The blue component of the second color.
 *  
 *  @return The Euclidean distance between the two colors.
 */
float color_distance(uint16_t r1, uint16_t g1, uint16_t b1, uint16_t r2, uint16_t g2, uint16_t b2);
/**
 *  @brief Sends a single byte of data to the WS2812 RGB LED.
 *  
 *  This function sends a byte of data to the WS2812 LED over SPI, with each bit corresponding to a part of the LED data stream.
 *  
 *  @param byte The byte to be sent to the WS2812.
 */
void WS2812_Send_Byte(uint8_t byte);
/**
 *  @brief Sets the color of the WS2812 RGB LED.
 *  
 *  This function sends the color values for red, green, and blue to the WS2812 LED using SPI.
 *  
 *  @param red The red component of the color (0-255).
 *  @param green The green component of the color (0-255).
 *  @param blue The blue component of the color (0-255).
 */
void WS2812_SetColor(uint8_t red, uint8_t green, uint8_t blue);

/**
 *  @brief Reads the configuration register of the VEML3328 color sensor.
 *  
 *  This function reads a 16-bit value from a specified register on the VEML3328 sensor over I2C.
 *  
 *  @param r The register address to be read.
 *  
 *  @return The 16-bit value read from the specified register.
 */
uint16_t VEML3328_readConfig(uint16_t r);

#endif
