/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

const unsigned int VENDOR_ID = 0x20a0;
const unsigned int PRODUCT_ID = 0x41ff;
const unsigned int ANY_KEY_SETUP_DATA_SIZE = 9;
const unsigned int MAX_NORMAL_KEYS = 6;
/// bRequest field of my custom request. This bRequest value tells the device to store the setup.
const unsigned int B_REQUEST_SET_ANY_KEY_SETUP = 0x00;
/// bRequest field of my custom request. This bRequest value tells the device to send the setup back to the host.
const unsigned int B_REQUEST_GET_ANY_KEY_SETUP = 0x01;

#endif /* CONSTANTS_H_ */
