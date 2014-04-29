/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "callbacks.h"
#include "usblib/usblib.h"
#include "usblib/usbhid.h"
#include "usblib/device/usbdevice.h"
#include "usblib/device/usbdhid.h"
#include "usblib/device/usbdhidkeyb.h"
#include "hidDescriptors.h"
#include "driverlib/debug.h"

/**
 *
 */
uint32_t usbReceiveEventCallback (void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgData, void *pvMsgData)
{
        CallbackDTO *callbackDTO;

        //
        // Make sure we did not get a NULL pointer.
        //
        ASSERT(pvCBData);

        //
        // Get a pointer to our instance data
        //
        callbackDTO = (CallbackDTO *)pvCBData;

        //
        // Which event were we sent?
        //
        switch (ui32Event)
        {
            //
            // The host has connected to us and configured the device.
            //
            case USB_EVENT_CONNECTED:
            {
                callbackDTO->ui8USBConfigured = true;
                break;
            }

            //
            // The host has disconnected from us.
            //
            case USB_EVENT_DISCONNECTED:
            {
                callbackDTO->ui8USBConfigured = false;
                break;
            }

            //
            // The host is polling us for a particular report and the HID driver
            // is asking for the latest version to transmit.
            //
            case USBD_HID_EVENT_IDLE_TIMEOUT:
            case USBD_HID_EVENT_GET_REPORT:
            {
                //
                // We only support a single input report so we don't need to check
                // the ui32MsgValue parameter in this case.  Set the report pointer
                // in *pvMsgData and return the length of the report in bytes.
                //
                *(uint8_t **)pvMsgData = callbackDTO->pui8Report;
                return(KEYB_IN_REPORT_SIZE);
            }

            //
            // The device class driver has completed sending a report to the
            // host in response to a Get_Report request.
            //
            case USBD_HID_EVENT_REPORT_SENT:
            {
                //
                // We have nothing to do here.
                //
                break;
            }

            //
            // This event is sent in response to a host Set_Report request.  We
            // must return a pointer to a buffer large enough to receive the
            // report into.
            //
            case USBD_HID_EVENT_GET_REPORT_BUFFER:
            {
                //
                // Are we being asked for a report that is shorter than the storage
                // we have set aside for this?  The only output report we define is
                // 8 bits long so we really expect to see a length of 1 passed.
                //
                if((uint32_t)pvMsgData == KEYB_OUT_REPORT_SIZE )
                {
                    //
                    // Yes - return our pointer.
                    //
                    return((uint32_t)callbackDTO->pui8DataBuffer);
                }
                else
                {
                    //
                    // We are being passed a report that is longer than the
                    // only report we expect so return NULL.  This causes the
                    // device class driver to stall the request.
                    //
                    return(0);
                }
            }

            //
            // This event indicates that the host has sent us an Output or
            // Feature report and that the report is now in the buffer we provided
            // on the previous USBD_HID_EVENT_GET_REPORT_BUFFER callback.
            //
            case USBD_HID_EVENT_SET_REPORT:
            {
                //
                // Inform the application if the keyboard LEDs have changed.
                //
                if(callbackDTO->ui8LEDStates != callbackDTO->pui8DataBuffer[0])
                {
                    //
                    // Note the new LED states.
                    //
                    callbackDTO->ui8LEDStates = callbackDTO->pui8DataBuffer[0];
                }
                break;
            }

            // The host is asking us to set either boot or report protocol.
            case USBD_HID_EVENT_SET_PROTOCOL:
            {
                callbackDTO->ui8Protocol = ui32MsgData;
                break;
            }

            //
            // The host is asking us to tell it which protocol we are currently
            // using, boot or request.
            //
            case USBD_HID_EVENT_GET_PROTOCOL:
            {
                return(callbackDTO->ui8Protocol);
            }

            //
            // Pass ERROR, SUSPEND and RESUME to the client unchanged.
            //
            case USB_EVENT_ERROR:
            case USB_EVENT_SUSPEND:
            case USB_EVENT_RESUME:
            case USB_EVENT_LPM_RESUME:
            case USB_EVENT_LPM_SLEEP:
            case USB_EVENT_LPM_ERROR:
            default:
            {
                break;
            }
        }
        return(0);
}

/**
 *
 */
uint32_t usbTransmitEventCallback (void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgParam, void *pvMsgData)
{
        CallbackDTO *callbackDTO;
        tUSBDHIDDevice *psHIDDevice;
        uint32_t ui32Count;

        //
        // Make sure we did not get a NULL pointer.
        //
        ASSERT(pvCBData);

        //
        // Get a pointer to our instance data
        //
        callbackDTO = (CallbackDTO *)pvCBData;
        psHIDDevice = callbackDTO->psHIDDevice;

        //
        // Which event were we sent?
        //
        switch (ui32Event)
        {
            //
            // A report transmitted via the interrupt IN endpoint was acknowledged
            // by the host.
            //
            case USB_EVENT_TX_COMPLETE:
            {
                //
                // Do we have any pending changes needing transmitted?
                //
                if(callbackDTO->bChangeMade)
                {
                    //
                    // Yes - go ahead and send another report immediately.
                    //
                    ui32Count = USBDHIDReportWrite((void *)psHIDDevice,
                                                 callbackDTO->pui8Report,
                                                 KEYB_IN_REPORT_SIZE, true);

                    //
                    // If we scheduled the report for transmission, clear the
                    // change flag.
                    //
                    if(ui32Count != 0)
                    {
                        callbackDTO->bChangeMade = false;
                    }
                }
                else
                {
                    //
                    // Our last transmission is complete and we have nothing more
                    // to send.
                    //
                    callbackDTO->eKeyboardState = HID_KEYBOARD_STATE_IDLE;
                }

                break;
            }

            //
            // We ignore all other events related to transmission of reports via
            // the interrupt IN endpoint.
            //
            default:
            {
                break;
            }
        }

        return(0);
}
