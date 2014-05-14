/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "Gui.h"
#include "Constants.h"
#include <gtk/gtk.h>

struct Gui::Impl {
        GtkWindow *window = nullptr;
        GtkBuilder *builder = nullptr;
};

/****************************************************************************/

Gui::Gui ()
{
        impl = new Impl;
}

/****************************************************************************/

Gui::~Gui ()
{
        delete impl;
}

/****************************************************************************/

void Gui::init (int argc, char **argv)
{
        auto &service = guard.service;
        service.transmitConfiguration ({1, 2, 3, 4, 5, 6, 7, 8, 9});
        service.receiveConfiguration ();
//        service.loop ();
        return;

        GObject *quitButton;

        gtk_init (&argc, &argv);

        /* Construct a GtkBuilder instance and load our UI description */
        impl->builder = gtk_builder_new();
        gtk_builder_add_from_file (impl->builder, "keyboard.ui", NULL);

        /* Connect signal handlers to the constructed widgets. */
        impl->window = GTK_WINDOW (gtk_builder_get_object(impl->builder, "window"));
        g_signal_connect(impl->window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

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
//        initUsb();
//        zeroButtonClicked (NULL, 0);
        gtk_widget_show_all (GTK_WIDGET (impl->window));
        gtk_main();
//        closeUsb();
}
