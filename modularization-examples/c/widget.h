/* This is a module header file. It contains the interface to the module; i.e.,
   It defines the constants, structures, and methods of the module, but does not
   actually provide method implementations.
*/

/* Headers should be wrapped in the "ifndef-define-endif" idiom to prevent
   the same file from being included multiple times in compilation
*/
#ifndef widget_h
#define widget_h

#define WIDGET_MAX_STR 3

typedef struct widget_s {
    char foo[WIDGET_MAX_STR+1];
    char bar[WIDGET_MAX_STR+1];
} * widget;

widget widget_create(const char *a, const char *b);
void widget_do_thing(widget w);
void widget_destroy(widget w);

#endif
