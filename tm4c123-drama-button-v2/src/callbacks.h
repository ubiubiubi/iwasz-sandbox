/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef CALLBACKS_H_
#define CALLBACKS_H_

extern uint32_t usbReceiveEventCallback (void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgParam, void *pvMsgData);
extern uint32_t usbTransmitEventCallback (void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgParam, void *pvMsgData);

#endif /* CALLBACKS_H_ */
