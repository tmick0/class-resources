#ifndef pbkdf2_h
#define pbkdf2_h

#include <stdint.h>
#include "hmac.h"

void pbkdf2(const prf_param_t *prf, const char *key, uint32_t klen, const char *salt, uint32_t slen, uint32_t c, uint32_t dkLen, char *result);

#endif
