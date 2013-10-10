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
#include <signal.h>

bool doExit = false;

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

static void LIBUSB_CALL cb_xfr (libusb_transfer *xfr)
{
        int i;
        // Error checkin on whole transfer.
        if (xfr->status != LIBUSB_TRANSFER_COMPLETED) {
                fprintf(stderr, "transfer status %d\n", xfr->status);
                libusb_free_transfer(xfr);
                exit(3);
        }

        // Error checking on every individual packet as suggested in the API docs.
        for (i = 0; i < xfr->num_iso_packets; i++) {
                struct libusb_iso_packet_descriptor *pack = &xfr->iso_packet_desc[i];

                if (pack->status != LIBUSB_TRANSFER_COMPLETED) {
                        fprintf(stderr, "Error: pack %u status %d\n", i, pack->status);
                        exit(5);
                }
                printf("pack%u length:%u, actual_length:%u\n", i, pack->length, pack->actual_length);

                uint8_t *buffer = libusb_get_iso_packet_buffer (xfr, 0);

                printf ("[");
                for (unsigned int i = 0; i < pack->actual_length; ++i) {
                        printf("%02x", buffer[i]);
                }
                printf("]\n");
        }

        printf("length:%u, actual_length:%u\n", xfr->length, xfr->actual_length);

        for (i = 0; i < xfr->actual_length; ++i) {
                printf("%02x", xfr->buffer[i]);
        }
        printf ("\n");

//        std::cerr << "Length received : " << xfr->actual_length << std::endl;

        if (libusb_submit_transfer(xfr) < 0) {
                fprintf(stderr, "error re-submitting URB\n");
                exit(1);
        }
}

static void sig_hdlr(int signum)
{
        switch (signum) {
        case SIGINT:
                std::cerr << "SIGINT !!!!" << std::endl;
                doExit = true;
                break;
        }
}

int main ()
{
//        std::thread t (hello);
//        t.join ();

        struct sigaction sigact;
        sigact.sa_handler = sig_hdlr;
        sigemptyset(&sigact.sa_mask);
        sigact.sa_flags = 0;
        sigaction(SIGINT, &sigact, NULL);


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

        if ((rc = libusb_set_interface_alt_setting (devh, 0, 1)) != 0) {
                std::cerr << "Error libusb_set_interface_alt_setting rc = "  << rc << std::endl;
                libusb_exit (NULL);
                exit (1);
        }
        else {
                std::cerr << "libusb_set_interface_alt_setting OK" << std::endl;
        }

        /*--------------------------------------------------------------------------*/

        int num_iso_pack = 1; // ?!?!? CO to jest!
        libusb_transfer *xfr = libusb_alloc_transfer (num_iso_pack);

        if (!xfr) {
                std::cerr << "Error" << std::endl;
                libusb_exit (NULL);
                exit (1);
        }

        uint8_t buf[4];
        libusb_fill_iso_transfer (xfr, devh, 0x81, buf, sizeof(buf), num_iso_pack, cb_xfr, NULL, 0);
        libusb_set_iso_packet_lengths (xfr, sizeof(buf)/num_iso_pack);

//        libusb_fill_interrupt_transfer(xfr, devh, 0x81, buf, sizeof (buf), cb_xfr, NULL, 100);

        int ret = libusb_submit_transfer (xfr);
        if (ret) {
                std::cerr << "libusb_submit_transfer error :" << std::endl;
                switch (ret) {
                case LIBUSB_ERROR_NO_DEVICE:
                        std::cerr << "LIBUSB_ERROR_NO_DEVICE" << std::endl;
                        break;
                case LIBUSB_ERROR_BUSY:
                        std::cerr << "LIBUSB_ERROR_BUSY" << std::endl;
                        break;
                case LIBUSB_ERROR_NOT_SUPPORTED:
                        std::cerr << "LIBUSB_ERROR_NOT_SUPPORTED" << std::endl;
                        break;
                default:
                        std::cerr << "Error nr : " << ret << std::endl;
                        break;
                }
        }
        else {
                std::cerr << "libusb_submit_transfer OK" << std::endl;
        }

        while (!doExit) {
                rc = libusb_handle_events(NULL);

                if (rc != LIBUSB_SUCCESS)
                        break;
        }

        /*--------------------------------------------------------------------------*/

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
