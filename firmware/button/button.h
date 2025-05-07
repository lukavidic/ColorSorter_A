/**
 * @file button.h
 * @brief BUTTON header file
 *
 * This file contains function definitions for BUTTON control
 */
#ifndef BUTTON_H

#define BUTTON_H

#include "../inc/inc.h"

// External variable that counts button presses.
extern volatile int press_counter;

// External variable that controls system reset.
extern int reset_counter;

/**
 * @brief Initializes the button and sets up interrupt configuration.
 * 
 * This function configures pin RB5 as an input and calls the function 
 * to initialize interrupts.
 */
void button_init();
/**
 * @brief Initializes interrupts for the button.
 * 
 * This function sets up the parameters for INT1 interrupt, enables the interrupt 
 * on falling edge, and enables global interrupts.
 */
void initPrekidi();
/**
 * @brief INT1 interrupt handler (button press).
 * 
 * This function is called when the button connected to INT1 is pressed. 
 * It increments the press counter and toggles the system state (turns LED on/off, 
 * centers the servo, and sets RGB color) based on the number of presses.
 */
void __attribute__((interrupt, no_auto_psv)) _INT1Interrupt(void);

#endif
