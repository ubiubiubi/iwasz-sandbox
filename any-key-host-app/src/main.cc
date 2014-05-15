/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include "Gui.h"
#include <exception>
#include <iostream>

/**
 *
 */
int main (int argc, char **argv)
{
        try {
                Gui gui;
                gui.init (argc, argv);
        }
        catch (std::exception const &e) {
                std::cerr << "An exception has occured in ~UsbServiceGuard. Message : " << e.what () << std::endl;
        }
        catch (...) {
                std::cerr << "Unknown exception has occured in ~UsbServiceGuard." << std::endl;
        }

        return 0;
}
