/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "config.h"
#include "motor.h"

static uint32_t motorGetAdditionalDelay (uint16_t step);
static void motorExcite (uint8_t motorState);

/**
 * BPH | APH state
 */
static uint8_t motorState = 0;

/*
 * DRV8835 (http://www.ti.com/lit/ds/symlink/drv8835.pdf):
 * MODE xENABLE xPHASE xOUT1 xOUT2 (DC MOTOR)
 *   1     0      X      L     L     Brake
 *   1     1      1      L     H     Reverse
 *   1     1      0      H     L     Forward
 */
void motorInit (void)
{
        motorState = 0;
        SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIO_MOTOR);
        GPIOPinTypeGPIOOutput (GPIO_PORT_MOTOR_BASE, GPIO_PIN_MOTOR_APHASE | GPIO_PIN_MOTOR_AENABLE | GPIO_PIN_MOTOR_BPHASE | GPIO_PIN_MOTOR_BENABLE);
        GPIOPinWrite (GPIO_PORT_MOTOR_BASE, GPIO_PIN_MOTOR_APHASE | GPIO_PIN_MOTOR_AENABLE | GPIO_PIN_MOTOR_BPHASE | GPIO_PIN_MOTOR_BENABLE, 0x00);
}

/*
 *
 */
void motorRun (int16_t steps)
{
        // Start step (hold for Tmax).
        motorStartStep ();
        uint16_t absSteps = abs (steps);
        for (uint16_t i = 0; i < absSteps; ++i) {
                // Move one step and delay MOTOR_MAX_T
                if (steps > 0) {
                        motorForward1Step ();
                }
                else {
                        motorBackward1Step ();
                }

                // Additional delay
                SysCtlDelay (motorGetAdditionalDelay (i));
        }

        // Stop step (hold for Tmax).
        motorStopStep ();
}

/**
 * One step forward (holds the windings for MOTOR_MAX_T and leaves in the pause mode).
 */
void motorForward1Step (void)
{
        switch (motorState) {
        case 0:
                motorState = GPIO_PIN_MOTOR_APHASE;
                break;
        case GPIO_PIN_MOTOR_APHASE:
                motorState = GPIO_PIN_MOTOR_APHASE | GPIO_PIN_MOTOR_BPHASE;
                break;
        case GPIO_PIN_MOTOR_APHASE | GPIO_PIN_MOTOR_BPHASE:
                motorState = GPIO_PIN_MOTOR_BPHASE;
                break;
        case GPIO_PIN_MOTOR_BPHASE:
        default:
                motorState = 0;
                break;
        }

        motorExcite (motorState);
}

/**
 * One step backward (holds the windings for MOTOR_MAX_T and leaves in the pause mode).
 */
void motorBackward1Step (void)
{
        switch (motorState) {
        case 0:
                motorState = GPIO_PIN_MOTOR_BPHASE;
                break;
        case GPIO_PIN_MOTOR_APHASE:
                motorState = 0;
                break;
        case GPIO_PIN_MOTOR_APHASE | GPIO_PIN_MOTOR_BPHASE:
                motorState = GPIO_PIN_MOTOR_APHASE;
                break;
        case GPIO_PIN_MOTOR_BPHASE:
        default:
                motorState = GPIO_PIN_MOTOR_APHASE | GPIO_PIN_MOTOR_BPHASE;
                break;
        }

        motorExcite (motorState);
}

/**
 *
 */
void motorStartStep (void)
{
        motorExcite (motorState);
}

/**
 *
 */
void motorStopStep (void)
{
        motorExcite (motorState);
}

/**
 * Additional delays for accelerating the motor. 5V assumed.
 */
static uint32_t motorGetAdditionalDelay (uint16_t step)
{
        switch (step) {
        case 0:
                return DELAY_COEFFICIENT_SCD / 150 - MOTOR_MAX_T_SCD;

        case 1:
        case 2:
                return DELAY_COEFFICIENT_SCD / 265 - MOTOR_MAX_T_SCD;

        case 3:
        case 4:
        case 5:
                return DELAY_COEFFICIENT_SCD / 343 - MOTOR_MAX_T_SCD;

        case 6:
        case 7:
        case 8:
        case 9:
                return DELAY_COEFFICIENT_SCD / 408 - MOTOR_MAX_T_SCD;

        default:
                return 1;
        }
}

/**
 * Turn on the current in the windings.
 */
static void motorExcite (uint8_t motorState)
{
        GPIOPinWrite(GPIO_PORT_MOTOR_BASE, GPIO_PIN_MOTOR_AENABLE | GPIO_PIN_MOTOR_BENABLE, 0xff);
        GPIOPinWrite(GPIO_PORT_MOTOR_BASE, GPIO_PIN_MOTOR_APHASE | GPIO_PIN_MOTOR_BPHASE, motorState);
        SysCtlDelay(MOTOR_MAX_T_SCD);
        GPIOPinWrite(GPIO_PORT_MOTOR_BASE, GPIO_PIN_MOTOR_AENABLE | GPIO_PIN_MOTOR_BENABLE, 0x00);
}

///**
// * Turn off the windings. Prevent overheating.
// */
//static void motorPause (uint8_t motorState)
//{
//        GPIOPinWrite(GPIO_PORT_MOTOR_BASE, GPIO_PIN_MOTOR_AENABLE | GPIO_PIN_MOTOR_BENABLE, 0x00);
//}
