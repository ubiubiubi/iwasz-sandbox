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
#include "inputs.h"

void inputsInit (void)
{
        SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIO_INPUTS);
        GPIOPinTypeGPIOInput (GPIO_PORT_INPUTS_BASE, GPIO_PIN_HEAD_TH | GPIO_PIN_HEAD_HDO | GPIO_PIN_MOTOR_PS | GPIO_PIN_MOTOR_HS);
}

