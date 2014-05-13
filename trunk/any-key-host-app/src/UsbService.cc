/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "UsbService.h"
#include "Constants.h"
#include <libusb.h>
#include "Exception.h"
// TODO logging using BOOST_LOG. Log to syslog.
#include <iostream>

using std::exception;

/**
 * Internal structures hidden from the user.
 */
struct UsbService::Impl {
        libusb_device_handle *device = nullptr;
};

/*--------------------------------------------------------------------------*/

UsbService::UsbService ()
{
        impl = new Impl;
}

/*--------------------------------------------------------------------------*/

UsbService::~UsbService ()
{
        delete impl;
}

/*--------------------------------------------------------------------------*/

void UsbService::init ()
{
        int r = libusb_init(NULL);

        if (r < 0) {
                std::cerr << "Error libusb_init!" << std::endl;
                return;
        }

        impl->device = libusb_open_device_with_vid_pid(NULL, 0x20a0, 0x41ff);

        if (!impl->device) {
                throw Exception ("Error finding USB device");
        } else {
                std::cerr << "USB device has been found." << std::endl;
        }

        int rc = libusb_set_auto_detach_kernel_driver (impl->device, true);

        if (rc == LIBUSB_SUCCESS) {
                std::cerr << "libusb_set_auto_detach_kernel_driver OK" << std::endl;
        }
        else {
                throw Exception ("libusb_set_auto_detach_kernel_driver Failed!");
        }

        rc = libusb_claim_interface(impl->device, 0);

        if (rc < 0) {
                throw Exception ("Error claiming interface : " + std::string (libusb_error_name(rc)));
        } else {
                std::cerr << "Interface claimed OK" << std::endl;
        }

        if ((rc = libusb_set_interface_alt_setting (impl->device, 0, 0)) != 0) {
                throw Exception ("Error libusb_set_interface_alt_setting : " + std::string (libusb_error_name(rc)));
        } else {
                std::cerr << "libusb_set_interface_alt_setting OK" << std::endl;
        }
}

/*--------------------------------------------------------------------------*/

void UsbService::destroy ()
{
        if (!libusb_release_interface(impl->device, 0)) {
                std::cerr << "Interface released OK" << std::endl;
        } else {
                throw Exception ("Error releasing interface.");
        }

        if (impl->device) {
                libusb_close(impl->device);
                std::cerr << "Device closed OK" << std::endl;
        }

        libusb_exit (NULL);
}

/*--------------------------------------------------------------------------*/

void UsbService::printDevs ()
{
        libusb_device **devs;
        ssize_t cnt = libusb_get_device_list(NULL, &devs);

        if (cnt < 0) {
                std::cerr << "Error retrieving usbList cnt=" << cnt << std::endl;
                libusb_exit (NULL);
                exit(1);
        }

        libusb_device *dev;
        int i = 0, j = 0;
        uint8_t path[8];

        while ((dev = devs[i++]) != NULL) {
                struct libusb_device_descriptor desc;
                int r = libusb_get_device_descriptor(dev, &desc);
                if (r < 0) {
                        fprintf(stderr, "failed to get device descriptor");
                        return;
                }

                printf("%04x:%04x (bus %d, device %d)", desc.idVendor,
                                desc.idProduct, libusb_get_bus_number(dev),
                                libusb_get_device_address(dev));

                r = libusb_get_port_numbers(dev, path, sizeof(path));
                if (r > 0) {
                        printf(" path: %d", path[0]);
                        for (j = 1; j < r; j++)
                                printf(".%d", path[j]);
                }
                printf("\n");
        }

        libusb_free_device_list(devs, 1);
}
