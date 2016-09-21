#include "hmac.h"
#include <openssl/hmac.h>
#include <string.h>

void hmac_sha256(const char *key, unsigned klen, const char *msg, unsigned mlen, char *res){
    unsigned int len;
    HMAC_CTX ctx;
    HMAC_CTX_init(&ctx);
    HMAC_Init_ex(&ctx, (const unsigned char *) key, klen, EVP_sha256(), NULL);
    HMAC_Update(&ctx,(const unsigned char *) msg, mlen);
    HMAC_Final(&ctx, (unsigned char *) res, &len);
    HMAC_CTX_cleanup(&ctx);
}
