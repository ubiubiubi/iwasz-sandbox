/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>

class MyClass {
public:

        void incPrint ()
        {
                std::lock_guard <std::mutex> lock {m};
                std::cerr << std::this_thread::get_id () << ", i = " << i++ << std::endl;
        }

private:

        unsigned int i = 0;
        std::mutex m;
};

struct Task {

        void run (MyClass &o)
        {
                while (true) {
                        o.incPrint ();
                        usleep (500000);
                }
        }

};

int main ()
{
        MyClass object;
        Task a, b;
        std::thread t {&Task::run, &a, std::ref (object)};
        std::thread u {&Task::run, &b, std::ref (object)};
        t.join ();
        u.join ();
}
