/****************************************************************************
 *                                                                          *
 *  Author : lukasz.iwaszkiewicz@gmail.com                                  *
 *  ~~~~~~~~                                                                *
 *  License : see COPYING file for details.                                 *
 *  ~~~~~~~~~                                                               *
 ****************************************************************************/

#ifndef ANY_KEY_EXCEPTION_H_
#define ANY_KEY_EXCEPTION_H_

#include <exception>
#include <string>

class Exception : public std::exception {
public:

        Exception (std::string const &s) : message (s) {}
        virtual ~Exception () {}
        virtual const char* what() const noexcept { return message.c_str (); }

private:

        std::string message;
};

#endif /* EXCEPTION_H_ */
