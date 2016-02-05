"""
Demonstration of the meet-in-the-middle attack on Double DES.

Explanation and some background information available at:
http://lo.calho.st/security/demonstrating-the-double-des-meet-in-the-middle-attack/

Travis Mick
New Mexico State University
CS 478 - Spring 2016
"""

from Crypto.Cipher import DES
import itertools

# For demonstration purposes, we will only use 18-bit keys 
enc1 = DES.new(b'\x05\x03\x01\x00\x00\x00\x00\x00', DES.MODE_ECB)
enc2 = DES.new(b'\x06\x04\x02\x00\x00\x00\x00\x00', DES.MODE_ECB)
plaintext = b'Hello world! :-)'
ciphertext = enc2.encrypt(enc1.encrypt(plaintext))

table = {}

print("Creating table for k1...")
for i in itertools.count(0):

        # Convert the integer i to a series of 8 bytes.
        key = b''.join([chr(i >> 8*j & 0xff) for j in range(0, 8)])
        
        # Initialize the cipher.
        test = DES.new(key, DES.MODE_ECB)
        
        # Encrypt the plaintext; this gives us the 'middle' value. We index the
        # table by this middle value, and store the corresponding key.
        table[test.encrypt(plaintext)] = key
        
        # The full search space is 2**64, but for this demo we only need 2**18.
        # if i == 2 ** 64 - 1:
        if i == 2 ** 18 - 1:
                break

print("Searching for k2...")
for i in itertools.count(0):
        
        # Convert key, initialize cipher.
        key = b''.join([chr(i >> 8*j & 0xff) for j in range(0, 8)])
        test = DES.new(key, DES.MODE_ECB)
        
        # Decrypt the ciphertext to get a middle value. Check if the middle
        # value is stored in the table. If it is, then we have found both
        # keys.
        mid = test.decrypt(ciphertext)
        if mid in table:
                k1 = ' '.join(['%02x' % ord(x) for x in table[mid]])
                k2 = ' '.join(['%02x' % ord(x) for x in key])
                print("k1: %s\nk2: %s" % (k1, k2))
                break
                
        # This should not be necessary, but we don't want an infinite loop, so
        # we will include it just in case.
        # elif i == 2 ** 64 - 1:
        elif i == 2 ** 18 - 1:
                print("Search failed.")
                break
