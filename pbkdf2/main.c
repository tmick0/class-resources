#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pbkdf2.h"
#include "io.h"
 
int main(int argc, char **argv){
    unsigned c = atoi(argv[3]);
    unsigned dklen = atoi(argv[4]);
    
    char *res = malloc(dklen);
    
    pbkdf2_hmac_sha256(argv[1], strlen(argv[1]), argv[2], strlen(argv[2]), c, dklen, res);
    hexprint(res, dklen);
    
    free(res);
    return 0;
}
