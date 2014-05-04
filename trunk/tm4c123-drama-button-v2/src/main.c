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
#include "hidDescriptors.h"
#include "deviceDescriptors.h"
#include "driverlib/debug.h"
#include "buttons.h"
#include "rgb.h"
#include "callbacks.h"

/*
 * https://www.microsoft.com/china/whdc/archive/w2kbd.mspx - here are instructions for HID keyboard design with additional miltimedia capabilities.
 * Microsoft reccomends that there should be a tleast two USB interfaces in suach a device. One should implement a oridinary keyboard compatible
 * with BOOT interface, so such keyboard would be operational during system startup, when there is no OS support yet, and another one would implement
 * the rest of desired keys such as play/pause, volume up, down and so on. I saw quite a few USB interface layouts onforming to this recommendations
 * over the net, including my own keyboard connected to my computer as I write this, so I assume this is the right way to do this.
 *
 * http://www.usblyzer.com/reports/usb-properties/usb-keyboard.html - this is an example of USB interface layout as mentioned earlier. HID reports are
 * also provided.
 */

#ifdef DEBUG
void __error__ (char *pcFilename, unsigned long ulLine)
{
        while (1) {

        }
}
#endif

#define VENDOR_ID 0x20a0
#define PRODUCT_ID 0x41ff
#define REPORT1_SIZE 8
#define REPORT2_SIZE 1

struct CallbackDTO {
        tDeviceInfo *deviceInfo;
        uint8_t report1[REPORT1_SIZE];
        uint8_t report2[REPORT2_SIZE];
};

typedef struct CallbackDTO CallbackDTO;

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
    6,                          // The string identifier that describes this configuration.
    USB_CONF_ATTR_SELF_PWR,     // Bus Powered, Self Powered, remote wake up.
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

static const uint8_t hideKeyboardDescriptor1[] =
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
    0,                          // The index for this interface.
    0,                          // The alternate setting for this interface.
    1,                          // The number of endpoints used by this
                                // interface.
    USB_CLASS_HID,              // The interface class
    USB_HID_SCLASS_NONE,        // The interface sub-class.
    USB_HID_PROTOCOL_NONE,      // The interface protocol for the sub-class
                                // specified above.
    5,                          // The string index for this interface.
};

static const uint8_t hideKeyboardDescriptor2[] =
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

static const tConfigSection hideKeyboardSection1 =
{
    sizeof(hideKeyboardDescriptor1),
    hideKeyboardDescriptor1
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
    sizeof(hideKeyboardDescriptor2),
    hideKeyboardDescriptor2
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
        &hideKeyboardSection1,
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
    g_pui8HIDInterfaceString2,
    g_pui8ConfigString
};

uint8_t NUM_STRING_DESCRIPTORS = sizeof(stringDescriptors) / sizeof(uint8_t *);

/*##########################################################################*/

static void onGetDescriptor(void *userData, tUSBRequest *psUSBRequest);
static void onRequest(void *userData, tUSBRequest *psUSBRequest);
//static void onConfigChange(void *pvHIDInstance, uint32_t ui32Info);
//static void onEP0DataReceived(void *pvHIDInstance, uint32_t ui32Info);
//static void onEP0DataSent(void *pvHIDInstance, uint32_t ui32Info);
//static void onReset(void *pvHIDInstance);
//static void onSuspend(void *pvHIDInstance);
//static void onResume(void *pvHIDInstance);
//static void onDisconnect(void *pvHIDInstance);
//static void onEndpoints(void *pvHIDInstance, uint32_t ui32Status);
//static void onDevice(void *pvHIDInstance, uint32_t ui32Request, void *pvRequestData);

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
    0/*onConfigChange*/,

    //
    // DataReceived
    //
    0/*onEP0DataReceived*/,

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
    0/*onEndpoints*/,

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

    // TODO here we have the interface number that the host is interested in. Probably.
    // psUSBRequest->wIndex

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
            //
            // Find the index to the descriptor that is being queried.
            //
            ui32Size = sizeof (reportDescriptor1);

            //
            // If there is more data to send than the host requested then just
            // send the requested amount of data.
            //
            if(ui32Size > psUSBRequest->wLength)
            {
                ui32Size = psUSBRequest->wLength;
            }

            //
            // Send the data via endpoint 0.
            //
            USBDCDSendDataEP0(0, reportDescriptor1, sizeof (reportDescriptor1));
            break;
        }

        //
        // This is a request for the HID descriptor (as found in the
        // configuration descriptor following the relevant interface).
        //
        case USB_HID_DTYPE_HID:
        {
            //
            // How big is the HID descriptor?
            //
            ui32Size = sizeof (hideKeyboardDescriptor1);

            //
            // If there is more data to send than the host requested then just
            // send the requested amount of data.
            //
            if(ui32Size > psUSBRequest->wLength)
            {
                ui32Size = psUSBRequest->wLength;
            }

            //
            // Send the data via endpoint 0.
            //
            USBDCDSendDataEP0(0, hideKeyboardDescriptor1, ui32Size);
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
    tHIDInstance *psInst;
    uint8_t ui8Protocol;
    uint32_t ui32Size, ui32Timeout;
    uint8_t *pui8Report;

    CallbackDTO *callbackDTO = (CallbackDTO *) userData;

    ASSERT(callbackDTO);

    //
    // Make sure the request was for this interface.
    //
    if(psUSBRequest->wIndex != psInst->ui8Interface)
    {
        return;
    }

    //
    // Determine the type of request.
    //
    switch(psUSBRequest->bRequest)
    {
        //
        // A Get Report request is used by the host to poll a device for its
        // current state.
        //
        case USBREQ_GET_REPORT:
        {
            //
            // Need to ACK the data on end point 0 in this case.
            //
            USBDevEndpointDataAck(psInst->ui32USBBase, USB_EP_0, true);

            //
            // ..then send back the requested report.
            //

//            TODO
//            psInst->bGetRequestPending = true;

            USBDCDSendDataEP0 (0, callbackDTO->report1, REPORT1_SIZE);
            break;
        }

        //
        // A set IDLE request has been made.  This indicates to us how often a
        // given report should be sent back to the host in the absence of any
        // change in state of the device.
        //
//        case USBREQ_SET_IDLE:
//        {
//            //
//            // Set the idle timeout for the requested report(s).
//            //
//            SetIdleTimeout(psHIDDevice, psUSBRequest->wValue & 0xFF, (psUSBRequest->wValue >> 8) & 0xFF);
//
//            //
//            // Need to ACK the data on end point 0 in this case.
//            //
//            USBDevEndpointDataAck(psInst->ui32USBBase, USB_EP_0, true);
//            break;
//        }

        //
        // A get IDLE request has been made.  This request queries the current
        // idle timeout for a given report.
        //
//        case USBREQ_GET_IDLE:
//        {
//            //
//            // Determine the timeout for the requested report.
//            //
//            ui32Timeout = GetIdleTimeout(psHIDDevice, psUSBRequest->wValue);
//
//            if(ui32Timeout != HID_NOT_FOUND)
//            {
//                //
//                // Need to ACK the data on end point 0 in this case.
//                //
//                USBDevEndpointDataAck(psInst->ui32USBBase, USB_EP_0, true);
//
//                //
//                // Send our response to the host.
//                //
//                USBDCDSendDataEP0(0, (uint8_t *)&ui32Timeout, 1);
//            }
//            else
//            {
//                //
//                // The report ID was not found so stall the endpoint.
//                //
//                USBDCDStallEP0(0);
//            }
//            break;
//        }

        //
        // Set either boot or report protocol for reports sent from the device.
        // This is only supported by devices in the boot subclass.
        //
//        case USBREQ_SET_PROTOCOL:
//        {
//            if(psHIDDevice->ui8Subclass == USB_HID_SCLASS_BOOT)
//            {
//                //
//                // We need to ACK the data on end point 0 in this case.
//                //
//                USBDevEndpointDataAck(psInst->ui32USBBase, USB_EP_0, true);
//
//                //
//                // We are a boot subclass device so pass this on to the
//                // application.
//                //
//                psHIDDevice->pfnRxCallback(psHIDDevice->pvRxCBData,
//                                           USBD_HID_EVENT_SET_PROTOCOL,
//                                           psUSBRequest->wValue,
//                                           (void *)0);
//            }
//            else
//            {
//                //
//                // This is not a boot subclass device so stall the endpoint to
//                // show that we don't support this request.
//                //
//                USBDCDStallEP0(0);
//            }
//            break;
//        }

        //
        // Inform the host of the protocol, boot or report, that is currently
        // in use.  This is only supported by devices in the boot subclass.
        //
//        case USBREQ_GET_PROTOCOL:
//        {
//            if(psHIDDevice->ui8Subclass == USB_HID_SCLASS_BOOT)
//            {
//                //
//                // We need to ACK the data on end point 0 in this case.
//                //
//                USBDevEndpointDataAck(psInst->ui32USBBase, USB_EP_0, true);
//
//                //
//                // We are a boot subclass device so pass this on to the
//                // application callback to get the answer.
//                //
//                ui8Protocol = (uint8_t)psHIDDevice->pfnRxCallback(
//                    psHIDDevice->pvRxCBData, USBD_HID_EVENT_GET_PROTOCOL, 0,
//                    (void *)0);
//
//                //
//                // Send our response to the host.
//                //
//                USBDCDSendDataEP0(0, (uint8_t *)&ui8Protocol, 1);
//            }
//            else
//            {
//                //
//                // This is not a boot subclass device so stall the endpoint to
//                // show that we don't support this request.
//                //
//                USBDCDStallEP0(0);
//            }
//            break;
//        }

        //
        // This request was not recognized so stall.
        //
        default:
        {
            USBDCDStallEP0(0);
            break;
        }
    }
}

/*##########################################################################*/



static tDeviceInfo deviceInfo;







///**
// * My initialization routine.
// */
//void *lowLevelHIDInit (uint32_t ui32Index, tUSBDHIDDevice *psHIDDevice, CallbackDTO *callbackDTO)
//{
//        tConfigDescriptor * pConfigDesc;
//
//        uint32_t ui32Loop;
//
//        // Initialize the various fields in our instance structure.
//        callbackDTO->ui8USBConfigured = 0;
//        callbackDTO->ui8Protocol = USB_HID_PROTOCOL_REPORT;
//        callbackDTO->sReportIdle.ui8Duration4mS = 125;
//        callbackDTO->sReportIdle.ui8ReportID = 0;
//        callbackDTO->sReportIdle.ui32TimeSinceReportmS = 0;
//        callbackDTO->sReportIdle.ui16TimeTillNextmS = 0;
//        callbackDTO->ui8LEDStates = 0;
//        callbackDTO->ui8KeyCount = 0;
//        callbackDTO->eKeyboardState = HID_KEYBOARD_STATE_UNCONFIGURED;
//        callbackDTO->psHIDDevice = psHIDDevice;
//
//        for(ui32Loop = 0; ui32Loop < KEYB_MAX_CHARS_PER_REPORT; ui32Loop++)
//        {
//                callbackDTO->pui8KeysPressed[ui32Loop] = HID_KEYB_USAGE_RESERVED;
//        }
//
//
//        psHIDDevice->ppsConfigDescriptor = g_ppsHIDConfigDescriptors;
//
//        void *pvRetcode = USBDHIDCompositeInit (ui32Index, psHIDDevice, 0);
//
//        pConfigDesc = (tConfigDescriptor *) g_pui8KeybDescriptor;
//        pConfigDesc->bmAttributes = psHIDDevice->ui8PwrAttributes;
//        pConfigDesc->bMaxPower = (uint8_t) (psHIDDevice->ui16MaxPowermA / 2);
//
//        //
//        // If we initialized the HID layer successfully, pass our device pointer
//        // back as the return code, otherwise return NULL to indicate an error.
//        //
//        if (pvRetcode) {
//                //
//                // Initialize the lower layer HID driver and pass it the various
//                // structures and descriptors necessary to declare that we are a
//                // keyboard.
//                //
//                pvRetcode = USBDHIDInit (ui32Index, psHIDDevice);
//
//                return ((void *) psHIDDevice);
//        }
//        else {
//                return ((void *) 0);
//        }
//}



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

        /****************************************************************************/

        CallbackDTO callbackDTO;
        callbackDTO.deviceInfo = &deviceInfo;

        deviceInfo.psCallbacks = &handlers; //???
        deviceInfo.pui8DeviceDescriptor = deviceDescriptor;
        deviceInfo.ppsConfigDescriptors = configHeaders;
        deviceInfo.ppui8StringDescriptors = stringDescriptors;
        deviceInfo.ui32NumStringDescriptors = NUM_STRING_DESCRIPTORS;
        USBDCDDeviceInfoInit (0, &deviceInfo);
//        InternalUSBTickInit();

        USBStackModeSet(0, eUSBModeForceDevice, 0);
        USBDCDInit (0, &deviceInfo, &callbackDTO);
//        GPIOPinWrite (GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);

        while (1) {

        }
}

