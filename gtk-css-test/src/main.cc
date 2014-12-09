/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <iostream>
#include <gtk/gtk.h>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <string>
#include <vector>

namespace po = boost::program_options;
using namespace std;

// Głowne okno;
GObject *window = NULL;

void guiLoadTheme (const char *directory, const char *theme_name, GObject *toplevel)
{
        GtkCssProvider *css_provider;
        GError *error = NULL;
        char buf[strlen(directory) + strlen(theme_name) + 32];
        /* Gtk theme is a directory containing gtk-3.0/gtkrc file */
        snprintf(buf, sizeof(buf), "%s/%s/gtk-3.0/gtk.css", directory, theme_name);
        css_provider = gtk_css_provider_new();
        gtk_css_provider_load_from_file(css_provider, g_file_new_for_path(buf), &error);

        if (error) {
                g_warning("%s\n", error->message);
                return;
        }

//        gtk_style_context_remove_provider_for_screen (gtk_widget_get_screen (GTK_WIDGET (toplevel)), NULL);

//        gtk_style_context_add_provider_for_screen(
//                        gtk_widget_get_screen (GTK_WIDGET (toplevel)),
//                        GTK_STYLE_PROVIDER(css_provider),
//                        GTK_STYLE_PROVIDER_PRIORITY_USER + 100);


        GdkDisplay *display = gdk_display_get_default();
        GdkScreen *screen = gdk_display_get_default_screen(display);

        gtk_style_context_add_provider_for_screen(screen,
                        GTK_STYLE_PROVIDER(css_provider),
                        GTK_STYLE_PROVIDER_PRIORITY_USER);

        g_object_unref (css_provider);
}

/*--------------------------------------------------------------------------*/
//globalsy globalsy

/*--------------------------------------------------------------------------*/

/**
 *
 */
int main (int argc, char **argv)
{
        // Declare the supported options.
        po::options_description desc ("Opcje:");
        desc.add_options()
                ("help,h", "produce help message")
                ("dir,d", po::value <string> ()->default_value ("themes"), "Katalog w którym znajdują się skórki")
                ("theme,t", po::value <string> ()->default_value ("Flattastic-Blue"), "Nazwa skórki (to jest nazwa katalogu w tym katalogu wyżej).")
                ("uifile,u", po::value <string> ()->default_value ("forrest.ui"), "Nazwa pliku UI do załadowania.");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            cout << desc << "\n";
            return 1;
        }

        GtkBuilder *builder;
        gtk_init(&argc, &argv);

        /* Construct a GtkBuilder instance and load our UI description */
        builder = gtk_builder_new();
        gtk_builder_add_from_file(builder, vm["uifile"].as<string>().c_str (), NULL);

        /* Connect signal handlers to the constructed widgets. */
        window = gtk_builder_get_object(builder, "window");
        g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        // http://nale12.deviantart.com/art/Flattastic-11-03-2014-424913255
        guiLoadTheme (vm["dir"].as<string> ().c_str (), vm["theme"].as<string> ().c_str (), window);

        gtk_main();

        return 0;
}
