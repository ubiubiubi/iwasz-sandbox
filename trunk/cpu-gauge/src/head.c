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
#include <math.h>
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

void headTransferLine1Bit (uint8_t const *data)
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

        GPIOPinWrite (GPIO_PORT_HEAD_BASE, GPIO_PIN_HEAD_HCLK | GPIO_PIN_HEAD_HDAT, 0x00);
}

/**
 *
 */
void headTransferBdat (uint16_t pages)
{
        for (int j = HEAD_NUMBER_OF_PAGES - 1; j >= 0; --j) {
                GPIOPinWrite (GPIO_PORT_HEAD_BASE, GPIO_PIN_HEAD_BCLK, 0x00);
                SysCtlDelay (HEAD_DATA_CLOCK_SCD_4);
                uint8_t currentState = (pages & (1 << j)) ? GPIO_PIN_HEAD_BDAT : 0x00;
                GPIOPinWrite (GPIO_PORT_HEAD_BASE, GPIO_PIN_HEAD_BDAT,  currentState);
                SysCtlDelay (HEAD_DATA_CLOCK_SCD_4);
                GPIOPinWrite (GPIO_PORT_HEAD_BASE, GPIO_PIN_HEAD_BCLK, GPIO_PIN_HEAD_BCLK);
                SysCtlDelay (HEAD_DATA_CLOCK_SCD_2);
        }

        GPIOPinWrite (GPIO_PORT_HEAD_BASE, GPIO_PIN_HEAD_BCLK | GPIO_PIN_HEAD_BDAT, 0x00);
}

void headHeatPulse (uint32_t scdDelay)
{
        if (scdDelay <= 0 || scdDelay > 100000) {
                GPIOPinWrite (GPIO_PORT_HEAD_BASE, GPIO_PIN_HEAD_DST, 0x00);
                return;
        }

        GPIOPinWrite (GPIO_PORT_HEAD_BASE, GPIO_PIN_HEAD_DST, 0xff);
        SysCtlDelay (scdDelay);
        GPIOPinWrite (GPIO_PORT_HEAD_BASE, GPIO_PIN_HEAD_DST, 0x00);

//        for (int i = 0; i < 20; ++i) {
//                GPIOPinWrite (GPIO_PORT_HEAD_BASE, GPIO_PIN_HEAD_DST, 0xff);
//                SysCtlDelay (1500);
//                GPIOPinWrite (GPIO_PORT_HEAD_BASE, GPIO_PIN_HEAD_DST, 0x00);
//                SysCtlDelay (1500);
//        }
//
//        GPIOPinWrite (GPIO_PORT_HEAD_BASE, GPIO_PIN_HEAD_DST, 0x00);
}

float headThermistorCelsius ()
{
        return 40.0;
}

float headPulseWidthLTP3445SCD (uint16_t dotsNum)
{
        float e = (0.32 - HEAD_TEMPERATURE_COEFFICIENT * (headThermistorCelsius () - 25)) * 1.0;
        // TODO const static or define.
        float va = (HEAD_VOLTAGE > 5) ? (HEAD_VOLTAGE - 0.9) : (HEAD_VOLTAGE * 1.26 - 2.46);

        float r = pow ((HEAD_RESISTANCE_OHM + 60 + (0.1 + HEAD_WIRING_RESISTANCE_OHM) * dotsNum), 2) / HEAD_RESISTANCE_OHM;

        // TODO rather actual pps than max pps should be here:
        float w = 2000.0 / MOTOR_MAX_PPS;

        float cm = (1 - 3.5) / (3.5 + w);
        float ca = (cm >= 0.75) ? (1) : (cm + 0.25);


        // Main and preheat head pulse combined width in ms (because I'm using ca which is cm + cs).
        float tms = (e * r * ca) / (va * va);

        // ?
        return tms * DELAY_COEFFICIENT_SCD;
}
