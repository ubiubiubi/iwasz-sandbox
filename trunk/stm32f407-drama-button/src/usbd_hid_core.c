/**
  ******************************************************************************
  * @file    usbd_hid_core.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    19-March-2012
  * @brief   This file provides the HID core functions.
  *
  * @verbatim
  *      
  *          ===================================================================      
  *                                HID Class  Description
  *          =================================================================== 
  *           This module manages the HID class V1.11 following the "Device Class Definition
  *           for Human Interface Devices (HID) Version 1.11 Jun 27, 2001".
  *           This driver implements the following aspects of the specification:
  *             - The Boot Interface Subclass
  *             - The Mouse protocol
  *             - Usage Page : Generic Desktop
  *             - Usage : Joystick)
  *             - Collection : Application 
  *      
  * @note     In HS mode and when the DMA is used, all variables and data structures
  *           dealing with the DMA during the transaction process should be 32-bit aligned.
  *           
  *      
  *  @endverbatim
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usbd_hid_core.h"
#include "usbd_desc.h"
#include "usbd_req.h"

static uint8_t USBD_HID_Init (void *pdev, uint8_t cfgidx);
static uint8_t USBD_HID_DeInit (void *pdev, uint8_t cfgidx);
static uint8_t USBD_HID_Setup (void *pdev, USB_SETUP_REQ *req);
static uint8_t *USBD_HID_GetCfgDesc (uint8_t speed, uint16_t *length);
static uint8_t USBD_HID_DataIn (void *pdev, uint8_t epnum);

USBD_Class_cb_TypeDef  USBD_HID_cb = 
{
  USBD_HID_Init,
  USBD_HID_DeInit,
  USBD_HID_Setup,
  NULL, /*EP0_TxSent*/  
  NULL, /*EP0_RxReady*/
  USBD_HID_DataIn, /*DataIn*/
  NULL, /*DataOut*/
  NULL, /*SOF */
  NULL,
  NULL,      
  USBD_HID_GetCfgDesc,
};

__ALIGN_BEGIN uint32_t  USBD_HID_AltSet  __ALIGN_END = 0;
__ALIGN_BEGIN uint32_t  USBD_HID_Protocol  __ALIGN_END = 0;
__ALIGN_BEGIN uint32_t  USBD_HID_IdleState __ALIGN_END = 0;

// Actual report to send.
uint8_t keyboardReport[HID_IN_PACKET] = { 0x00, 0x00 };
uint8_t lastKey = 0x00;

/*
 * Configuration descriptor.
 */
__ALIGN_BEGIN static uint8_t USBD_HID_CfgDesc[USB_HID_CONFIG_DESC_SIZ] __ALIGN_END =
{
        0x09,           /* bLength (1) Configuration Descriptor size in bytes, always 0x09. */
        USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType (1B) Stała 0x02 (patrz tabelka typów deskryptorów */
        USB_HID_CONFIG_DESC_SIZ,           /* wTotalLength (2B) Rozmiar w bajtach całego deskryptora wraz z jego "doklejonymi" */
        0x00,                              /* deskryptorami interfejsów i endpointów. */
        0x01,         /* bNumInterfaces (1B) Liczba interfejsów. */
        0x01,         /* bConfigurationValue (1B) numer porządkowy konfiguracji. Musi być większy równy 1. Rozumiem, że musi być */
                      /* unikalny dla każdej konfiguracji. */
        0x00,         /* iConfiguration (1B) Index of string descriptor describing the configuration, albo 0, jeśli nie ma. */
        0xE0,         /* bmAttributes (1B) Atrybuty w postaci maski bitowej. Bit 6==1 -> self powered. Bit 6==0 -> bus powered */
                      /* Bit 5==1 -> urządzenie obsługuje "remote wakeup feature". Bit5==0 -> nie obsługuje. Bit 4..0 muszą być 0 */
                      /* Bit7 musi być równy 1. */
        0x32,         /* bMaxPower (1B) Max prąd w jednostkach 2mA dla USB 2.0 i w jednostkach 8mA dla USB 3.0. W tym przypadku */
                      /* 0x32 = 50, czyli 100mA. */

        /*
         * Device descriptor of keyboard interface.
         */
        0x09,         /* bLength : Interface Descriptor size == 0x09. */
        USB_INTERFACE_DESCRIPTOR_TYPE,/* bDescriptorType : Interface descriptor type (constant 0x04). */
        0x00,         /* bInterfaceNumber: Unikalny numer. Urządzenia typu composite będą miały następne interfejsy, a każdy */
                      /* będzie miał kolejne numery. Domyślny interfejs ma numer 0. */
        0x00,         /* bAlternateSetting: Unikalny numer "alternate settings". Każdy interfejs może mieć odmiany, które */
                      /* właśnie nazywamy "alternate setting". Każde takie ustawienie musi mieć unikalny numer w tym polu. */
                      /* Domyślny numer to 0 i kazdy interfejs musi mieć takie "alternatywne ustawienie" o nr. 0. */
        0x01,         /* bNumEndpoints : Liczba endpointów prócz EP0. */
        0x03,         /* bInterfaceClass : Klasa. Listę można znaleźć na wikipedii. 0xff to vendor specific. Klasę urządzenia */
                      /* można także podać w deskryptorze urządzenia, ale najczęściej się podaj tu. 0x03 to HID. */
        0x01,         /* bInterfaceSubClass : 1=BOOT, 0=no boot. Ustanowione przez USB-IF dla klas. 0xff == vendor specific. */
                      /* Linux has some trouble indentifying the device when boot == 0 here. */
        0x01,         /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse. Ustanowione przez USB-IF dla klas. 0xff == vendor */
                      /* specific. Ustawiamy tylko gdy boot protocol. */
        0,            /* iInterface: Index of string descriptor, or 0 if there is no string. */

        /*
         * HID descriptor (mouse).
         */
        0x09,         /* bLength: HID Descriptor size*/
        HID_DESCRIPTOR_TYPE, /* bDescriptorType: HID*/
        0x11,         /* bcdHID: HID Class Spec release number*/
        0x01,
        0x00,         /* bCountryCode: Hardware target country*/
        0x01,         /* bNumDescriptors: Number of HID class descriptors to follow*/
        0x22,         /* bDescriptorType*/
        HID_KEYBOARD_REPORT_DESC_SIZE,/* wItemLength: Total length of Report descriptor*/
        0x00,

        /*
         * Endpoint descriptor.
         */
        0x07,          /* bLength: Endpoint Descriptor size */
        USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: Stała 0x05 */
        HID_IN_EP,     /* bEndpointAddress: Endpoint Address. 4 LSB to numer endpointu. Urządzenia LS moga mieć w interfejsie */
                       /* max 3 endpointy. Pozostałe urządzenia mogą mieć 16. Bit MSB to kierunek : 0 == OUT, 1 == IN. Bity */\
                       /* 6..4 muszą być 0. */
        0x03,          /* bmAttributes : */
                       /* Bits 0..1 Transfer Type */
                       /*  00 = Control */
                       /*  01 = Isochronous */
                       /*  10 = Bulk */
                       /*  11 = Interrupt */
                       /* Bits 2..7 are reserved. If Isochronous endpoint, */
                       /* Bits 3..2 = Synchronisation Type (Iso Mode) */
                       /*  00 = No Synchonisation */
                       /*  01 = Asynchronous */
                       /*  10 = Adaptive */
                       /*  11 = Synchronous */
                       /* Bits 5..4 = Usage Type (Iso Mode) */
                       /*  00 = Data Endpoint */
                       /*  01 = Feedback Endpoint */
                       /*  10 = Explicit Feedback Data Endpoint */
                       /*  11 = Reserved */
        HID_IN_PACKET, /* wMaxPacketSize (2B): Dla USB 2.0 Bity 0-10 (LSB) oznaczają liczbę bajtów. To może być wartść od 0 */
                       /* do 1204. Bity 11 i 12 oznaczają liczbę dodatkowych transakcji w mikro ramce dla transferów iso */
                       /* i interrupt  (0-2, bo 11 jest zarezerwowane). Bity 13-15 musza być 0. */
        0x00,
        0x0A,          /* bInterval: Polling Interval. Interval for polling endpoint data transfers. Value in frame counts (ms). */
                       /* Ignored for Bulk & Control Endpoints. Isochronous must equal 1 and field may range from 1 to 255 for */
                       /* interrupt endpoints. */
};

/**
 * Report descriptor. It only describes the report, this is not the report itself.
 */
//__ALIGN_BEGIN static uint8_t HID_KEYBOARD_ReportDesc[HID_KEYBOARD_REPORT_DESC_SIZE] __ALIGN_END = {
//        0x05, 0x01,
//        0x09, 0x02,
//        0xA1, 0x01,
//        0x09, 0x01,
//        0xA1, 0x00,
//        0x05, 0x09,
//        0x19, 0x01,
//        0x29, 0x03,
//        0x15, 0x00,
//        0x25, 0x01,
//        0x95, 0x03,
//        0x75, 0x01,
//        0x81, 0x02,
//        0x95, 0x01,
//        0x75, 0x05,
//        0x81, 0x01,
//        0x05, 0x01,
//        0x09, 0x30,
//        0x09, 0x31,
//        0x09, 0x38,
//        0x15, 0x81,
//        0x25, 0x7F,
//        0x75, 0x08,
//        0x95, 0x03,
//        0x81, 0x06,
//        0xC0, 0x09,
//        0x3c, 0x05,
//        0xff, 0x09,
//        0x01, 0x15,
//        0x00, 0x25,
//        0x01, 0x75,
//        0x01, 0x95,
//        0x02, 0xb1,
//        0x22, 0x75,
//        0x06, 0x95,
//        0x01, 0xb1,
//        0x01, 0xc0
//};

/**
 * Report descriptor. It only describes the report, this is not the report itself.
 * Based on USB HID spec.
 */
#if 0
__ALIGN_BEGIN static uint8_t HID_KEYBOARD_ReportDesc[HID_KEYBOARD_REPORT_DESC_SIZE] __ALIGN_END = {
        0x05, 0x01, // Usage Page (Generic Desktop),
        0x09, 0x06, // Usage (Keyboard),
        0xA1, 0x01, // Collection (Application),
        0x05, 0x07, //  Usage Page (Key Codes);
        0x19, 0xE0, //  Usage Minimum (224),
        0x29, 0xE7, //  Usage Maximum (231),
        0x15, 0x00, //  Logical Minimum (0),
        0x25, 0x01, //  Logical Maximum (1),
        0x75, 0x01, //  Report Size (1 bits?),
        0x95, 0x08, //  Report Count (8 * report size i.e. 8 * 1 bit),
        0x81, 0x02, //  Input (Data, Variable, Absolute), ;Modifier byte

        0x95, 0x01, //  Report Count (1),
        0x75, 0x08, //  Report Size (8),
        0x81, 0x01, //  Input (Constant), ;Reserved byte

//        0x95, 0x05, //  Report Count (5),
//        0x75, 0x01, //  Report Size (1),
//        0x05, 0x08, //  Usage Page (Page# for LEDs),
//        0x19, 0x01, //  Usage Minimum (1),
//        0x29, 0x05, //  Usage Maximum (5),
//        0x91, 0x02, //  Output (Data, Variable, Absolute), ;LED report
//
//        0x95, 0x01, //  Report Count (1),
//        0x75, 0x03, //  Report Size (3),
//        0x91, 0x01, //  Output (Constant), ;LED report padding

        0x95, 0x06, //  Report Count (6),
        0x75, 0x08, //  Report Size (8),
        0x15, 0x00, //  Logical Minimum (0),
        0x25, 0x65, //  Logical Maximum (101),
        0x05, 0x07, //  Usage Page (Key Codes),
        0x19, 0x00, //  Usage Minimum (0),
        0x29, 0x65, //  Usage Maximum (101),
        0x81, 0x00, //  Input (Data, Array), ;Key arrays (6 bytes)
        0xC0       // End Collection
};
#endif

#if 0
/**
 * Simple keyboard with only one key.
 * size = 35
 */
__ALIGN_BEGIN static uint8_t HID_KEYBOARD_ReportDesc[HID_KEYBOARD_REPORT_DESC_SIZE] __ALIGN_END = {
        0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
        0x09, 0x06,                    // USAGE (Keyboard)
        0xa1, 0x01,                    // COLLECTION (Application)
        0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
        0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
        0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
        0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
        0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
        0x75, 0x01,                    //   REPORT_SIZE (1)
        0x95, 0x08,                    //   REPORT_COUNT (8)
        0x81, 0x02,                    //   INPUT (Data,Var,Abs)
        0x95, 0x01,                    //   REPORT_COUNT (1)
        0x75, 0x08,                    //   REPORT_SIZE (8)
        0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
        0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
        0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
        0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
        0xc0                           // END_COLLECTION
};
#endif

/**
 * bit 7 nu
 * bit 6 vol down
 * bit 5 vol up
 * bit 4 mute
 * bit 3 play / pause
 * bit 2 stop
 * bit 1 prev track
 * bit 0 next track
 *
 * 0x01 Next Track
 * 0x02 Prev Track
 * 0x04 Stop
 * 0x08 Play / Pause
 * 0x10 Mute
 * 0x20 Volume Up
 * 0x40 Volume Down
 *
 * size = 37
 * report size = 7 bits (one byte?)
 */
__ALIGN_BEGIN static uint8_t HID_KEYBOARD_ReportDesc[HID_KEYBOARD_REPORT_DESC_SIZE] __ALIGN_END = {
        0x05, 0x0C, // Usage Page (Consumer Devices)
        0x09, 0x01, // Usage (Consumer Control)
        0xA1, 0x01, // Collection (Application)
        0x05, 0x0C, // Usage Page (Consumer Devices)
        0x15, 0x00, // Logical Minimum (0)
        0x25, 0x01, // Logical Maximum (1)
        0x75, 0x01, // Report Size (1)
        0x95, 0x07, // Report Count (7)
        0x09, 0xB5, // Usage (Scan Next Track)
        0x09, 0xB6, // Usage (Scan Previous Track)
        0x09, 0xB7, // Usage (Stop)
        0x09, 0xCD, // Usage (Play / Pause)
        0x09, 0xE2, // Usage (Mute)
        0x09, 0xE9, // Usage (Volume Up)
        0x09, 0xEA, // Usage (Volume Down)
        0x81, 0x02, // Input (Data, Variable, Absolute)
        0x95, 0x01, // Report Count (1)
        0x81, 0x01, // Input (Constant)
        0xC0 // End Collection
};

/**
 * @brief  USBD_HID_Init
 *         Initialize the HID interface
 * @param  pdev: device instance
 * @param  cfgidx: Configuration index
 * @retval status
 */
static uint8_t USBD_HID_Init (void *pdev, uint8_t cfgidx)
{

        /* Open EP IN */
        DCD_EP_Open (pdev, HID_IN_EP, HID_IN_PACKET, USB_OTG_EP_INT);

        /* Open EP OUT */
        DCD_EP_Open (pdev, HID_OUT_EP, HID_OUT_PACKET, USB_OTG_EP_INT);

        return USBD_OK;
}

/**
 * @brief  USBD_HID_Init
 *         DeInitialize the HID layer
 * @param  pdev: device instance
 * @param  cfgidx: Configuration index
 * @retval status
 */
static uint8_t USBD_HID_DeInit (void *pdev, uint8_t cfgidx)
{
        /* Close HID EPs */
        DCD_EP_Close (pdev, HID_IN_EP);
        DCD_EP_Close (pdev, HID_OUT_EP);

        return USBD_OK;
}

/**
 * @brief  USBD_HID_Setup
 *         Handle the HID specific requests
 * @param  pdev: instance
 * @param  req: usb requests
 * @retval status
 */
static uint8_t USBD_HID_Setup (void *pdev, USB_SETUP_REQ *req)
{
        uint16_t len = 0;
        uint8_t *pbuf = NULL;

        switch (req->bmRequest & USB_REQ_TYPE_MASK) {
        case USB_REQ_TYPE_CLASS:
                switch (req->bRequest) {

                case HID_REQ_SET_PROTOCOL: // required for HIDs that support a boot protocol
                        USBD_HID_Protocol = (uint8_t) (req->wValue);
                        break;

                case HID_REQ_GET_PROTOCOL: // required for HIDs that support a boot protocol
                        USBD_CtlSendData (pdev, (uint8_t *) &USBD_HID_Protocol, 1);
                        break;

                case HID_REQ_SET_IDLE: // Not required, except for keyboards using the boot protocol
                        USBD_HID_IdleState = (uint8_t) (req->wValue >> 8);
                        break;

                case HID_REQ_GET_IDLE: // Optional for keyboards (?)
                        USBD_CtlSendData (pdev, (uint8_t *) &USBD_HID_IdleState, 1);
                        break;

                case HID_REQ_GET_REPORT:
                {
                        buildReport (0x00, lastKey);
                        USBD_HID_SendReport (pdev);
                        break;
                }

                default:
                        USBD_CtlError (pdev, req);
                        return USBD_FAIL;
                }
                break;

        case USB_REQ_TYPE_STANDARD:
                switch (req->bRequest) {
                case USB_REQ_GET_DESCRIPTOR:
                        if (req->wValue >> 8 == HID_REPORT_DESC) {
                                len = MIN(HID_KEYBOARD_REPORT_DESC_SIZE , req->wLength);
                                pbuf = HID_KEYBOARD_ReportDesc;
                        }
                        else if (req->wValue >> 8 == HID_DESCRIPTOR_TYPE) {
                                pbuf = USBD_HID_CfgDesc + 0x12;
                                len = MIN(USB_HID_DESC_SIZ , req->wLength);
                        }

                        USBD_CtlSendData (pdev, pbuf, len);

                        break;

                case USB_REQ_GET_INTERFACE:
                        USBD_CtlSendData (pdev, (uint8_t *) &USBD_HID_AltSet, 1);
                        break;

                case USB_REQ_SET_INTERFACE:
                        USBD_HID_AltSet = (uint8_t) (req->wValue);
                        break;
                }
        }
        return USBD_OK;
}

/**
 * @brief  USBD_HID_SendReport
 *         Send HID Report
 * @param  pdev: device instance
 * @param  buff: pointer to report
 * @retval status
 */
uint8_t USBD_HID_SendReport (USB_OTG_CORE_HANDLE *pdev)
{
        if (pdev->dev.device_status == USB_OTG_CONFIGURED) {
                DCD_EP_Tx (pdev, HID_IN_EP, keyboardReport, HID_IN_PACKET);
        }
        return USBD_OK;
}

void buildReport (uint8_t modifier, uint8_t key)
{
        keyboardReport[0] = modifier; // Modifier.
        keyboardReport[1] = key; // Key (A).
}

/**
 * @brief  USBD_HID_GetCfgDesc
 *         return configuration descriptor
 * @param  speed : current device speed
 * @param  length : pointer data length
 * @retval pointer to descriptor buffer
 */
static uint8_t *USBD_HID_GetCfgDesc (uint8_t speed, uint16_t *length)
{
        *length = sizeof(USBD_HID_CfgDesc);
        return USBD_HID_CfgDesc;
}

/**
 * @brief  USBD_HID_DataIn
 *         handle data IN Stage
 * @param  pdev: device instance
 * @param  epnum: endpoint index
 * @retval status
 */
static uint8_t USBD_HID_DataIn (void *pdev, uint8_t epnum)
{
        /* Ensure that the FIFO is empty before a new transfer, this condition could
         be caused by  a new transfer before the end of the previous transfer */
        DCD_EP_Flush (pdev, HID_IN_EP);
        return USBD_OK;
}

