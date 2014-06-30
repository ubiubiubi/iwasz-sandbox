/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "config.h"

static uint8_t motorNextStep (void);
static uint8_t motorPrevStep (void);
static void motorStartStep (void);

/**
 * BPH | APH state
 */
static uint8_t motorState = 0;

/*
 * DRV8835:
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
}

static void motorStartStep (void)
{
        GPIOPinWrite(GPIO_PORT_MOTOR_BASE, GPIO_PIN_MOTOR_APHASE | GPIO_PIN_MOTOR_AENABLE | GPIO_PIN_MOTOR_BPHASE | GPIO_PIN_MOTOR_BENABLE, 0xF8);
}

/**
 * Feeds paper forward. Accelerates if necessary. Leves motor in STOP pause mode
 * (no excitation).
 */
void motorForward (uint16_t steps)
{
        // Start step : assuming the APH and BPH state is up to date (always should be) set both AEN and BEN to up;
        GPIOPinWrite(GPIO_PORT_MOTOR_BASE, GPIO_PIN_MOTOR_AENABLE | GPIO_PIN_MOTOR_BENABLE, 0xff);
        // Start step (hold for Tmax).
        SysCtlDelay (MOTOR_MAX_T);

        for (int i = 0; i < steps; ++i) {
                // Advance and delay MOTOR_MAX_T
                motorForward1Step ();

                // Additional delay
                SysCtlDelay (motorGetAdditionalDelay (i));
        }

        // Stop step (hold for Tmax).
        SysCtlDelay (MOTOR_MAX_T);

        // Pause (release motor windings. prevent overheating).
        GPIOPinWrite(GPIO_PORT_MOTOR_BASE, GPIO_PIN_MOTOR_AENABLE | GPIO_PIN_MOTOR_BENABLE, 0x00);
}

static uint8_t motorNextStep ()
{
        switch (motorState) {
        case 0:
                return 1;
        case 1:
                return 3;
        case 3:
                return 2;
        case 2:
        default:
                return 0;
        }
}

static uint8_t motorPrevStep ()
{
        switch (motorState) {
        case 0:
                return 2;
        case 1:
                return 0;
        case 3:
                return 1;
        case 2:
        default:
                return 3;
        }
}
