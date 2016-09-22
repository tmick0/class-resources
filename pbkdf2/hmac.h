#ifndef hmac_h
#define hmac_h

#include <stdint.h>

typedef void (*prf_t)(const char *key, uint32_t klen, const char *msg, uint32_t mlen, char *res);

typedef struct {
    const char *name;
    prf_t       prf;
    uint32_t    len;
} prf_param_t;

#define HMAC_SHA256_LEN (32)
void hmac_sha256(const char *key, uint32_t klen, const char *msg, uint32_t mlen, char *res);
static const prf_param_t prf_hmac_sha256 = {"HMAC_SHA256", hmac_sha256, HMAC_SHA256_LEN};

#endif
