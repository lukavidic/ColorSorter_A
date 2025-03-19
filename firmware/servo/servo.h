#ifndef SERVO_H
#define SERVO_H

#include "../inc/inc.h"

/* servo init function
 * Remaps pins for servo
 */
void servo_init();

/* servo set angle function
 * sets angle of servo by changing OC1R register
 */
void servo_set_angle(int angle);

#endif
