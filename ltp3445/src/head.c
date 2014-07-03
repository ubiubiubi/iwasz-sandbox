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
#include "head.h"

void headInit (void)
{
        SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIO_HEAD);
        GPIODirModeSet (GPIO_PORT_HEAD_BASE, GPIO_PIN_HEAD_ALL, GPIO_DIR_MODE_OUT);
        GPIOPadConfigSet (GPIO_PORT_HEAD_BASE, GPIO_PIN_HEAD_ALL, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);
        GPIOPinWrite (GPIO_PORT_HEAD_BASE, GPIO_PIN_HEAD_ALL, GPIO_PIN_HEAD_BCLR | GPIO_PIN_HEAD_LATCH);
}

void headCtrl (bool bit)
{
        GPIOPinWrite (GPIO_PORT_HEAD_BASE, GPIO_PIN_HEAD_CTRL, (bit) ? 0xff : 0x00);
}

void headLatch ()
{
        GPIOPinWrite (GPIO_PORT_HEAD_BASE, GPIO_PIN_HEAD_LATCH, 0x00);
        SysCtlDelay (HEAD_DATA_CLOCK_SCD);
        GPIOPinWrite (GPIO_PORT_HEAD_BASE, GPIO_PIN_HEAD_LATCH, 0xff);
}

void headTransferLine1Bit (uint8_t *data)
{
        for (int i = 0; i < HEAD_BYTES_IN_LINE; ++i) {
                for (int j = 7; j >= 0; --j) {
                        GPIOPinWrite (GPIO_PORT_HEAD_BASE, GPIO_PIN_HEAD_HCLK, 0x00);
                        SysCtlDelay (HEAD_DATA_CLOCK_SCD_4);
                        uint8_t currentState = (*(data + i) & (1 << j)) ? GPIO_PIN_HEAD_HDAT : 0x00;
                        GPIOPinWrite (GPIO_PORT_HEAD_BASE, GPIO_PIN_HEAD_HDAT,  currentState);
                        SysCtlDelay (HEAD_DATA_CLOCK_SCD_4);
                        GPIOPinWrite (GPIO_PORT_HEAD_BASE, GPIO_PIN_HEAD_HCLK, GPIO_PIN_HEAD_HCLK);
                        SysCtlDelay (HEAD_DATA_CLOCK_SCD_2);
                }
        }
}

/**
 *
 */
void headTransferBdat (uint16_t pages)
{
        for (int j = HEAD_NUMBER_OF_PAGES; j >= 0; --j) {
                GPIOPinWrite (GPIO_PORT_HEAD_BASE, GPIO_PIN_HEAD_BCLK, 0x00);
                SysCtlDelay (HEAD_DATA_CLOCK_SCD_4);
                uint8_t currentState = (pages & (1 << j)) ? GPIO_PIN_HEAD_BDAT : 0x00;
                GPIOPinWrite (GPIO_PORT_HEAD_BASE, GPIO_PIN_HEAD_BDAT,  currentState);
                SysCtlDelay (HEAD_DATA_CLOCK_SCD_4);
                GPIOPinWrite (GPIO_PORT_HEAD_BASE, GPIO_PIN_HEAD_BCLK, GPIO_PIN_HEAD_BCLK);
                SysCtlDelay (HEAD_DATA_CLOCK_SCD_2);
        }
}

void headHeatPulse (void)
{
        GPIOPinWrite (GPIO_PORT_HEAD_BASE, GPIO_PIN_HEAD_DST, 0xff);
        SysCtlDelay (HEAD_DATA_CLOCK_SCD);
        GPIOPinWrite (GPIO_PORT_HEAD_BASE, GPIO_PIN_HEAD_DST, 0x00);
}
