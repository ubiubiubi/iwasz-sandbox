/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "AnyKeyUsbService.h"
#include "Constants.h"
#include <libusb.h>
#include "Exception.h"
// TODO logging using BOOST_LOG. Log to syslog.
#include <iostream>

using std::exception;

/**
 * Internal structures hidden from the user.
 */
struct AnyKeyUsbService::Impl {
        libusb_device_handle *device = nullptr;
        static void onControlTransferCompletion (libusb_transfer *xfr);
};

/*--------------------------------------------------------------------------*/

AnyKeyUsbService::AnyKeyUsbService ()
{
        impl = new Impl;
}

/*--------------------------------------------------------------------------*/

AnyKeyUsbService::~AnyKeyUsbService ()
{
        delete impl;
}

/*--------------------------------------------------------------------------*/

void AnyKeyUsbService::init ()
{
        int r = libusb_init(NULL);

        if (r < 0) {
                std::cerr << "Error libusb_init!" << std::endl;
                return;
        }

        impl->device = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, PRODUCT_ID);

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

void AnyKeyUsbService::destroy ()
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

void AnyKeyUsbService::transmitConfiguration (Buffer const &buf)
{
        if (buf.size () != ANY_KEY_SETUP_DATA_SIZE) {
                throw ("AnyKeyUsbService::transmitConfiguration : wrong buffer size.");
        }

        int ret = libusb_control_transfer (impl->device,
                        LIBUSB_ENDPOINT_OUT | LIBUSB_RECIPIENT_INTERFACE | LIBUSB_REQUEST_TYPE_VENDOR,
                        B_REQUEST_SET_ANY_KEY_SETUP, 0x00, 0x00, const_cast <uint8_t *> (&*buf.begin ()), buf.size (), 500);

        if (ret >= 0) {
                return;
        }

        switch (ret) {
        case LIBUSB_ERROR_TIMEOUT:
                throw Exception ("AnyKeyUsbService::transmitConfiguration : timeout reached when setting HUB to zero.");

        case LIBUSB_ERROR_PIPE:
                throw Exception ("AnyKeyUsbService::transmitConfiguration : the control request was not supported by the device.");

        case LIBUSB_ERROR_NO_DEVICE:
                throw Exception ("AnyKeyUsbService::transmitConfiguration : the device has been disconnected.");

        default:
                throw Exception ("AnyKeyUsbService::transmitConfiguration : undefined error : " + std::string (libusb_error_name(ret)) + ".");
        }
}

/*--------------------------------------------------------------------------*/

AnyKeyUsbService::Buffer AnyKeyUsbService::receiveConfiguration () const
{
        uint8_t configTransferBuffer[ANY_KEY_SETUP_DATA_SIZE];

        int ret = libusb_control_transfer (impl->device,
                        LIBUSB_ENDPOINT_IN | LIBUSB_RECIPIENT_INTERFACE | LIBUSB_REQUEST_TYPE_VENDOR,
                        B_REQUEST_GET_ANY_KEY_SETUP, 0x00, 0x00, configTransferBuffer, sizeof (configTransferBuffer), 500);

        if (ret < 0) {
                switch (ret) {
                case LIBUSB_ERROR_TIMEOUT:
                        throw Exception ("AnyKeyUsbService::transmitConfiguration : timeout reached when setting HUB to zero.");

                case LIBUSB_ERROR_PIPE:
                        throw Exception ("AnyKeyUsbService::transmitConfiguration : the control request was not supported by the device.");

                case LIBUSB_ERROR_NO_DEVICE:
                        throw Exception ("AnyKeyUsbService::transmitConfiguration : the device has been disconnected.");

                default:
                        throw Exception ("AnyKeyUsbService::transmitConfiguration : undefined error : " + std::string (libusb_error_name(ret)) + ".");
                }
        }

        for (int i = 0; i < ret; ++i) {
                printf ("%02x ", configTransferBuffer[i]);
        }
        std::cerr << ret << std::endl;

        Buffer buffer;
        buffer.reserve(ANY_KEY_SETUP_DATA_SIZE);
        std::copy (configTransferBuffer, configTransferBuffer + ANY_KEY_SETUP_DATA_SIZE, std::back_inserter (buffer));
        return buffer;
}

/*--------------------------------------------------------------------------*/

void AnyKeyUsbService::receiveConfiguration_deprecated () const
{
        libusb_transfer *xfr = libusb_alloc_transfer (0);

        if (!xfr) {
                throw Exception ("AnyKeyUsbService::receiveConfiguration : libusb_alloc_transfer returned NULL.");
        }

        /*
         * First 8 bytes are the request.
         * Next there are data to be sent in data stage of control transfer.
         */
        uint8_t configTransferBuffer[LIBUSB_CONTROL_SETUP_SIZE + ANY_KEY_SETUP_DATA_SIZE];

        // Populate the configTransferBuffer with request.
        libusb_fill_control_setup (configTransferBuffer,
                                   LIBUSB_ENDPOINT_IN | LIBUSB_RECIPIENT_INTERFACE | LIBUSB_REQUEST_TYPE_VENDOR,
                                   B_REQUEST_GET_ANY_KEY_SETUP,
                                   0x00,
                                   0x00,
                                   ANY_KEY_SETUP_DATA_SIZE);


        libusb_fill_control_transfer (xfr,
                                      impl->device,
                                      configTransferBuffer,
                                      &AnyKeyUsbService::Impl::onControlTransferCompletion,
                                      impl,
                                      100);

        int ret = libusb_submit_transfer(xfr);
        if (ret) {
                throw Exception ("AnyKeyUsbService::receiveConfiguration : libusb_submit_transfer failed with error : " + std::string (libusb_error_name (ret)));
        } else {
                std::cerr << "libusb_submit_transfer OK" << std::endl;
        }
}

/*
 * This is called from C.
 */
void AnyKeyUsbService::Impl::onControlTransferCompletion (libusb_transfer *xfr)
{
        fprintf (stderr, "AnyKeyUsbService::Impl::onControlTransferCompletion\n");

        int i;
        // Error checking on whole transfer.
        if (xfr->status != LIBUSB_TRANSFER_COMPLETED) {
                fprintf (stderr, "AnyKeyUsbService::Impl::onControlTransferCompletion : transfer failed with error : %s\n", libusb_error_name (xfr->status));
                libusb_free_transfer(xfr);
                return;
        }

#if 1
        printf("length:%u, actual_length:%u\n", xfr->length, xfr->actual_length);

        for (i = 0; i < xfr->actual_length; ++i) {
                printf ("%02x", xfr->buffer[i]);
        }

        printf ("\n");

//        std::cerr << "Length received : " << xfr->actual_length << std::endl;
#endif

        libusb_free_transfer(xfr);
}

/*--------------------------------------------------------------------------*/

void AnyKeyUsbService::loop ()
{
        while (true) {
                int rc = libusb_handle_events (NULL);

                if (rc != LIBUSB_SUCCESS) {
                        throw Exception ("Problem with handling event from the USB!");
                }
        }
}

/*--------------------------------------------------------------------------*/

void AnyKeyUsbService::printDevs ()
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
