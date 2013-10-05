/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <iostream>
#include <thread>
#include <libusb.h>

void hello ()
{
        std::cout << "Hello Concurrent World\n";
}

void print_devs (libusb_device **devs)
{
        libusb_device *dev;
        int i = 0, j = 0;
        uint8_t path[8];

        while ((dev = devs[i++]) != NULL) {
                struct libusb_device_descriptor desc;
                int r = libusb_get_device_descriptor (dev, &desc);
                if (r < 0) {
                        fprintf (stderr, "failed to get device descriptor");
                        return;
                }

                printf ("%04x:%04x (bus %d, device %d)", desc.idVendor, desc.idProduct, libusb_get_bus_number (dev), libusb_get_device_address (dev));

                r = libusb_get_port_numbers (dev, path, sizeof(path));
                if (r > 0) {
                        printf (" path: %d", path[0]);
                        for (j = 1; j < r; j++)
                                printf (".%d", path[j]);
                }
                printf ("\n");
        }
}

int main ()
{
//        std::thread t (hello);
//        t.join ();

        libusb_device **devs;
        int r;
        ssize_t cnt;

        r = libusb_init (NULL);

        if (r < 0) {
                return r;
        }

        cnt = libusb_get_device_list (NULL, &devs);

        if (cnt < 0) {
                std::cerr << "Error retrieving usbList cnt=" << cnt << std::endl;
                libusb_exit (NULL);
                exit (1);
        }

        print_devs (devs);
        libusb_free_device_list (devs, 1);

        libusb_device_handle *devh = libusb_open_device_with_vid_pid (NULL, 0x20a0, 0x41ff);

        if (!devh) {
                std::cerr << "Error finding USB device" << std::endl;
                libusb_exit (NULL);
                exit (1);
        }
        else {
                std::cerr << "USB device has been found." << std::endl;
        }

        int rc = libusb_claim_interface (devh, 0);

        if (rc < 0) {
                std::cerr << "Error claiming interface : " << libusb_error_name (rc) << std::endl;
                libusb_exit (NULL);
                exit (1);
        }
        else {
                std::cerr << "Interface claimed." << std::endl;
        }

        if (!libusb_release_interface (devh, 0)) {
                std::cerr << "Interface released." << std::endl;
        }
        else {
                std::cerr << "Error releasing interface." << std::endl;
        }

        if (devh) {
                libusb_close(devh);
        }

        libusb_exit (NULL);
        return 0;
}
