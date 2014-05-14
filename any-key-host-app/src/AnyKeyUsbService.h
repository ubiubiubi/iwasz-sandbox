/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef USBSERVICE_H_
#define USBSERVICE_H_

#include <exception>
#include <vector>
#include <cstdint>
// TODO BOOST_LOG
#include <iostream>

/**
 * USB stuff for anyKey.
 */
class AnyKeyUsbService {
public:

        typedef std::vector <uint8_t> Buffer;

        AnyKeyUsbService ();
        ~AnyKeyUsbService ();

        AnyKeyUsbService (AnyKeyUsbService const &) = delete;
        AnyKeyUsbService &operator= (AnyKeyUsbService const &) = delete;

        /**
         * Connects to the device, and claims its interface.
         */
        void init ();

        /**
         * Releases the interface.
         */
        void destroy ();

        /**
         * Prints all the available USB devices connected to the host we are running on.
         */
        void printDevs ();

        /**
         * USB event loop.
         */
        void loop ();

        void transmitConfiguration (Buffer const &buf);
        void receiveConfiguration_deprecated () const;
        Buffer receiveConfiguration () const;

private:

        // PIMPL idiom prevents exposing usb headers to user (I am the only user, but who cares).
        struct Impl;
        Impl *impl;
};

/**
 * RAII capabilities for UsbService.
 */
template <typename Service>
struct UsbServiceGuard {
        UsbServiceGuard ()
        {
                service.init ();
        }

        ~UsbServiceGuard ()
        {
                try {
                        service.destroy ();
                }
                catch (std::exception const &e) {
                        std::cerr << "An exception has occured in ~UsbServiceGuard. Message : " << e.what () << std::endl;
                }
                catch (...) {
                        std::cerr << "Unknown exception has occured in ~UsbServiceGuard." << std::endl;
                }
        }

        Service service;
};

#endif /* USBSERVICE_H_ */
