#include "hmac.h"
#include <openssl/hmac.h>
#include <string.h>

void hmac_sha256(const char *key, uint32_t klen, const char *msg, uint32_t mlen, char *res){
    uint32_t len;
    HMAC_CTX ctx;
    HMAC_CTX_init(&ctx);
    HMAC_Init_ex(&ctx, (const unsigned char *) key, klen, EVP_sha256(), NULL);
    HMAC_Update(&ctx,(const unsigned char *) msg, mlen);
    HMAC_Final(&ctx, (unsigned char *) res, &len);
    HMAC_CTX_cleanup(&ctx);
}

