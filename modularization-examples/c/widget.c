/* This is a module implementation file. It contains the implementation of
   the methods given in the module's header
*/

/* Must include the interface header, as well as the headers of any other
   modules which are used in this implementation.
*/
#include "widget.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Implement all of the methods named in the header */

widget widget_create(const char *a, const char *b){ 

    widget w = (widget) malloc(sizeof(struct widget_s));
    
    strncpy(w->foo, a, WIDGET_MAX_STR);
    strncpy(w->bar, b, WIDGET_MAX_STR);
    
    w->foo[WIDGET_MAX_STR] = '\0';
    w->bar[WIDGET_MAX_STR] = '\0';
    
    return w;
}

void widget_do_thing(widget w){
    printf("%s %s\n", w->foo, w->bar);
}

void widget_destroy(widget w){
    free(w);
}
