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

#ifdef DEBUG
void __error__ (char *pcFilename, unsigned long ulLine)
{
        while (1) {

        }
}
#endif

/**
 * The languages supported by this device.
 */
const uint8_t g_pui8LangDescriptor[] =
{
    4,
    USB_DTYPE_STRING,
    USBShort(USB_LANG_EN_US)
};

/**
 * The manufacturer string.
 */
const uint8_t g_pui8ManufacturerString[] =
{
    (8 + 1) * 2,
    USB_DTYPE_STRING,
    'I', 0, 'w', 0, 'a', 0, 's', 0, 'z', 0, '.', 0, 'p', 0, 'l', 0
};

/**
 * The product string.
 */
const uint8_t g_pui8ProductString[] =
{
    (12 + 1) * 2,
    USB_DTYPE_STRING,
    'D', 0, 'r', 0, 'a', 0, 'm', 0, 'a', 0, ' ', 0, 'b', 0, 'u', 0,
    't', 0, 't', 0, 'o', 0, 'n'
};

/**
 * The serial number string.
 */
const uint8_t g_pui8SerialNumberString[] =
{
    (2 + 1) * 2,
    USB_DTYPE_STRING,
    '0', 0, '1', 0
};

/**
 * The interface description string.
 */
const uint8_t g_pui8HIDInterfaceString[] =
{
    (22 + 1) * 2,
    USB_DTYPE_STRING,
    'H', 0, 'I', 0, 'D', 0, ' ', 0, 'K', 0, 'e', 0, 'y', 0, 'b', 0,
    'o', 0, 'a', 0, 'r', 0, 'd', 0, ' ', 0, 'I', 0, 'n', 0, 't', 0, 'e', 0,
    'r', 0, 'f', 0, 'a', 0, 'c', 0, 'e', 0
};

/**
 * The configuration description string.
 */
const uint8_t g_pui8ConfigString[] =
{
    (26 + 1) * 2,
    USB_DTYPE_STRING,
    'H', 0, 'I', 0, 'D', 0, ' ', 0, 'K', 0, 'e', 0, 'y', 0, 'b', 0,
    'o', 0, 'a', 0, 'r', 0, 'd', 0, ' ', 0, 'C', 0, 'o', 0, 'n', 0, 'f', 0,
    'i', 0, 'g', 0, 'u', 0, 'r', 0, 'a', 0, 't', 0, 'i', 0, 'o', 0,
    'n', 0
};

/**
 * The descriptor string table.
 */
const uint8_t * const stringDescriptors[] =
{
    g_pui8LangDescriptor,
    g_pui8ManufacturerString,
    g_pui8ProductString,
    g_pui8SerialNumberString,
    g_pui8HIDInterfaceString,
    g_pui8ConfigString
};

#define NUM_STRING_DESCRIPTORS (sizeof(stringDescriptors) / sizeof(uint8_t *))

/**
 * Report descriptor.
 */
static const uint8_t reportDescriptor[]=
{
        UsagePage(USB_HID_GENERIC_DESKTOP),
        Usage(USB_HID_KEYBOARD),
        Collection(USB_HID_APPLICATION),
                ReportCount(1),
                ReportSize(8),
                LogicalMinimum(0),
                LogicalMaximum(101),
                UsagePage(USB_HID_USAGE_KEYCODES),
                UsageMinimum (0),
                UsageMaximum (101),
                Input(USB_HID_INPUT_DATA | USB_HID_INPUT_ARRAY),
        EndCollection,
};

/**
 * The HID class descriptor table. For the mouse class, we have only a single
 * report descriptor.
 */
static const uint8_t * const classDescriptors[] =
{
        reportDescriptor
};

/**
 * The HID descriptor for the mouse device.
 */
static const tHIDDescriptor hidDescriptor =
{
        9, // bLength
        USB_HID_DTYPE_HID, // bDescriptorType
        0x111, // bcdHID (version 1.11 compliant)
        0, // bCountryCode (not localized)
        1, // bNumDescriptors
        USB_HID_DTYPE_REPORT,
        // Report descriptor
        sizeof (reportDescriptor) // Size of report descriptor
};

tHIDReportIdle reportIdle[1] = {
        { 125, 1, 0, 0 }, // Report 1 polled every 500mS (4 * 125).
};

#define VENDOR_ID 0x20a0
#define PRODUCT_ID 0x41ff

uint32_t usbReceiveEventCallback (void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgParam, void *pvMsgData);
uint32_t usbTransmitEventCallback (void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgParam, void *pvMsgData);

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
        &usbHIDDevice, // A value that you want passed to the receive callback alongside every event.
        usbTransmitEventCallback, // A pointer to your transmit callback event handler.
        &usbHIDDevice, // A value that you want passed to the transmit callback alongside every event.
        false, // This device does not want to use a dedicated interrupt OUT endpoint since there are no output or feature reports required.
        &hidDescriptor, // A pointer to the HID descriptor for the device.
        classDescriptors, // A pointer to the array of HID class descriptor pointers for this device. The number of elements in this array and their order must match the information in the HID descriptor provided above.
        stringDescriptors, // A pointer to your string table.
        NUM_STRING_DESCRIPTORS, // The number of entries in your string table. This must equal (1 + (5 * (num languages))).
        0
};

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

        // Set the USB stack mode to Device mode.
        USBStackModeSet(0, eUSBModeForceDevice, 0);

        tUSBDHIDDevice *device = (tUSBDHIDDevice *)USBDHIDInit (0, &usbHIDDevice);
//        tUSBDHIDDevice *device = (tUSBDHIDDevice *)USBDHIDKeyboardInit (0, &usbHIDDevice);
        GPIOPinWrite (GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);

        if (!device) {
                while (1);
        }

        GPIOPinWrite (GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);

        uint32_t ui32Loop;

        while (1) {
                uint8_t buf = 0x04;
//                USBDHIDKeyboardKeyStateChange (device, 0, 0x04, true);
                USBDHIDReportWrite (device, &buf, 1, false);

                // DELAY
                for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++)
                {
                }
        }
}

uint32_t usbReceiveEventCallback (void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgParam, void *pvMsgData)
{
//        USB_EVENT_RX_AVAILABLE
//        USBD_HID_EVENT_IDLE_TIMEOUT
//        USBD_HID_EVENT_GET_REPORT_BUFFER
//        USBD_HID_EVENT_GET_REPORT
//        USBD_HID_EVENT_SET_PROTOCOL (for BIOS protocol devices)
//        USBD_HID_EVENT_GET_PROTOCOL (for BIOS protocol devices)
//        USBD_HID_EVENT_SET_REPORT
//        USB_EVENT_CONNECTED
//        USB_EVENT_DISCONNECTED
        return 0;
}

uint32_t usbTransmitEventCallback (void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgParam, void *pvMsgData)
{
//        USB_EVENT_TX_COMPLETE
        return 0;
}
