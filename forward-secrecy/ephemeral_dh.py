#!/usr/env python2.7

# An example of a mutually-authenticated ephemeral Diffie-Hellman protocol.
# Requires pycrypto library.

# Refer to Figure 9.22 on pg. 329 of Information Security: Principles and
# Practice, 2nd ed., by Mark Stamp.

from crypto import *
import struct

def main():

    # Create two users, alice will be connecting to bob
    alice = user()
    bob   = user()

    # Assume alice and bob already know each other's public key
    alice.set_other_pubkey(bob.rsa.pubkey())
    bob.set_other_pubkey(alice.rsa.pubkey())

    # Alice sends to Bob:
    # nonce1
    tx1 = alice.send_message1()
    bob.recv_message1(tx1)

    # Bob sends to Alice:
    # nonce2 + [ { nonce1 # g^b mod n }_A ]_B
    tx2 = bob.send_message2()
    alice.recv_message2(tx2)

    # Alice sends to Bob:
    # [ { nonce2 @ g^a mod n }_B ]_A
    tx3 = alice.send_message3()
    bob.recv_message3(tx3)

    # Now they have agreed on a key
    print("alice key: %s" % hexify(alice.get_key()))
    print("bob key:   %s" % hexify(bob.get_key()))

# Shortcuts to encode/decode a 64-bit number to/from a byte string
def pack(x):
    return struct.pack("!Q", x)
def unpack(x):
    return struct.unpack("!Q", x)[0]

# Models one user's session
class user:
    
    # Each user has a name, a Diffie Hellman session and an RSA keypair
    def __init__(self):
        self.dh   = diffie_hellman()
        self.rsa  = rsa()

    # First message (client -> server):
    # - Client generates 1st nonce
    def send_message1(self):
        self.nonce = rand_uint64()
        return pack(self.nonce)
        
    # On receipt by server:
    # - Just remember the 1st nonce
    def recv_message1(self, msg):
        self.other_nonce = unpack(msg)

    # Second message (server -> client):
    # - Server generates DH share and 2nd nonce
    # - DH share is concatenated to the 1st nonce
    # - Concatenated data is encrypted
    # - Encrypted data is then signed
    # - Signed data is then concatenated to the 2nd nonce
    def send_message2(self):
        self.nonce    = rand_uint64()
        self.dh_share = self.dh.step1()
        plaintext     = pack(self.other_nonce) + self.dh_share
        ciphertext    = self.other_pubkey.encrypt(plaintext)
        signed        = self.rsa.sign(ciphertext)
        return pack(self.nonce) + signed
      
    # On receipt by client:
    # - Signature is verified
    # - 1st nonce is checked  
    # - Client derives the ephemeral key
    def recv_message2(self, msg):
        self.other_nonce   = unpack(msg[:8])
        signed             = msg[8:]
        ciphertext, valid  = self.other_pubkey.verify(signed)
        plaintext          = self.rsa.decrypt(ciphertext)
        rx_nonce           = unpack(plaintext[:8])
        self.recv_dh_share = plaintext[8:]
        self.dh_share      = self.dh.step2(self.recv_dh_share)
        assert(valid and rx_nonce == self.nonce)
    
    # Third message (client -> server):
    # - DH share is concatenated to the 2nd nonce
    # - Concatenated data is encrypted, then signed
    def send_message3(self):
        plaintext  = pack(self.other_nonce) + self.dh_share
        ciphertext = self.other_pubkey.encrypt(plaintext)
        signed     = self.rsa.sign(ciphertext)
        return signed
    
    # On receipt by server:
    # - Signature is verified
    # - 2nd nonce is checked
    # - Server derives the ephemeral key
    def recv_message3(self, msg):
        signed             = msg
        ciphertext, valid  = self.other_pubkey.verify(signed)
        plaintext          = self.rsa.decrypt(ciphertext)
        rx_nonce           = unpack(plaintext[:8])
        self.recv_dh_share = plaintext[8:]
        self.dh.step3(self.recv_dh_share)
        assert(valid and rx_nonce == self.nonce)
    
    # Helper method for "exchanging" public keys
    def set_other_pubkey(self, pubkey):
        self.other_pubkey = rsa(pubkey)
    
    # Shortcut to obtain the key from the DH module
    def get_key(self):
        return self.dh.key()

if __name__ == "__main__":
    main()
