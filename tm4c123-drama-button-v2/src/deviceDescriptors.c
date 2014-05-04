///****************************************************************************
// *                                                                          *
// *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
// *  ~~~~~~~~                                                                *
// *  License : see COPYING file for details.                                 *
// *  ~~~~~~~~~                                                               *
// ****************************************************************************/
//
//#include "deviceDescriptors.h"
//#include "driverlib/usb.h"
//
////*****************************************************************************
////
//// HID device configuration descriptor.
////
//// It is vital that the configuration descriptor bConfigurationValue field
//// (byte 6) is 1 for the first configuration and increments by 1 for each
//// additional configuration defined here.  This relationship is assumed in the
//// device stack for simplicity even though the USB 2.0 specification imposes
//// no such restriction on the bConfigurationValue values.
////
//// Note that this structure is deliberately located in RAM since we need to
//// be able to patch some values in it based on client requirements.
////
////*****************************************************************************
//static uint8_t g_pui8KeybDescriptor[] =
//{
//    //
//    // Configuration descriptor header.
//    //
//    9,                          // Size of the configuration descriptor.
//    USB_DTYPE_CONFIGURATION,    // Type of this descriptor.
//    USBShort(34),               // The total size of this full structure.
//    1,                          // The number of interfaces in this
//                                // configuration.
//    1,                          // The unique value for this configuration.
//    5,                          // The string identifier that describes this
//                                // configuration.
//    USB_CONF_ATTR_SELF_PWR,     // Bus Powered, Self Powered, remote wake up.
//    250,                        // The maximum power in 2mA increments.
//};
//
////*****************************************************************************
////
//// The remainder of the configuration descriptor is stored in flash since we
//// don't need to modify anything in it at runtime.
////
////*****************************************************************************
//static uint8_t g_pui8HIDInterface[HIDINTERFACE_SIZE] =
//{
//    //
//    // HID Device Class Interface Descriptor.
//    //
//    9,                          // Size of the interface descriptor.
//    USB_DTYPE_INTERFACE,        // Type of this descriptor.
//    0,                          // The index for this interface.
//    0,                          // The alternate setting for this interface.
//    1,                          // The number of endpoints used by this
//                                // interface.
//    USB_CLASS_HID,              // The interface class
//    USB_HID_SCLASS_NONE/*USB_HID_SCLASS_BOOT*/,        // The interface sub-class.
//    USB_HID_PROTOCOL_NONE/*USB_HID_PROTOCOL_KEYB*/,      // The interface protocol for the sub-class
//                                // specified above.
//    4,                          // The string index for this interface.
//};
//
//static const uint8_t g_pui8HIDInEndpoint[HIDINENDPOINT_SIZE] =
//{
//    //
//    // Interrupt IN endpoint descriptor
//    //
//    7,                          // The size of the endpoint descriptor.
//    USB_DTYPE_ENDPOINT,         // Descriptor type is an endpoint.
//    USB_EP_DESC_IN | USBEPToIndex(USB_EP_1),
//    USB_EP_ATTR_INT,            // Endpoint is an interrupt endpoint.
//    USBShort(USBFIFOSizeToBytes(USB_FIFO_SZ_64)),
//                                // The maximum packet size.
//    16,                         // The polling interval for this endpoint.
//};
//
//static const uint8_t g_pui8HIDOutEndpoint[HIDOUTENDPOINT_SIZE] =
//{
//    //
//    // Interrupt OUT endpoint descriptor
//    //
//    7,                          // The size of the endpoint descriptor.
//    USB_DTYPE_ENDPOINT,         // Descriptor type is an endpoint.
//    USB_EP_DESC_OUT | USBEPToIndex(USB_EP_2),
//    USB_EP_ATTR_INT,            // Endpoint is an interrupt endpoint.
//    USBShort(USBFIFOSizeToBytes(USB_FIFO_SZ_64)),
//                                // The maximum packet size.
//    16,                         // The polling interval for this endpoint.
//};
//
////*****************************************************************************
////
//// The following is the HID report structure definition that is passed back
//// to the host.
////
////*****************************************************************************
//static const uint8_t g_pui8KeybReportDescriptor[] =
//{
//    UsagePage(USB_HID_GENERIC_DESKTOP),
//    Usage(USB_HID_KEYBOARD),
//    Collection(USB_HID_APPLICATION),
//
//        //
//        // Modifier keys.
//        // 8 - 1 bit values indicating the modifier keys (ctrl, shift...)
//        //
//        ReportSize(1),
//        ReportCount(8),
//        UsagePage(USB_HID_USAGE_KEYCODES),
//        UsageMinimum(224),
//        UsageMaximum(231),
//        LogicalMinimum(0),
//        LogicalMaximum(1),
//        Input(USB_HID_INPUT_DATA | USB_HID_INPUT_VARIABLE | USB_HID_INPUT_ABS),
//
//        //
//        // One byte of rsvd data required by HID spec.
//        //
//        ReportCount(1),
//        ReportSize(8),
//        Input(USB_HID_INPUT_CONSTANT),
//
//        //
//        // Keyboard LEDs.
//        // 5 - 1 bit values.
//        //
//        ReportCount(5),
//        ReportSize(1),
//        UsagePage(USB_HID_USAGE_LEDS),
//        UsageMinimum(1),
//        UsageMaximum(5),
//        Output(USB_HID_OUTPUT_DATA | USB_HID_OUTPUT_VARIABLE |
//               USB_HID_OUTPUT_ABS),
//        //
//        // 1 - 3 bit value to pad out to a full byte.
//        //
//        ReportCount(1),
//        ReportSize(3),
//        Output(USB_HID_OUTPUT_CONSTANT), //LED report padding
//
//        //
//        // The Key buffer.
//        // 6 - 8 bit values to store the current key state.
//        //
//        ReportCount(6),
//        ReportSize(8),
//        LogicalMinimum(0),
//        LogicalMaximum(101),
//        UsagePage(USB_HID_USAGE_KEYCODES),
//        UsageMinimum (0),
//        UsageMaximum (101),
//        Input(USB_HID_INPUT_DATA | USB_HID_INPUT_ARRAY),
//    EndCollection
//};
//
////*****************************************************************************
////
//// The HID descriptor for the keyboard device.
////
////*****************************************************************************
//static const tHIDDescriptor g_sKeybHIDDescriptor =
//{
//    9,                              // bLength
//    USB_HID_DTYPE_HID,              // bDescriptorType
//    0x111,                          // bcdHID (version 1.11 compliant)
//    0,                              // bCountryCode (not localized)
//    1,                              // bNumDescriptors
//    {
//        {
//            USB_HID_DTYPE_REPORT,   // Report descriptor
//            sizeof(g_pui8KeybReportDescriptor)
//                                    // Size of report descriptor
//        }
//    }
//};
//
////*****************************************************************************
////
//// The HID configuration descriptor is defined as four or five sections
//// depending upon the client's configuration choice.  These sections are:
////
//// 1.  The 9 byte configuration descriptor (RAM).
//// 2.  The interface descriptor (RAM).
//// 3.  The HID report and physical descriptors (provided by the client)
////     (FLASH).
//// 4.  The mandatory interrupt IN endpoint descriptor (FLASH).
//// 5.  The optional interrupt OUT endpoint descriptor (FLASH).
////
////*****************************************************************************
//static const tConfigSection g_sHIDConfigSection =
//{
//    sizeof(g_pui8KeybDescriptor),
//    g_pui8KeybDescriptor
//};
//
//static const tConfigSection g_sHIDInterfaceSection =
//{
//    sizeof(g_pui8HIDInterface),
//    g_pui8HIDInterface
//};
//
//static const tConfigSection g_sHIDInEndpointSection =
//{
//    sizeof(g_pui8HIDInEndpoint),
//    g_pui8HIDInEndpoint
//};
//
//static const tConfigSection g_sHIDOutEndpointSection =
//{
//    sizeof(g_pui8HIDOutEndpoint),
//    g_pui8HIDOutEndpoint
//};
//
////*****************************************************************************
////
//// Place holder for the user's HID descriptor block.
////
////*****************************************************************************
//static tConfigSection g_sHIDDescriptorSection =
//{
//   sizeof(g_sKeybHIDDescriptor),
//   (const uint8_t *)&g_sKeybHIDDescriptor
//};
//
////*****************************************************************************
////
//// This array lists all the sections that must be concatenated to make a
//// single, complete HID configuration descriptor.
////
////*****************************************************************************
//static const tConfigSection *g_psHIDSections[] =
//{
//    &g_sHIDConfigSection,
//    &g_sHIDInterfaceSection,
//    &g_sHIDDescriptorSection,
//    &g_sHIDInEndpointSection,
//    &g_sHIDOutEndpointSection
//};
//
//#define NUM_HID_SECTIONS        ((sizeof(g_psHIDSections) /                   \
//                                  sizeof(g_psHIDSections[0])) - 1)
//
////*****************************************************************************
////
//// The header for the single configuration we support.  This is the root of
//// the data structure that defines all the bits and pieces that are pulled
//// together to generate the configuration descriptor.  Note that this must be
//// in RAM since we need to include or exclude the final section based on
//// client supplied initialization parameters.
////
////*****************************************************************************
//static tConfigHeader g_sHIDConfigHeader =
//{
//    NUM_HID_SECTIONS,
//    g_psHIDSections
//};
//
////*****************************************************************************
////
//// Configuration Descriptor.
////
////*****************************************************************************
//const tConfigHeader * const g_ppsHIDConfigDescriptors[] =
//{
//    &g_sHIDConfigHeader
//};
