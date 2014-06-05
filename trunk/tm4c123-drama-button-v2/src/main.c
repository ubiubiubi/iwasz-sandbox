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

#ifdef DEBUG
void __error__ (char *pcFilename, unsigned long ulLine)
{
        while (1) {

        }
}
#endif

#define VENDOR_ID 0x20a0
#define PRODUCT_ID 0x41ff

/**
 * Size of report on interface 1
 */
#define REPORT0_SIZE 8

/**
 * Size of report on interface 2
 */
#define REPORT1_SIZE 1

/**
 * Remember to set this to max (REPORT1_SIZE, REPORT2_SIZE)
 */
#define REPORT_BUFFER_SIZE REPORT0_SIZE

#define B_REQUEST_SET_ANY_KEY_SETUP 0x00
#define B_REQUEST_GET_ANY_KEY_SETUP 0x01

/**
 * Infromation regaring report to be sent on every keypress. This structure is
 * sent directly from the host-application (on EP0), stored in the EEprom here
 * in the device, and resubmited to the host (on EP1 or EP2) after every button
 * press.
 */
struct ReportConfig {
        /**
         * Which interface chould send the report? This should be the same value
         * as interface index provided in the interface descriptor. We have 2 interfaces
         * here : 0 is the ordinary keyboard (like QWERTY keyboards), and the second
         * interface (index 1) is responsible for "multimedia" buttons.
         */
        uint8_t interface;

        /**
         * Report that would be sent when key is pressed (array of zeros otherwise).
         */
        uint8_t report[REPORT_BUFFER_SIZE];
};

typedef struct ReportConfig ReportConfig;

/**
 * Size od config data sent by the host.
 * http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0067d/Babjddhe.html :
 * "A field with a char type is aligned to the next available byte."
 */
#define ANY_KEY_SETUP_DATA_SIZE sizeof (ReportConfig)

struct CallbackDTO {
        tDeviceInfo *deviceInfo;
        /// Report template.
        ReportConfig reportConfigTemplate;
        /// Actual report.
        ReportConfig report;
        uint8_t iHIDTxState0;
        uint8_t iHIDTxState1;
        uint8_t incoming[ANY_KEY_SETUP_DATA_SIZE];
};

typedef struct CallbackDTO CallbackDTO;

void readEeprom (ReportConfig *reportConfig);
void saveEeprom (ReportConfig const *reportConfig);

/*##########################################################################*/

static const uint8_t deviceDescriptor[] =
{
    18,                         // Size of this structure.
    USB_DTYPE_DEVICE,           // Type of this structure.
    USBShort(0x110),            // USB version 1.1 (if we say 2.0, hosts assume
                                // high-speed - see USB 2.0 spec 9.2.6.6)
    USB_CLASS_DEVICE,           // USB Device Class
    0,                          // USB Device Sub-class
    USB_HID_PROTOCOL_NONE,      // USB Device protocol
    USBDHID_MAX_PACKET,         // Maximum packet size for default pipe.
    USBShort(VENDOR_ID),        // Vendor ID (VID).
    USBShort(PRODUCT_ID),       // Product ID (PID).
    USBShort(0x100),            // Device Version BCD.
    1,                          // Manufacturer string identifier.
    2,                          // Product string identifier.
    3,                          // Product serial number.
    1                           // Number of configurations.
};

//*****************************************************************************
//
// HID device configuration descriptor.
//
// It is vital that the configuration descriptor bConfigurationValue field
// (byte 6) is 1 for the first configuration and increments by 1 for each
// additional configuration defined here.  This relationship is assumed in the
// device stack for simplicity even though the USB 2.0 specification imposes
// no such restriction on the bConfigurationValue values.
//
// Note that this structure is deliberately located in RAM since we need to
// be able to patch some values in it based on client requirements.
//
//*****************************************************************************
static uint8_t configurationDescriptor[] =
{
    //
    // Configuration descriptor header.
    //
    9,                          // Size of the configuration descriptor.
    USB_DTYPE_CONFIGURATION,    // Type of this descriptor.
    USBShort(34),               // The total size of this full structure.
    2,                          // The number of interfaces in this configuration.
    1,                          // The unique value for this configuration.
    5,                          // The string identifier that describes this configuration.
    USB_CONF_ATTR_BUS_PWR,     // Bus Powered, Self Powered, remote wake up.
    125,                        // The maximum power in 2mA increments.
};

/**
 * Report descriptor. Standard BOOT protocol keyboard
 */
const uint8_t reportDescriptor1[]=
{
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
};

//*****************************************************************************
//
// The remainder of the configuration descriptor is stored in flash since we
// don't need to modify anything in it at runtime.
//
//*****************************************************************************
static const uint8_t interfaceDescriptor1[] =
{
        //
        // HID Device Class Interface Descriptor.
        //
        9,                          // Size of the interface descriptor.
        USB_DTYPE_INTERFACE,        // Type of this descriptor.
        0,                          // The index for this interface.
        0,                          // The alternate setting for this interface.
        1,                          // The number of endpoints used by this
                                    // interface.
        USB_CLASS_HID,              // The interface class
        USB_HID_SCLASS_BOOT,        // The interface sub-class.
        USB_HID_PROTOCOL_KEYB,      // The interface protocol for the sub-class
                                    // specified above.
        4                           // The string index for this interface.
};

static const uint8_t hidKeyboardDescriptor1[] =
{
        // HID Descriptor
        9, // bLength
        USB_HID_DTYPE_HID, // bDescriptorType
        USBShort(0x111), // bcdHID (version 1.11 compliant)
        0, // bCountryCode (not localized)
        1, // bNumDescriptors
        USB_HID_DTYPE_REPORT,
        // Report descriptor
        USBShort (sizeof (reportDescriptor1)) // Size of report descriptor
};

static const uint8_t inEndpointDescriptor1[HIDINENDPOINT_SIZE] =
{
    7,                          // The size of the endpoint descriptor.
    USB_DTYPE_ENDPOINT,         // Descriptor type is an endpoint.
    USB_EP_DESC_IN | USBEPToIndex(USB_EP_1),
    USB_EP_ATTR_INT,            // Endpoint is an interrupt endpoint.
    USBShort(USBFIFOSizeToBytes(USB_FIFO_SZ_64)),
                                // The maximum packet size.
    16,                         // The polling interval for this endpoint.
};


/**
 * Report descriptor. Keyboard with "multimedia" buttons.
 */
const uint8_t reportDescriptor2[]=
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

        // http://www.usblyzer.com/reports/usb-properties/usb-keyboard.html
        UsagePage(0x0C), // Usage Page (Consumer Devices)
        Usage(0x01),     // Usage (Consumer Control)
        Collection(USB_HID_APPLICATION),
                UsagePage(0x0C), // Usage Page (Consumer Devices)
                LogicalMinimum(0),
                LogicalMaximum(1),
                ReportSize(1),
                ReportCount(7),
                Usage (0x30), // Usage (Power)
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
};

/**
 *
 */
static const uint8_t interfaceDescriptor2[] =
{
    //
    // HID Device Class Interface Descriptor.
    //
    9,                          // Size of the interface descriptor.
    USB_DTYPE_INTERFACE,        // Type of this descriptor.
    1,                          // The index for this interface.
    0,                          // The alternate setting for this interface.
    1,                          // The number of endpoints used by this
                                // interface.
    USB_CLASS_HID,              // The interface class
    USB_HID_SCLASS_NONE,        // The interface sub-class.
    USB_HID_PROTOCOL_NONE,      // The interface protocol for the sub-class
                                // specified above.
    4,                          // The string index for this interface.
};

static const uint8_t hidKeyboardDescriptor2[] =
{
        // HID Descriptor
        9, // bLength
        USB_HID_DTYPE_HID, // bDescriptorType
        USBShort(0x111), // bcdHID (version 1.11 compliant)
        0, // bCountryCode (not localized)
        1, // bNumDescriptors
        USB_HID_DTYPE_REPORT,
        // Report descriptor
        USBShort (sizeof (reportDescriptor2)) // Size of report descriptor
};

static const uint8_t inEndpointDescriptor2[HIDINENDPOINT_SIZE] =
{
    7,                          // The size of the endpoint descriptor.
    USB_DTYPE_ENDPOINT,         // Descriptor type is an endpoint.
    USB_EP_DESC_IN | USBEPToIndex(USB_EP_2),
    USB_EP_ATTR_INT,            // Endpoint is an interrupt endpoint.
    USBShort(USBFIFOSizeToBytes(USB_FIFO_SZ_64)),
                                // The maximum packet size.
    16,                         // The polling interval for this endpoint.
};


/*##########################################################################*/

static const tConfigSection configSection =
{
    sizeof(configurationDescriptor),
    configurationDescriptor
};

static const tConfigSection interfaceSection1 =
{
    sizeof(interfaceDescriptor1),
    interfaceDescriptor1
};

static const tConfigSection hidKeyboardSection1 =
{
    sizeof(hidKeyboardDescriptor1),
    hidKeyboardDescriptor1
};

static const tConfigSection endpointSection1 =
{
    sizeof(inEndpointDescriptor1),
    inEndpointDescriptor1
};

static const tConfigSection interfaceSection2 =
{
    sizeof(interfaceDescriptor2),
    interfaceDescriptor2
};

static const tConfigSection hideKeyboardSection2 =
{
    sizeof(hidKeyboardDescriptor2),
    hidKeyboardDescriptor2
};

static const tConfigSection endpointSection2 =
{
    sizeof(inEndpointDescriptor2),
    inEndpointDescriptor2
};

//*****************************************************************************
//
// This array lists all the sections that must be concatenated to make a
// single, complete HID keyboard configuration descriptor.
//
//*****************************************************************************
const tConfigSection *allConfigSections[] = {
        &configSection,
        &interfaceSection1,
        &hidKeyboardSection1,
        &endpointSection1,
        &interfaceSection2,
        &hideKeyboardSection2,
        &endpointSection2
};

#define NUM_KEYBOARD_SECTIONS (sizeof(allConfigSections) / sizeof(tConfigSection *))

const static tConfigHeader configHeader = {
        NUM_KEYBOARD_SECTIONS,
        allConfigSections
};

const static tConfigHeader *configHeaders[] = { &configHeader };

/****************************************************************************/

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
const uint8_t g_pui8HIDInterfaceString1[] =
{
    (23 + 1) * 2,
    USB_DTYPE_STRING,
    'H', 0, 'I', 0, 'D', 0, ' ', 0, 'K', 0, 'e', 0, 'y', 0, 'b', 0,
    'o', 0, 'a', 0, 'r', 0, 'd', 0, ' ', 0, 'I', 0, 'n', 0, 't', 0, 'e', 0,
    'r', 0, 'f', 0, 'a', 0, 'c', 0, 'e', 0, '1', 0
};

/**
 * The interface description string.
 */
const uint8_t g_pui8HIDInterfaceString2[] =
{
    (23 + 1) * 2,
    USB_DTYPE_STRING,
    'H', 0, 'I', 0, 'D', 0, ' ', 0, 'K', 0, 'e', 0, 'y', 0, 'b', 0,
    'o', 0, 'a', 0, 'r', 0, 'd', 0, ' ', 0, 'I', 0, 'n', 0, 't', 0, 'e', 0,
    'r', 0, 'f', 0, 'a', 0, 'c', 0, 'e', 0, '1', 0
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
 * The descriptor string table. The various string identifiers
 * passed in other descriptors are indexes into the pStringDescriptor array. The first entry of the string
 * descriptor array has a special format and indicates the languages supported by the device.
 */
const uint8_t * const stringDescriptors[] =
{
    g_pui8LangDescriptor,
    g_pui8ManufacturerString,
    g_pui8ProductString,
    g_pui8SerialNumberString,
    g_pui8HIDInterfaceString1,
//    g_pui8HIDInterfaceString2,
    g_pui8ConfigString
};

// -1 because first element is g_pui8LangDescriptor which is not a string.
uint8_t NUM_STRING_DESCRIPTORS = sizeof(stringDescriptors) / sizeof(uint8_t *);

/*##########################################################################*/

static void onGetDescriptor(void *userData, tUSBRequest *psUSBRequest);
static void onRequest(void *userData, tUSBRequest *psUSBRequest);
static void onEndpointsActivity (void *pvHIDInstance, uint32_t ui32Status);
static void onConfigChange (void *pvHIDInstance, uint32_t ui32Info);
static void onDataReceivedEP0 (void *userData, uint32_t ui32Info);

static int32_t scheduleReportTransmission (uint8_t *data, uint16_t len, uint32_t endpoint);

//*****************************************************************************
//
// The device information structure for the USB HID devices.
//
//*****************************************************************************
const tCustomHandlers handlers =
{
    //
    // GetDescriptor
    //
    onGetDescriptor,

    //
    // RequestHandler
    //
    onRequest,

    //
    // InterfaceChange
    //
    0,

    //
    // ConfigChange
    //
    onConfigChange,

    //
    // DataReceived
    //
    onDataReceivedEP0,

    //
    // DataSentCallback
    //
    0/*onEP0DataSent*/,

    //
    // ResetHandler
    //
    0/*onReset*/,

    //
    // SuspendHandler
    //
    0/*onSuspend*/,

    //
    // ResumeHandler
    //
    0/*onResume*/,

    //
    // DisconnectHandler
    //
    0/*onDisconnect*/,

    //
    // EndpointHandler
    //
    onEndpointsActivity,

    //
    // Device handler.
    //
    0/*onDevice*/
};

//*****************************************************************************
//
// This function is called by the USB device stack whenever a request for a
// non-standard descriptor is received.
//
// \param pvHIDInstance is the instance data for this request.
// \param psUSBRequest points to the request received.
//
// This call parses the provided request structure and determines which
// descriptor is being requested.  Assuming the descriptor can be found, it is
// scheduled for transmission via endpoint zero.  If the descriptor cannot be
// found, the endpoint is stalled to indicate an error to the host.
//
//*****************************************************************************
static void onGetDescriptor(void *userData, tUSBRequest *psUSBRequest)
{
    uint32_t ui32Size;
    ASSERT(userData);

    //
    // Which type of class descriptor are we being asked for?
    //
    switch(psUSBRequest->wValue >> 8)
    {
        //
        // This is a request for a HID report or physical descriptor.
        //
        case USB_HID_DTYPE_REPORT:
        {
                if (psUSBRequest->wIndex == 0) {
                        ui32Size = sizeof(reportDescriptor1);

                        // If there is more data to send than the host requested then just send the requested amount of data.
                        if (ui32Size > psUSBRequest->wLength) {
                                ui32Size = psUSBRequest->wLength;
                        }

                        // Send the data via endpoint 0.
                        USBDCDSendDataEP0 (0, (uint8_t *)reportDescriptor1, sizeof(reportDescriptor1));
                }
                else if (psUSBRequest->wIndex == 1) {
                        ui32Size = sizeof(reportDescriptor2);

                        // If there is more data to send than the host requested then just send the requested amount of data.
                        if (ui32Size > psUSBRequest->wLength) {
                                ui32Size = psUSBRequest->wLength;
                        }

                        // Send the data via endpoint 0.
                        USBDCDSendDataEP0 (0, (uint8_t *)reportDescriptor2, sizeof(reportDescriptor2));
                }
            break;
        }

        //
        // This is a request for the HID descriptor (as found in the
        // configuration descriptor following the relevant interface).
        //
        case USB_HID_DTYPE_HID:
        {

                if (psUSBRequest->wIndex == 0) {
                        ui32Size = sizeof(hidKeyboardDescriptor1);

                        if (ui32Size > psUSBRequest->wLength) {
                                ui32Size = psUSBRequest->wLength;
                        }
                        USBDCDSendDataEP0 (0, (uint8_t *)hidKeyboardDescriptor1, ui32Size);
                }
                else if (psUSBRequest->wIndex == 1) {
                        ui32Size = sizeof(hidKeyboardDescriptor2);

                        if (ui32Size > psUSBRequest->wLength) {
                                ui32Size = psUSBRequest->wLength;
                        }
                        USBDCDSendDataEP0 (0, (uint8_t *)hidKeyboardDescriptor2, ui32Size);
                }
            break;
        }

        //
        // This was an unknown request so stall.
        //
        default:
        {
            USBDCDStallEP0(0);
            break;
        }
    }
}

//*****************************************************************************
//
// This function is called by the USB device stack whenever a non-standard
// request is received.
//
// \param pvHIDInstance is the instance data for this HID device.
// \param psUSBRequest points to the request received.
//
// This call parses the provided request structure.  Assuming the request is
// understood, it is handled and any required response generated.  If the
// request cannot be handled by this device class, endpoint zero is stalled to
// indicate an error to the host.
//
//*****************************************************************************
static void onRequest(void *userData, tUSBRequest *psUSBRequest)
{
        printf ("onRequest bmRequestType:%x, bRequest:%x, wValue:%x, wIndex:%x, wLength:%x\r\n",
                psUSBRequest->bmRequestType,
                psUSBRequest->bRequest,
                psUSBRequest->wValue,
                psUSBRequest->wIndex,
                psUSBRequest->wLength);

        CallbackDTO *callbackDTO = (CallbackDTO *) userData;
        ASSERT(callbackDTO);

        if (psUSBRequest->bmRequestType & USB_RTYPE_VENDOR &&
            psUSBRequest->bmRequestType & USB_RTYPE_INTERFACE) {
                if (psUSBRequest->bRequest == B_REQUEST_SET_ANY_KEY_SETUP) {
                        // This first parameter is asserted to always be 0. This is a bug in the TI's usblib i think.
                        USBDCDRequestDataEP0 (0, callbackDTO->incoming, ANY_KEY_SETUP_DATA_SIZE);
                        USBDevEndpointDataAck(USB0_BASE, USB_EP_0, false);
                }
                else if (psUSBRequest->bRequest == B_REQUEST_GET_ANY_KEY_SETUP) {
                        printf ("Sending the configuration back to the host\r\n");
                        USBDevEndpointDataAck(USB0_BASE, USB_EP_0, true);
                        USBDCDSendDataEP0 (0, (uint8_t *)&callbackDTO->reportConfigTemplate, sizeof (ReportConfig));
                }
                return;
        }

        switch (psUSBRequest->bRequest) {
        /*
         * A Get Report request is used by the host to poll a device for its
         * current state. The only required request for non boot-device.
         */
        case USBREQ_GET_REPORT: {
                // Need to ACK the data on end point 0 in this case.
                USBDevEndpointDataAck(USB0_BASE, USB_EP_0, true);

                if (callbackDTO->report.interface == 0) {
                        USBDCDSendDataEP0 (0, callbackDTO->report.report, REPORT0_SIZE);
                }
                else if (callbackDTO->report.interface == 1) {
                        USBDCDSendDataEP0 (0, callbackDTO->report.report, REPORT1_SIZE);
                }

                break;
        }

        // This request was not recognized so stall.
        default: {
                USBDCDStallEP0(0);
                break;
        }
        }
}

/**
 * Gets called when data from host to the device has arrived after USBDCDRequestDataEP0
 * has been called.
 */
static void onDataReceivedEP0 (void *userData, uint32_t ui32Info)
{

        CallbackDTO *callbackDTO = (CallbackDTO *) userData;
        ASSERT(callbackDTO);

#if 1
        printf ("onDataReceivedEP0 len (%u) [", (unsigned int)ui32Info);

        for (int i = 0; i < ANY_KEY_SETUP_DATA_SIZE; ++i) {
                printf ("%d, ", callbackDTO->incoming[i]);
        }

        printf ("]\r\n");
#endif

        ReportConfig *rc = &callbackDTO->reportConfigTemplate;
        rc->interface = callbackDTO->incoming[0];
        memcpy (rc->report, callbackDTO->incoming + 1, sizeof (ReportConfig) - 1);
        saveEeprom (rc);
        USBDevEndpointDataAck(USB0_BASE, USB_EP_0, true);
}

//*****************************************************************************
//
// Called by the USB stack for any activity involving one of our endpoints
// other than EP0.  This function is a fan out that merely directs the call to
// the correct handler depending upon the endpoint and transaction direction
// signaled in ui32Status.
//
//*****************************************************************************
static void onEndpointsActivity (void *userData, uint32_t ui32Status)
{
        printf ("onEndpointsActivity ui32Status : %x\r\n", (unsigned int)ui32Status);

        CallbackDTO *callbackDTO = (CallbackDTO *) userData;
        ASSERT(callbackDTO);

        if (ui32Status & 1 << USBEPToIndex (USB_EP_1)) {
                uint32_t ui32EPStatus = USBEndpointStatus (USB0_BASE, USB_EP_1);
                USBDevEndpointStatusClear(USB0_BASE, USB_EP_1, ui32EPStatus);
                callbackDTO->iHIDTxState0 = eHIDStateIdle;
        }

        if (ui32Status & 1 << USBEPToIndex (USB_EP_2)) {
                uint32_t ui32EPStatus = USBEndpointStatus (USB0_BASE, USB_EP_2);
                USBDevEndpointStatusClear(USB0_BASE, USB_EP_2, ui32EPStatus);
                callbackDTO->iHIDTxState1 = eHIDStateIdle;
        }
}

/**
 *
 */
static void onConfigChange (void *userData, uint32_t ui32Info)
{
        printf ("onConfigChange\r\n");

        CallbackDTO *callbackDTO = (CallbackDTO *) userData;
        ASSERT (callbackDTO);

        callbackDTO->iHIDTxState0 = eHIDStateIdle;
        callbackDTO->iHIDTxState1 = eHIDStateIdle;
}

//*****************************************************************************
//
// Schedule transmission of the next packet forming part of an input report.
//
// \param psHIDInst points to the HID device instance whose input report is to
// be sent.
//
// This function is called to transmit the next packet of an input report
// passed to the driver via a call to USBDHIDReportWrite.  If any data remains
// to be sent, a USB packet is written to the FIFO and scheduled for
// transmission to the host.  The function ensures that reports are sent as
// a sequence of full packets followed by either a single int16_t packet or a
// packet with no data to indicate the end of the transaction.
//
//*****************************************************************************
static int32_t scheduleReportTransmission (uint8_t *data, uint16_t len, uint32_t endpoint)
{
        int32_t i32Retcode;

        // Put the data in the correct FIFO.
        i32Retcode = USBEndpointDataPut(USB0_BASE, endpoint, data, len);

        if (i32Retcode != -1) {
                // Send out the current data.
                i32Retcode = USBEndpointDataSend(USB0_BASE, endpoint, USB_TRANS_IN);
        }

        //
        // Tell the caller how we got on.
        //
        return (i32Retcode);
}

/**
 *
 */
uint32_t reportWrite (CallbackDTO *callbackDTO, uint8_t *data, uint16_t length, uint32_t endpoint)
{
        ASSERT(callbackDTO);
        printf ("reportWrite dataLen : %d, \r\n", length);

        //
        // Can we send the data provided?
        //
        if ((endpoint == USB_EP_1 && callbackDTO->iHIDTxState0 != eHIDStateIdle) ||
            (endpoint == USB_EP_2 && callbackDTO->iHIDTxState1 != eHIDStateIdle)) {
                // We are in the middle of sending another report.  Return 0 to
                // indicate that we can't send this report until the previous one
                // finishes.
                return (0);
        }

        // Schedule transmission of the first packet of the report.
        if (endpoint == USB_EP_1) {
                callbackDTO->iHIDTxState0 = eHIDStateWaitData;
        }
        else if (endpoint == USB_EP_2) {
                callbackDTO->iHIDTxState1 = eHIDStateWaitData;
        }

        uint32_t i32Retcode = scheduleReportTransmission(data, length, endpoint);

        //
        // Did an error occur while trying to send the data?
        //
        if (i32Retcode != -1) {
                // No - tell the caller we sent all the bytes provided.
                return (length);
        } else {
                // Yes - tell the caller we could not send the data.
                return (0);
        }
}

/*##########################################################################*/

static tDeviceInfo deviceInfo;

void uartStdioConfig (uint32_t ui32PortNum, uint32_t ui32Baud, uint32_t ui32SrcClock)
{
        //
        // Check the arguments.
        //
        ASSERT((ui32PortNum == 0) || (ui32PortNum == 1) || (ui32PortNum == 2));

        //
        // Check to make sure the UART peripheral is present.
        //
        if (!SysCtlPeripheralPresent(SYSCTL_PERIPH_UART0)) {
                return;
        }

        //
        // Enable the UART peripheral for use.
        //
        SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

        //
        // Configure the UART for 115200, n, 8, 1
        //
        UARTConfigSetExpClk(UART0_BASE, ui32SrcClock, ui32Baud, (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE |
        UART_CONFIG_WLEN_8));

        //
        // Enable the UART operation.
        //
        UARTEnable(UART0_BASE);
}

void configureUART (void)
{
        //
        // Enable the GPIO Peripheral used by the UART.
        //
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

        //
        // Enable UART0
        //
        SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

        //
        // Configure GPIO Pins for UART mode.
        //
        GPIOPinConfigure(GPIO_PA0_U0RX);
        GPIOPinConfigure(GPIO_PA1_U0TX);
        GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

        //
        // Use the internal 16MHz oscillator as the UART clock source.
        //
        UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

        //
        // Initialize the UART for console I/O.
        //
        uartStdioConfig(0, 115200, 16000000);
}

/*##########################################################################*/

void initEeprom (void)
{
        SysCtlPeripheralEnable (SYSCTL_PERIPH_EEPROM0);
        uint32_t ret = EEPROMInit ();

        if (ret != EEPROM_INIT_OK) {
#ifdef DEBUG
                printf ("initEeprom failed. Error code %u which translates to : ", (unsigned int)ret);

                switch (ret) {
                case EEPROM_INIT_RETRY:
                        printf ("EEPROM_INIT_RETRY\r\n");
                        break;

                case EEPROM_INIT_ERROR:
                        printf ("EEPROM_INIT_ERROR\r\n");
                        break;
                }
#endif
                return;
        }

        printf ("EEprom init OK. Size : %u B\r\n", (unsigned int)EEPROMSizeGet());
}

/**
 *
 */
void readEeprom (ReportConfig *reportConfig)
{
        size_t rcSize = sizeof (ReportConfig);
        uint8_t buffer[rcSize];
        EEPROMRead ((uint32_t *)buffer, 0, rcSize);
        reportConfig->interface = buffer[0];
        memcpy (reportConfig->report, buffer + 1, rcSize - 1);
}

/**
 *
 */
void saveEeprom (ReportConfig const *reportConfig)
{
        printf ("Saving config to EEPROM.");
        uint32_t ret = EEPROMProgram ((uint32_t *)reportConfig, 0, sizeof (ReportConfig));
        ASSERT (ret == 0);
}

/*##########################################################################*/



/**
 *
 */
int main (void)
{
        SysCtlClockSet (SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);

        configureUART ();
        printf ("Any key device starts. tm4c123 micro.\r\n");

        // Enable the GPIO port that is used for the on-board LED.
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);


        // USB config : Enable the GPIO peripheral used for USB, and configure the USB pins.
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
        SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOD);
        GPIOPinTypeUSBAnalog(GPIO_PORTD_AHB_BASE, GPIO_PIN_4 | GPIO_PIN_5);

        ButtonsInit();
        initEeprom ();

        /****************************************************************************/

        CallbackDTO callbackDTO;
        callbackDTO.deviceInfo = &deviceInfo;
        callbackDTO.iHIDTxState0 = eHIDStateUnconfigured;
        callbackDTO.iHIDTxState1 = eHIDStateUnconfigured;

        readEeprom (&callbackDTO.reportConfigTemplate);

        deviceInfo.psCallbacks = &handlers;
        deviceInfo.pui8DeviceDescriptor = deviceDescriptor;
        deviceInfo.ppsConfigDescriptors = configHeaders;
        deviceInfo.ppui8StringDescriptors = stringDescriptors;
        deviceInfo.ui32NumStringDescriptors = NUM_STRING_DESCRIPTORS;
        USBDCDDeviceInfoInit (0, &deviceInfo);

        USBStackModeSet(0, eUSBModeForceDevice, 0);
        USBDCDInit (0, &deviceInfo, &callbackDTO);

        uint8_t ui8ButtonsChanged, ui8Buttons;

        while (1) {
                ButtonsPoll(&ui8ButtonsChanged, &ui8Buttons);

                if (ui8ButtonsChanged) {
                        if(ui8Buttons & LEFT_BUTTON)
                        {
                                printf ("pressed\r\n");
                                memcpy (callbackDTO.report.report, callbackDTO.reportConfigTemplate.report, REPORT_BUFFER_SIZE);
                        }
                        else {
                                printf ("released\r\n");
                                memset (callbackDTO.report.report, '\0', REPORT_BUFFER_SIZE);
                        }

                        callbackDTO.report.interface = callbackDTO.reportConfigTemplate.interface;

                        if (callbackDTO.report.interface == 0) {
                                reportWrite (&callbackDTO, callbackDTO.report.report, REPORT0_SIZE, USB_EP_1);
                        }
                        else if (callbackDTO.report.interface == 1) {
                                reportWrite (&callbackDTO, callbackDTO.report.report, REPORT1_SIZE, USB_EP_2);
                        }
                }
        }
}

