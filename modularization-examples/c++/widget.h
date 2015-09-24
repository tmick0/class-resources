/* This is a module header file. It contains the interface to the module; i.e.,
   It defines the class but does not actually provide its implementation.
*/


/* Headers should be wrapped in the "ifndef-define-endif" idiom to prevent
   the same file from being included multiple times in compilation
*/
#ifndef widget_h
#define widget_h

/* If you reference types defined in other modules, you must include their
   headers.
*/
#include <string>

class widget {

    private:
    std::string foo, bar;
    
    public:
    widget(std::string a, std::string b);
    void do_thing();

};

#endif
