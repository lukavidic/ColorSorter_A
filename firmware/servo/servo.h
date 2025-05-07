/**
 * @file servo.h
 * @brief Header file for servo control
 *
 * Contains funtion for initializing servo module, and setting it's
 * positions
 */

#ifndef SERVO_H
#define SERVO_H

#include "../inc/inc.h"

/**
 * @brief servo init function
 *
 * Remaps pins for servo
 */
void servo_init();

/**
 * @brief servo set angle function
 *
 * Sets angle of servo by changing OC1R register
 */
void servo_set_angle(int angle);

/**
 * @brief Sets position of servo to left
 */
void servo_left();

/**
 * @brief Sets position of servo to center
 */
void servo_center();

/**
 * @brief Sets position of servo to right
 */
void servo_right();

#endif
