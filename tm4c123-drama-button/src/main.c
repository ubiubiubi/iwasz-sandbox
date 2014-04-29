/****************************************************************************
 *                                                                          *
 *  Modified by : lukasz.iwaszkiewicz@gmail.com                             *
 *  ~~~~~~~~                                                                *
 ****************************************************************************/
// Copyright (c) 2012-2014 Texas Instruments Incorporated.  All rights reserved.

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_sysctl.h"
#include "usblib/usblib.h"
#include "usblib/usbhid.h"
#include "usblib/device/usbdevice.h"
#include "usblib/device/usbdhid.h"
#include "usblib/device/usbdhidkeyb.h"
#include "hidDescriptors.h"
#include "deviceDescriptors.h"
#include "driverlib/debug.h"
#include "buttons.h"
#include "rgb.h"
#include "callbacks.h"

#ifdef DEBUG
void __error__ (char *pcFilename, unsigned long ulLine)
{
        while (1) {

        }
}
#endif


/**
 * My initialization routine.
 */
void *lowLevelHIDInit (uint32_t ui32Index, tUSBDHIDDevice *psHIDDevice, CallbackDTO *callbackDTO)
{
        tConfigDescriptor * pConfigDesc;

        uint32_t ui32Loop;

        // Initialize the various fields in our instance structure.
        callbackDTO->ui8USBConfigured = 0;
        callbackDTO->ui8Protocol = USB_HID_PROTOCOL_REPORT;
        callbackDTO->sReportIdle.ui8Duration4mS = 125;
        callbackDTO->sReportIdle.ui8ReportID = 0;
        callbackDTO->sReportIdle.ui32TimeSinceReportmS = 0;
        callbackDTO->sReportIdle.ui16TimeTillNextmS = 0;
        callbackDTO->ui8LEDStates = 0;
        callbackDTO->ui8KeyCount = 0;
        callbackDTO->eKeyboardState = HID_KEYBOARD_STATE_UNCONFIGURED;
        callbackDTO->psHIDDevice = psHIDDevice;

        for(ui32Loop = 0; ui32Loop < KEYB_MAX_CHARS_PER_REPORT; ui32Loop++)
        {
                callbackDTO->pui8KeysPressed[ui32Loop] = HID_KEYB_USAGE_RESERVED;
        }


        psHIDDevice->ppsConfigDescriptor = g_ppsHIDConfigDescriptors;

        void *pvRetcode = USBDHIDCompositeInit (ui32Index, psHIDDevice, 0);

        pConfigDesc = (tConfigDescriptor *) g_pui8KeybDescriptor;
        pConfigDesc->bmAttributes = psHIDDevice->ui8PwrAttributes;
        pConfigDesc->bMaxPower = (uint8_t) (psHIDDevice->ui16MaxPowermA / 2);

        //
        // If we initialized the HID layer successfully, pass our device pointer
        // back as the return code, otherwise return NULL to indicate an error.
        //
        if (pvRetcode) {
                //
                // Initialize the lower layer HID driver and pass it the various
                // structures and descriptors necessary to declare that we are a
                // keyboard.
                //
                pvRetcode = USBDHIDInit (ui32Index, psHIDDevice);

                return ((void *) psHIDDevice);
        }
        else {
                return ((void *) 0);
        }
}

/**
 *
 */
int main (void)
{
        SysCtlClockSet (SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

        // Enable the GPIO port that is used for the on-board LED.
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);


        // USB config : Enable the GPIO peripheral used for USB, and configure the USB pins.
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
        SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOD);
        GPIOPinTypeUSBAnalog(GPIO_PORTD_AHB_BASE, GPIO_PIN_4 | GPIO_PIN_5);

        ButtonsInit();
        RGBInit(0);


        CallbackDTO callbackDTO;

        /**
         * Main device structure.
         */
        tUSBDHIDDevice usbHIDDevice = {
                VENDOR_ID, // The Vendor ID you have been assigned by USB-IF.
                PRODUCT_ID, // The product ID you have assigned for this device.
                200, // The power consumption of your device in milliamps.
                USB_CONF_ATTR_BUS_PWR, // The value to be passed to the host in the USB configuration descriptor’s bmAttributes field.
                USB_HID_SCLASS_BOOT, // This device supports the boot subclass.
                USB_HID_PROTOCOL_KEYB, // This device supports the BIOS keyboard report protocol.
                1, // Only one input report.
                reportIdle, // A pointer to our array of tHIDReportIdle structures. For this device, the array must have 1 element (matching the value of the previous field).
                usbReceiveEventCallback, // A pointer to your receive callback event handler.
                &callbackDTO, // A value that you want passed to the receive callback alongside every event.
                usbTransmitEventCallback, // A pointer to your transmit callback event handler.
                &callbackDTO, // A value that you want passed to the transmit callback alongside every event.
                false, // This device does not want to use a dedicated interrupt OUT endpoint since there are no output or feature reports required.
                &hidDescriptor, // A pointer to the HID descriptor for the device.
                classDescriptors, // A pointer to the array of HID class descriptor pointers for this device. The number of elements in this array and their order must match the information in the HID descriptor provided above.
                stringDescriptors, // A pointer to your string table.
                NUM_STRING_DESCRIPTORS, // The number of entries in your string table. This must equal (1 + (5 * (num languages))).
                0
        };

        // Set the USB stack mode to Device mode.
        USBStackModeSet(0, eUSBModeForceDevice, 0);
        tUSBDHIDDevice *device = (tUSBDHIDDevice *)lowLevelHIDInit (0, &usbHIDDevice, &callbackDTO);

        ASSERT (device);

        uint32_t ui32Loop;
        uint8_t ui8ButtonsChanged, ui8Buttons;

        uint32_t color1[3] = { 0xffffu, 0x0000u, 0x0000u } ;
        uint32_t color2[3] = { 0x0000u, 0xffffu, 0x0000u } ;

        while (1) {
                ButtonsPoll(&ui8ButtonsChanged, &ui8Buttons);


//                if(ui8Buttons & RIGHT_BUTTON)
//                {
//                        RGBSet (color2, 0.01);
//                        RGBEnable();
//                }
//                else {
//                        RGBDisable ();
//                }

                if (ui8ButtonsChanged) {


                        if(ui8Buttons & LEFT_BUTTON)
                        {
                                RGBSet(color1, 0.01);
                                RGBEnable();
                                callbackDTO.pui8Report[0] = 0x00;
                                callbackDTO.pui8Report[1] = 0x00;
                                callbackDTO.pui8Report[2] = 0x04;

//                                callbackDTO.pui8Report[8] = 0x08; // play / pause
//                                //                                callbackDTO.pui8Report[0] = 0x10; // Vol up?
                        }
                        else {
                                RGBDisable ();
//                                for (int i = 0; i < KEYB_IN_REPORT_SIZE; ++i) {
//                                        callbackDTO.pui8Report[i] = 0x00;
//                                }
                                callbackDTO.pui8Report[0] = 0x00;
                                callbackDTO.pui8Report[1] = 0x00;
                                callbackDTO.pui8Report[2] = 0x00;
                        }


                        if (USBDHIDTxPacketAvailable(device)) {
                                // Send the report to the host.
                                callbackDTO.eKeyboardState = HID_KEYBOARD_STATE_SEND;

                                uint32_t ui32Count = USBDHIDReportWrite(device, callbackDTO.pui8Report, KEYB_IN_REPORT_SIZE, true);

                                if (ui32Count != 0) {
                                        callbackDTO.bChangeMade = false;
                                }
                        }
                        else
                        {
                            // We can't send the report immediately so mark the instance so that
                            // it is sent next time the transmitter is free.
                            callbackDTO.bChangeMade = true;
                        }
                }

//                // DELAY
//                for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++)
//                {
//                }
        }
}

