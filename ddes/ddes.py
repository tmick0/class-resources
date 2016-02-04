from Crypto.Cipher import DES
import itertools

# For demonstration purposes, we will only use 18-bit keys 
enc1 = DES.new(b'\x05\x03\x01\x00\x00\x00\x00\x00', DES.MODE_ECB)
enc2 = DES.new(b'\x06\x04\x02\x00\x00\x00\x00\x00', DES.MODE_ECB)
plaintext = b'Hello world! :-)'
ciphertext = enc2.encrypt(enc1.encrypt(plaintext))

table = {}

print("Creating table for k1")
for i in itertools.count(0):
        key = b''.join([chr(i >> 8*j & 0xff) for j in range(0, 8)])
        test = DES.new(key, DES.MODE_ECB)
        table[test.encrypt(plaintext)] = key
        #The full search space is 2**64, but for this demo we only need 2**18
        #if i == 2 ** 64 - 1:
        if i == 2 ** 18 - 1:
                break

print("Searching for k2")
for i in itertools.count(0):
        key = b''.join([chr(i >> 8*j & 0xff) for j in range(0, 8)])
        test = DES.new(key, DES.MODE_ECB)
        res = test.decrypt(ciphertext)
        if res in table:
                k1 = ' '.join(['%02x' % ord(x) for x in table[res]])
                k2 = ' '.join(['%02x' % ord(x) for x in key])
                print("k1: %s\nk2: %s" % (k1, k2))
                break
        #elif i == 2 ** 64 - 1:
        elif i == 2 ** 18 - 1:
                print("Search failed")
                break
