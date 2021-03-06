/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 *                                                                          *
 *  Lots of code taken from TI:                                             *
 *  Copyright (c) 2012-2014 Texas Instruments Incorporated.                 *
 *  All rights reserved.                                                    *
 *                                                                          *
 ****************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/usb.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_sysctl.h"
#include "usblib/usblib.h"
#include "usblib/usbhid.h"
#include "usblib/device/usbdevice.h"
#include "usblib/device/usbdhid.h"
#include "usblib/device/usbdhidkeyb.h"
#include "driverlib/debug.h"
#include "buttons.h"
#include "utils/uartstdio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/eeprom.h"

#include "config.h"
#include "motor.h"
#include "head.h"
#include "inputs.h"
#include "franek.h"

#ifdef DEBUG
void __error__ (char *pcFilename, unsigned long ulLine)
{
        while (1) {

        }
}
#endif


/**
 *
 */
void uartStdioConfig (uint32_t ui32PortNum, uint32_t ui32Baud, uint32_t ui32SrcClock)
{
        // Check the arguments.
        ASSERT((ui32PortNum == 0) || (ui32PortNum == 1) || (ui32PortNum == 2));

        // Check to make sure the UART peripheral is present.
        if (!SysCtlPeripheralPresent(SYSCTL_PERIPH_UART0)) {
                return;
        }

        // Enable the UART peripheral for use.
        SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

        // Configure the UART for 115200, n, 8, 1
        UARTConfigSetExpClk(UART0_BASE, ui32SrcClock, ui32Baud, (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE |
        UART_CONFIG_WLEN_8));

        // Enable the UART operation.
        UARTEnable(UART0_BASE);
}

/**
 *
 */
void configureUART (void)
{
        // Enable the GPIO Peripheral used by the UART.
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

        // Enable UART0
        SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

        // Configure GPIO Pins for UART mode.
        GPIOPinConfigure(GPIO_PA0_U0RX);
        GPIOPinConfigure(GPIO_PA1_U0TX);
        GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

        // Use the internal 16MHz oscillator as the UART clock source.
        UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

        // Initialize the UART for console I/O.
        uartStdioConfig(0, 115200, 16000000);
}

/*##########################################################################*/



/*##########################################################################*/

int main (void)
{
        SysCtlClockSet (SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

        configureUART ();
        printf ("LTP4335 project.\r\n");

        // Enable the GPIO port that is used for the on-board LED.
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

//        // USB config : Enable the GPIO peripheral used for USB, and configure the USB pins.
//        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
//        SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOD);
//        GPIOPinTypeUSBAnalog(GPIO_PORTD_AHB_BASE, GPIO_PIN_4 | GPIO_PIN_5);

//        ButtonsInit();
//        printf ("Buttons OK.\r\n");

        motorInit ();
        printf ("Motor initialized.\r\n");

        headInit ();
        printf ("Head initialized.\r\n");

//        inputsInit ();
//        printf ("Inputs initialized.\r\n");

        printf ("Test sequence : +40 steps.\r\n");
//        motorRun (40);
        printf ("Test sequence : -40 steps.\r\n");
//        motorRun (-40);
        printf ("Test sequence : OK.\r\n");
        SysCtlDelay (DELAY_COEFFICIENT_SCD / 10);

        headCtrl (false);

        /****************************************************************************/

        int bitsPerPixel = 2;
        int widthPx = 832;
        int pxPerPage = widthPx / HEAD_NUMBER_OF_PAGES;
        int heightPx = franek_gray_len * 8 / (widthPx * bitsPerPixel);

        uint16_t bitMask = 0;

        for (int i = 15; i > 15 - bitsPerPixel; --i) {
                bitMask |= (1 << i);
        }

        uint8_t monochrome[widthPx / 8];
        uint8_t const *p = NULL;
        uint8_t shadesNum = (1 << bitsPerPixel) - 1;
        uint32_t intensityIncremental[] = { 30000, 25000, 20000 };

        for (int j = 0; j < heightPx; ++j) { // Row
                for (int k = 0; k < shadesNum; ++k) { // Shade of gray
                        memset (monochrome, 0, sizeof (monochrome));

                        for (int i = 0; i < widthPx; ++i) { // Column
                                p = franek_gray + (j * widthPx * bitsPerPixel / 8) + (i * bitsPerPixel / 8);
                                uint16_t color = ((int)p[0]) << 8 | ((int)p[1]);
                                color <<= (i * bitsPerPixel) % 8;
                                color &= bitMask;
                                color >>= 15 - bitsPerPixel;
                                monochrome[i / 8] |= ((color <= k) << (7 - (i % 8)));
                        }

                        SysCtlDelay (HEAD_DATA_CLOCK_SCD * 5);
                        headTransferLine1Bit (monochrome);
                        SysCtlDelay (HEAD_DATA_CLOCK_SCD * 5);
                        headLatch ();
                        SysCtlDelay (HEAD_DATA_CLOCK_SCD * 5);

                        for (int j = HEAD_NUMBER_OF_PAGES - 1; j >= 0; --j) {
                                headTransferBdat (1 << j);
                                SysCtlDelay (HEAD_DATA_CLOCK_SCD);
                                headHeatPulse (intensityIncremental[k]);
                                SysCtlDelay (HEAD_DATA_CLOCK_SCD);
                        }
//                        printf ("\r\n");
                }
                motorRun (2);
        }

        /****************************************************************************/

        uint8_t ui8ButtonsChanged, ui8Buttons;

        while (1) {
                ButtonsPoll(&ui8ButtonsChanged, &ui8Buttons);

                if (ui8ButtonsChanged) {
                        if(ui8Buttons & LEFT_BUTTON)
                        {
                                printf ("pressed\r\n");
                        }
                        else {
                                printf ("released\r\n");
                        }
                }
        }
}

