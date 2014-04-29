/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef DEVICEDESCRIPTORS_H_
#define DEVICEDESCRIPTORS_H_

#include <stdint.h>
#include <stdbool.h>
#include "usblib/usblib.h"
#include "usblib/usbhid.h"
#include "usblib/device/usbdevice.h"
#include "usblib/device/usbdhid.h"
#include "usblib/device/usbdhidkeyb.h"

extern const tConfigHeader * const g_ppsHIDConfigDescriptors[];

#endif /* DEVICEDESCRIPTORS_H_ */
