/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <iostream>
#include <libusb.h>
#include <signal.h>
#include <gtk/gtk.h>
#include <cstring>
#include <boost/lexical_cast.hpp>

const int ISO_PACKET_SIZE = 64;
const int ENCODERS_NUMBER = 32;
bool doExit = false;

// Pointer to adjustment.
static GtkAdjustment *adjustment[ENCODERS_NUMBER];

// Handle to my device.
static libusb_device_handle *devh = NULL;

static void LIBUSB_CALL cb_xfr (libusb_transfer *xfr);

typedef uint8_t Buffer[ISO_PACKET_SIZE];

// Bufor na dane przychodzące z USB. Z niego będziemy kopiować do buf
Buffer inputBuf;

// Bufor dla danych wyjściowych do prezentacji na ekranie.
std::vector <uint8_t> outputBuf (ISO_PACKET_SIZE);

//const int DB_BUFFER_INITIAL_SIZE = ISO_PACKET_SIZE * 1024;
std::vector <uint8_t> dbBuffer;

// Głowne okno;
GObject *window = NULL;

/*--------------------------------------------------------------------------*/

void printDevs (libusb_device **devs)
{
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
}

/*--------------------------------------------------------------------------*/

void initUsb ()
{
        libusb_device **devs;
        ssize_t cnt;

        int r = libusb_init(NULL);

        if (r < 0) {
                std::cerr << "Error libusb_init!" << std::endl;
                return;
        }

        cnt = libusb_get_device_list(NULL, &devs);

        if (cnt < 0) {
                std::cerr << "Error retrieving usbList cnt=" << cnt
                                << std::endl;
                libusb_exit (NULL);
                exit(1);
        }

        printDevs(devs);
        libusb_free_device_list(devs, 1);

        devh = libusb_open_device_with_vid_pid(NULL, 0x20a0, 0x41ff);

        if (!devh) {
                std::cerr << "Error finding USB device" << std::endl;
                libusb_exit (NULL);
                exit(1);
        } else {
                std::cerr << "USB device has been found." << std::endl;
        }

        int rc = libusb_set_auto_detach_kernel_driver (devh, 1);

        if (rc == LIBUSB_SUCCESS) {
                std::cerr << "libusb_set_auto_detach_kernel_driver OK" << std::endl;
        }
        else {
                std::cerr << "libusb_set_auto_detach_kernel_driver Failed!" << std::endl;
        }

        rc = libusb_claim_interface(devh, 0);

        if (rc < 0) {
                std::cerr << "Error claiming interface : " << libusb_error_name(rc) << std::endl;
                libusb_exit (NULL);
                exit(1);
        } else {
                std::cerr << "Interface claimed." << std::endl;
        }

        if ((rc = libusb_set_interface_alt_setting(devh, 0, 0)) != 0) {
                std::cerr << "Error libusb_set_interface_alt_setting rc = " << rc << std::endl;
                libusb_exit (NULL);
                exit(1);
        } else {
                std::cerr << "libusb_set_interface_alt_setting OK" << std::endl;
        }

        /*--------------------------------------------------------------------------*/

        int num_iso_pack = 1; // ?!?!? CO to jest!
        libusb_transfer *xfr = libusb_alloc_transfer(num_iso_pack);

        if (!xfr) {
                std::cerr << "Error" << std::endl;
                libusb_exit (NULL);
                exit(1);
        }

//        libusb_fill_iso_transfer (xfr, devh, 0x81, buf, ISO_PACKET_SIZE, num_iso_pack, cb_xfr, NULL, 1000);
//        libusb_set_iso_packet_lengths (xfr, ISO_PACKET_SIZE / num_iso_pack);

        libusb_fill_interrupt_transfer(xfr, devh, 0x81, inputBuf, sizeof(inputBuf), cb_xfr, NULL, 100);

        int ret = libusb_submit_transfer(xfr);
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
        } else {
                std::cerr << "libusb_submit_transfer OK" << std::endl;
        }
}

void closeUsb ()
{
        if (!libusb_release_interface(devh, 0)) {
                std::cerr << "Interface released." << std::endl;
        } else {
                std::cerr << "Error releasing interface." << std::endl;
        }

        if (devh) {
                libusb_close(devh);
                std::cerr << "Device closed" << std::endl;
        }

        libusb_exit (NULL);
}

void usbThread ()
{
        initUsb();

        while (true) {
                int rc = libusb_handle_events(NULL);

                if (rc != LIBUSB_SUCCESS) {
                        std::cerr << "Problem with handling event from the USB!" << std::endl;
                        break;
                }
        }
}

//void playThread ()
//{
//        size_t numberOfFrames = dbBuffer.size () / ISO_PACKET_SIZE;
//
//        std::cerr << "Play buffer here. I have exactly # frames : " << numberOfFrames << std::endl;
//
//        auto it = dbBuffer.begin ();
//
//        for (size_t i = 0; i < numberOfFrames; ++i) {
//                {
//                        std::lock_guard < std::mutex > guard (bufferMutex);
//                        auto i = outputBuf.begin ();
//                        std::copy (it, it + ISO_PACKET_SIZE, i);
////                        std::cerr << (int)*it << std::endl;
//                        it += ISO_PACKET_SIZE;
//                }
//                usleep (10 * 1000);
//        }
//}

//gboolean guiThread (gpointer user_data)
//{
//        {
//                std::lock_guard < std::mutex > guard(bufferMutex);
//
//                for (int i = 0; i < ENCODERS_NUMBER; ++i) {
//                        int bufPos = i * 2;
//                        int16_t val =
//                                        static_cast<int16_t>(outputBuf[bufPos]
//                                                        | (static_cast<uint16_t>(outputBuf[bufPos
//                                                                        + 1])
//                                                                        << 8));
//                        gtk_adjustment_set_value(adjustment[i], val);
//                }
//
////                        printf ("[");
////                        for (unsigned int i = 0; i < ISO_PACKET_SIZE; ++i) {
////                                printf("%02x", buf[i]);
////                        }
////                        printf("]%d %d\n", buf[16], buf[17]);
//        }
//
////                for (int i = 0; i < ENCODERS_NUMBER; ++i) {
////                        gtk_adjustment_set_value (adjustment, buffer[0]);
////                }
//
//        return G_SOURCE_CONTINUE;
//}

static void /*LIBUSB_CALL*/ cb_xfr (libusb_transfer *xfr)
{
//        if (!usbRunning.load ()) {
//                if (libusb_submit_transfer(xfr) < 0) {
//                        fprintf(stderr, "error re-submitting URB\n");
//                        exit(1);
//                }
//
//                return;
//        }

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

#if 0
                printf("pack%u length:%u, actual_length:%u\n", i, pack->length, pack->actual_length);

                printf ("[");
                for (unsigned int i = 0; i < pack->actual_length; ++i) {
                        printf("%02x", buffer[i]);
                }
                printf("]\n");
#endif
        }

#if 0
        printf("length:%u, actual_length:%u\n", xfr->length, xfr->actual_length);

        if (xfr->actual_length == ISO_PACKET_SIZE) {
                for (i = 0; i < xfr->actual_length; ++i) {
                        printf ("%02x", xfr->buffer[i]);
                        if ((i+1) % 16 == 0) {
                                printf (" ");
                        }
                }
        }
        else {
                for (i = 0; i < xfr->actual_length; ++i) {
                        printf ("%02x", xfr->buffer[i]);
                }
        }

        printf ("\n");

//        std::cerr << "Length received : " << xfr->actual_length << std::endl;
#endif

        if (xfr->actual_length == ISO_PACKET_SIZE) {
                {
//                        std::lock_guard < std::mutex > guard (bufferMutex);
//                        // memcpy(outputBuf, xfr->buffer, ISO_PACKET_SIZE);
//                        auto i = outputBuf.begin ();
//                        std::copy (xfr->buffer, xfr->buffer + ISO_PACKET_SIZE, i);
                }

                {
//                        std::lock_guard < std::mutex > guard(dbBufferMutex);
//                        std::copy (xfr->buffer, xfr->buffer + ISO_PACKET_SIZE, std::back_inserter (dbBuffer));
                }
        }

        if (libusb_submit_transfer(xfr) < 0) {
                fprintf(stderr, "error re-submitting URB\n");
                exit(1);
        }
}

void zeroButtonClicked (GtkButton *button, gpointer user_data)
{
        int ret = libusb_control_transfer(devh,
                        LIBUSB_RECIPIENT_INTERFACE | LIBUSB_REQUEST_TYPE_VENDOR,
                        0x00, 0x00, 0x00, NULL, 0x00, 500);

        if (ret >= 0) {
                return;
        }

        switch (ret) {
        case LIBUSB_ERROR_TIMEOUT:
                std::cerr << "Timeout reached when setting HUB to zero" << std::endl;
                break;

        case LIBUSB_ERROR_PIPE:
                std::cerr << "The control request was not supported by the device" << std::endl;
                break;

        case LIBUSB_ERROR_NO_DEVICE:
                std::cerr << "The device has been disconnected" << std::endl;
                break;

        default:
                std::cerr << "Undefined error : " << ret << std::endl;
                break;
        }
}

void popupError (std::string const &str)
{

                GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW (window),
                                                 GTK_DIALOG_DESTROY_WITH_PARENT,
                                                 GTK_MESSAGE_ERROR,
                                                 GTK_BUTTONS_CLOSE,
                                                 str.c_str ());

                gtk_dialog_run (GTK_DIALOG (dialog));
                gtk_widget_destroy (dialog);

                return;

}

//void saveButtonClicked (GtkButton *button, gpointer user_data)
//{
//        if (usbRunning.load ()) {
//                popupError ("Wyłącz USB najpierew!");
//        }
//
//        pqxx::work tx (dbConnection);
//        std::lock_guard < std::mutex > guard(dbBufferMutex);
//        tx.exec ("INSERT INTO examination (data) VALUES ('" + tx.esc_raw(dbBuffer.data (), dbBuffer.size ()) + "')");
//        tx.commit ();
//
//        dbBuffer.clear ();
//}

//void playButtonClicked (GtkButton *button, gpointer user_data)
//{
//        if (usbRunning.load ()) {
//                popupError ("Wyłącz USB najpierew!");
//        }
//
//        pqxx::work tx (dbConnection);
//
//        pqxx::result r = tx.exec("SELECT max (id) from examination");
//
//        if (r.size () != 1) {
//                popupError ("Nie udało sie counta zrobić!");
//                return;
//        }
//
//        int examinationId;
//
//        try {
//                examinationId = r[0][0].as<int> ();
//        }
//        catch (pqxx::conversion_error const &) {
//                popupError ("Nie ma nic w bazie!!!!!");
//                return;
//        }
//
//        r = tx.exec("SELECT data from examination where id = " + tx.quote(examinationId));
//
//        if (r.size () != 1) {
//                popupError ("Nie udało sie pobrać danych!");
//                return;
//        }
//
//        pqxx::binarystring blob(r[0][0]);
//        uint8_t const *ptr = static_cast <uint8_t const *> (blob.data());
//        size_t len = blob.size();
//
//        std::cout << "Size od data retrieved " << len << std::endl;
//
//        dbBuffer.clear ();
//        std::copy (ptr, ptr + len, std::back_inserter (dbBuffer));
//
//        std::thread t(playThread);
//        t.detach();
//}

void usbButtonToggled (GtkToggleButton *button, gpointer user_data)
{
        bool state = gtk_toggle_button_get_active (button);
//        usbRunning.store (state);
//        std::cerr << "USB state : " << usbRunning.load () << std::endl;
}

/**
 *
 */
int main (int argc, char **argv)
{
        GtkBuilder *builder;
        GObject *quitButton;

        gtk_init(&argc, &argv);

        /* Construct a GtkBuilder instance and load our UI description */
        builder = gtk_builder_new();
        gtk_builder_add_from_file(builder, "keyboard.ui", NULL);

        /* Connect signal handlers to the constructed widgets. */
        window = gtk_builder_get_object(builder, "window");
        g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

//        quitButton = gtk_builder_get_object(builder, "quit");
//        g_signal_connect(quitButton, "clicked", G_CALLBACK(gtk_main_quit), NULL);
//
//        GObject *zeroButton = gtk_builder_get_object(builder, "zero");
//        g_signal_connect(zeroButton, "clicked", G_CALLBACK(zeroButtonClicked), NULL);
//
//        GObject *saveButton = gtk_builder_get_object(builder, "save");
//        g_signal_connect(saveButton, "clicked", G_CALLBACK(saveButtonClicked), NULL);
//
//        GObject *playButton = gtk_builder_get_object(builder, "play");
//        g_signal_connect(playButton, "clicked", G_CALLBACK(playButtonClicked), NULL);
//
//        GObject *usbCheckbox = gtk_builder_get_object(builder, "usbRunning");
//        g_signal_connect(usbCheckbox, "toggled", G_CALLBACK(usbButtonToggled), NULL);

        //        GtkScale *scale = GTK_SCALE (gtk_builder_get_object (builder, "scale1"));
//        for (int i = 0; i < ENCODERS_NUMBER; ++i) {
//                adjustment[i] = GTK_ADJUSTMENT(gtk_builder_get_object(builder, (std::string("adjustment") + boost::lexical_cast <std::string> (i + 1)).c_str()));
//        }

//        std::thread t(usbThread);
//        t.detach();
//
//        g_idle_add(guiThread, NULL);
        initUsb();
        zeroButtonClicked (NULL, 0);
        gtk_widget_show_all (GTK_WIDGET (window));
        gtk_main();
        closeUsb();

        return 0;
}
