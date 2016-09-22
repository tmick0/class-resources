#include "pbkdf2.h"
#include "hmac.h"
#include "utils.h"

#include <math.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <arpa/inet.h>

/** pbkdf2_block:
    Computes one block of an extended key.
    
    Params:
    - work: buffer to use as a working area, must be 3 times the PRF output length
    - prf: a pointer to the PRF structure to use for key derivation
    - key: the password or secret to use for key derivation
    - klen: the length of the key
    - salt: the salt to use for key derivation
    - slen: the length of the salt
    - c: the number of iterations to perform
    - i: the block number (starting at 0)
    - result: character array where the derived block will be stored
    - trunc: the maximum output length
    
    Return:
    - Nothing
    
*/
void pbkdf2_block(char *work, const prf_param_t *prf, const char *key, uint32_t klen, const char *salt, uint32_t slen, uint32_t c, uint32_t i, char *result, uint32_t trunc){
    char *a = work;
    char *b = work + 1 * prf->len;
    char *x = work + 2 * prf->len;
    uint32_t tmp = htonl(i + 1);
    int j;
        
    bzero(a, prf->len);
    memcpy(a, salt, slen);
    memcpy(a + slen, &tmp, sizeof(uint32_t));
    
    prf->prf(key, klen, a, slen + sizeof(uint32_t), b);
    memcpy(x, b, prf->len);
    memcpy(a, b, prf->len);
    
    for(j = 1; j < c; j++){
        prf->prf(key, klen, a, prf->len, b);
        block_xor(b, x, x, prf->len);
        memcpy(a, b, prf->len);
    }
    
    memcpy(result, x, (prf->len <= trunc) ? prf->len : trunc);
}

/** pbkdf2:
    Computes an extended key using a chosen PRF.
    
    Params:
    - prf: a pointer to the PRF structure to use for key derivation
    - key: the password or secret to use for key derivation
    - klen: the length of the key
    - salt: the salt to use for key derivation
    - slen: the length of the salt
    - c: the number of iterations to perform
    - result: character array where the derived block will be stored
    
    Return:
    - Nothing
    
*/
void pbkdf2(const prf_param_t *prf, const char *key, uint32_t klen, const char *salt, uint32_t slen, uint32_t c, uint32_t dkLen, char *result){
    uint32_t i;
    uint32_t blocks = ceil((float) dkLen / prf->len);
    char *buffer = malloc(prf->len * 3);
    
    bzero(result, dkLen);
    
    for(i = 0; i < blocks; i++){
        pbkdf2_block(buffer, prf,  key, klen, salt, slen, c, i, result + i * prf->len, dkLen - i * prf->len);
    }
    
    free(buffer);
}
