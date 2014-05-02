/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "hidDescriptors.h"
#include "callbacks.h"

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

uint8_t NUM_STRING_DESCRIPTORS = sizeof(stringDescriptors) / sizeof(uint8_t *);

/**
 * Report descriptor.
 */
const uint8_t reportDescriptor[]=
{
//                0x05, 0x0C, // Usage Page (Consumer Devices)
//                0x09, 0x01, // Usage (Consumer Control)
//                0xA1, 0x01, // Collection (Application)
//                0x05, 0x0C, // Usage Page (Consumer Devices)
//                0x15, 0x00, // Logical Minimum (0)
//                0x25, 0x01, // Logical Maximum (1)
//                0x75, 0x01, // Report Size (1)
//                0x95, 0x07, // Report Count (7)
//                0x09, 0xB5, // Usage (Scan Next Track)
//                0x09, 0xB6, // Usage (Scan Previous Track)
//                0x09, 0xB7, // Usage (Stop)
//                0x09, 0xCD, // Usage (Play / Pause)
//                0x09, 0xE2, // Usage (Mute)
//                0x09, 0xE9, // Usage (Volume Up)
//                0x09, 0xEA, // Usage (Volume Down)
//                0x81, 0x02, // Input (Data, Variable, Absolute)
//                0x95, 0x01, // Report Count (1)
//                0x81, 0x01, // Input (Constant)
//                0xC0 // End Collection

#if 1   // http://www.usblyzer.com/reports/usb-properties/usb-keyboard.html
        UsagePage(0x0C), // Usage Page (Consumer Devices)
        Usage(0x01),     // Usage (Consumer Control)
        Collection(USB_HID_APPLICATION),
                UsagePage(0x0C), // Usage Page (Consumer Devices)
                LogicalMinimum(0),
                LogicalMaximum(1),
                ReportSize(1),
                ReportCount(7),
                Usage (0xB5), // Usage (Scan Next Track)
                Usage (0xB6), // Usage (Scan Previous Track)
                Usage (0xB7), // Usage (Stop)
                Usage (0xcd), // Usage (Play / Pause)
                Usage (0xe2), // Usage (Mute)
                Usage (0xe9), // Usage (Volume Up)
                Usage (0xea), // Usage (Volume Down)
                Input (USB_HID_INPUT_DATA | USB_HID_INPUT_VARIABLE | USB_HID_INPUT_ABS),
                ReportCount(1),
                Input (USB_HID_INPUT_CONSTANT | USB_HID_INPUT_ARRAY | USB_HID_INPUT_ABS),
        EndCollection

#else
        UsagePage(USB_HID_GENERIC_DESKTOP),
        Usage(USB_HID_KEYBOARD),
        Collection(USB_HID_APPLICATION),
                //
                // Modifier keys.
                // 8 - 1 bit values indicating the modifier keys (ctrl, shift...)
                //
                ReportSize(1),
                ReportCount(8),
                UsagePage(USB_HID_USAGE_KEYCODES),
                UsageMinimum(224),
                UsageMaximum(231),
                LogicalMinimum(0),
                LogicalMaximum(1),
                Input(USB_HID_INPUT_DATA | USB_HID_INPUT_VARIABLE | USB_HID_INPUT_ABS),

                //
                // One byte of rsvd data required by HID spec.
                //
                ReportCount(1),
                ReportSize(8),
                Input(USB_HID_INPUT_CONSTANT),

                //
                // Keyboard LEDs.
                // 5 - 1 bit values.
                //
                ReportCount(5),
                ReportSize(1),
                UsagePage(USB_HID_USAGE_LEDS),
                UsageMinimum(1),
                UsageMaximum(5),
                Output(USB_HID_OUTPUT_DATA | USB_HID_OUTPUT_VARIABLE | USB_HID_OUTPUT_ABS),
                //
                // 1 - 3 bit value to pad out to a full byte.
                //
                ReportCount(1),
                ReportSize(3),
                Output(USB_HID_OUTPUT_CONSTANT), //LED report padding

                //
                // The Key buffer.
                // 6 - 8 bit values to store the current key state.
                //
                ReportCount(6),
                ReportSize(8),
                LogicalMinimum(0),
                LogicalMaximum(101),
                UsagePage(USB_HID_USAGE_KEYCODES),
                UsageMinimum (0),
                UsageMaximum (101),
                Input(USB_HID_INPUT_DATA | USB_HID_INPUT_ARRAY),
        EndCollection,
#endif
};

/**
 * The HID class descriptor table. For the mouse class, we have only a single
 * report descriptor.
 */
const uint8_t * const classDescriptors[] =
{
        reportDescriptor
};

/**
 * The HID descriptor for the device.
 */
const tHIDDescriptor hidDescriptor =
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

uint8_t g_pui8KeybDescriptor[] =
{
    //
    // Configuration descriptor header.
    //
    9,                          // Size of the configuration descriptor.
    USB_DTYPE_CONFIGURATION,    // Type of this descriptor.
    USBShort(34),               // The total size of this full structure.
    1,                          // The number of interfaces in this
                                // configuration.
    1,                          // The unique value for this configuration.
    5,                          // The string identifier that describes this
                                // configuration.
    USB_CONF_ATTR_SELF_PWR,     // Bus Powered, Self Powered, remote wake up.
    250,                        // The maximum power in 2mA increments.
};

