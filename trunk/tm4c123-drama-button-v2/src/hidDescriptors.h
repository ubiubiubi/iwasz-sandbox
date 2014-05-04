///****************************************************************************
// *                                                                          *
// *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
// *  ~~~~~~~~                                                                *
// *  License : see COPYING file for details.                                 *
// *  ~~~~~~~~~                                                               *
// ****************************************************************************/
//
//#ifndef DESCRIPTORS_H_
//#define DESCRIPTORS_H_
//
//#include <stdint.h>
//#include <stdbool.h>
//#include "usblib/usblib.h"
//#include "usblib/usbhid.h"
//#include "usblib/device/usbdevice.h"
//#include "usblib/device/usbdhid.h"
//#include "usblib/device/usbdhidkeyb.h"
//
//#define VENDOR_ID 0x20a0
//#define PRODUCT_ID 0x41ff
//
//extern const uint8_t * const stringDescriptors[];
//extern const uint8_t reportDescriptor[];
//extern const uint8_t * const classDescriptors[];
//extern const tHIDDescriptor hidDescriptor;
//extern tHIDReportIdle reportIdle[1];
//extern uint8_t g_pui8KeybDescriptor[];
////extern tUSBDHIDDevice usbHIDDevice;
//
//#ifdef KEYB_IN_REPORT_SIZE
//#undef KEYB_IN_REPORT_SIZE
//#define KEYB_IN_REPORT_SIZE 1
//#endif
//
//#ifdef KEYB_OUT_REPORT_SIZE
//#undef KEYB_OUT_REPORT_SIZE
//#define KEYB_OUT_REPORT_SIZE 1
//#endif
//
//#ifdef KEYB_MAX_CHARS_PER_REPORT
//#undef KEYB_MAX_CHARS_PER_REPORT
//#define KEYB_MAX_CHARS_PER_REPORT 5
//#endif
//
//extern uint8_t NUM_STRING_DESCRIPTORS;
//
///**
// * Passed to the callbacks.
// */
//struct _CallbackDTO {
//        //
//        // The USB configuration number set by the host or 0 of the device is
//        // currently unconfigured.
//        //
//        uint8_t ui8USBConfigured;
//
//        //
//        // The protocol requested by the host, USB_HID_PROTOCOL_BOOT or
//        // USB_HID_PROTOCOL_REPORT.
//        //
//        uint8_t ui8Protocol;
//
//        //
//        // The current states that the keyboard LEDs are to be set to.
//        //
//        volatile uint8_t ui8LEDStates;
//
//        //
//        // The total number of keys currently pressed.  This indicates the number
//        // of key press entries in the pui8KeysPressed array.
//        //
//        uint8_t ui8KeyCount;
//
//        //
//        // The current state of the keyboard interrupt IN endpoint.
//        //
//        volatile tKeyboardState eKeyboardState;
//
//        //
//        // A flag to indicate that the application pressed or released a key
//        // but that we couldn't send the report immediately.
//        //
//        volatile bool bChangeMade;
//
//        //
//        // A buffer used to receive output reports from the host.
//        //
//        uint8_t pui8DataBuffer[KEYB_OUT_REPORT_SIZE];
//
//        //
//        // A buffer used to hold the last input report sent to the host.
//        //
//        uint8_t pui8Report[KEYB_IN_REPORT_SIZE];
//
//        //
//        // A buffer containing the usage codes of all non-modifier keys currently
//        // in the pressed state.
//        //
//        uint8_t pui8KeysPressed[KEYB_MAX_CHARS_PER_REPORT];
//
//        //
//        // The idle timeout control structure for our input report.  This is
//        // required by the lower level HID driver.
//        //
//        tHIDReportIdle sReportIdle;
//
//        tUSBDHIDDevice *psHIDDevice;
//};
//
//typedef struct _CallbackDTO CallbackDTO;
//
//#endif /* DESCRIPTORS_H_ */
