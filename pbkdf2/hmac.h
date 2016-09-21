#ifndef hmac_h
#define hmac_h

#define HMAC_SHA256_LEN (32)

void hmac_sha256(const char *key, unsigned klen, const char *msg, unsigned mlen, char *res);

#endif
