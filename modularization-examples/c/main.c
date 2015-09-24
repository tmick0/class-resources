/* This is the main program file. It should not actually implement any
   algorithms or data structures, but rather tie them together or provide
   a user-interface to them.
*/

/* Include the header files for the modules you need to use */
#include "widget.h"

int main(int argc, char** argv){
    widget w = widget_create("foo", "bar");
    widget_do_thing(w);
    widget_destroy(w);
    return 0;
}
