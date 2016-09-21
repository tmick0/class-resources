#include "io.h"
#include <stdio.h>

void hexprint(const char *v, size_t len){
    int i;
    for(i = 0; i < len; i++){
        printf("%02x ", (unsigned char) v[i]);
    }
    printf("\n");
}
