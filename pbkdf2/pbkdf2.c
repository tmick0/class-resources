#include "pbkdf2.h"
#include "hmac.h"
#include "io.h"

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

void block_xor(const char *a, const char *b, char *c, unsigned len){
    int i;
    for(i = 0; i < len; i++){
        c[i] = a[i] ^ b[i];
    }
}

void pbkdf2_hmac_sha256_block(const char *key, unsigned klen, const char *salt, unsigned slen, unsigned c, unsigned i, char *result, unsigned trunc){
    char a[HMAC_SHA256_LEN], b[HMAC_SHA256_LEN], x[HMAC_SHA256_LEN];
    int j;
    uint32_t tmp = htonl(i + 1);
        
    bzero(a, HMAC_SHA256_LEN);
    memcpy(a, salt, slen);
    memcpy(a + slen, &tmp, sizeof(uint32_t));
    
    hmac_sha256(key, klen, a, slen + sizeof(uint32_t), b);
    memcpy(x, b, HMAC_SHA256_LEN);
    memcpy(a, b, HMAC_SHA256_LEN);
    
    printf("    u1 = ");
    hexprint(b, HMAC_SHA256_LEN);
    
    for(j = 1; j < c; j++){
        hmac_sha256(key, klen, a, HMAC_SHA256_LEN, b);
        printf("    u%u = ", j+1);
        hexprint(b, HMAC_SHA256_LEN);
        block_xor(b, x, x, HMAC_SHA256_LEN);
        memcpy(a, b, HMAC_SHA256_LEN);
    }
    
    memcpy(result, x, (HMAC_SHA256_LEN <= trunc) ? HMAC_SHA256_LEN : trunc);
    
}

void pbkdf2_hmac_sha256(const char *key, unsigned klen, const char *salt, unsigned slen, unsigned c, unsigned dkLen, char *result){
    unsigned i;
    unsigned blocks = ceil((float) dkLen / HMAC_SHA256_LEN);
    
    printf("K = ");
    hexprint(key, klen);
    
    printf("S = ");
    hexprint(salt, slen);
    
    printf("c = %u\n", c);
    printf("dkLen = %u\n", dkLen);
    
    bzero(result, dkLen);
    
    for(i = 0; i < blocks; i++){
    
        printf("  i = %u\n", i);
        pbkdf2_hmac_sha256_block(key, klen, salt, slen, c, i, result + i * HMAC_SHA256_LEN, dkLen - i * HMAC_SHA256_LEN);
        
        printf("  block = ");
        hexprint(result, HMAC_SHA256_LEN);
        
    }
}
