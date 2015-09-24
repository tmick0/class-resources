/* This is a module implementation file. It contains the implementation of
   the methods given in the module's header
*/

/* Must include the interface header, as well as the headers of any other
   modules which are used in this implementation.
*/
#include "widget.h"
#include <iostream>

/* Implement all of the methods named in the class */

widget::widget(std::string a, std::string b){
    this->foo = a;
    this->bar = b;
}

void widget::do_thing(){
    std::cout << this->foo << " " << this->bar << std::endl;
}
