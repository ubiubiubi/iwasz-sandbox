/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef GUI_H_
#define GUI_H_

#include "UsbService.h"

class Gui {
public:
        Gui ();
        virtual ~Gui ();
        void init (int argc, char **argv);

private:

        struct Impl;
        Impl *impl;
        UsbServiceGuard <UsbService> service;
};

#endif /* GUI_H_ */
