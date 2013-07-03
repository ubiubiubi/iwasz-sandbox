/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <iostream>
#include <thread>

void hello ()
{
        std::cout << "Hello Concurrent World\n";
}

int main ()
{
        std::thread t (hello);
        t.join ();
}
