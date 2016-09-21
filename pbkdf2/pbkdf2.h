#ifndef pbkdf2_h
#define pbkdf2_h

void pbkdf2_hmac_sha256(const char *key, unsigned klen, const char *salt, unsigned slen, unsigned c, unsigned dkLen, char *result);

#endif
