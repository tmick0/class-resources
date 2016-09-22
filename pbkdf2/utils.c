#include "utils.h"

/** block_xor:
    Performs xor between two character arrays and stores the result in a third
    array. It is safe for c to be equal to either a or b.
    
    Params:
    - a: input array
    - b: input array
    - c: output array
    - len: length of each
    
    Return:
    - Nothing
    
*/
void block_xor(const char *a, const char *b, char *c, uint32_t len){
    int i;
    for(i = 0; i < len; i++){
        c[i] = a[i] ^ b[i];
    }
}
