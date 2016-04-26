import os, struct
from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP
from Crypto.Signature import PKCS1_PSS
from Crypto.Hash import SHA256

# Generates a random 64-bit integer
def rand_uint64():
    return struct.unpack("@Q", os.urandom(8))[0]

# Converts a byte string to a hexadecimal representation
def hexify(s):
    return " ".join("%02x" % ord(x) for x in s)

# Models a Diffie-Hellman key exchange session
class diffie_hellman:
    """
    WARNING:
    Never implement your own crypto in real life. I only did this here because
    I couldn't find an accessible python lib for DH or ECDH.
    """

    def __init__(self):
        pass
    
    def step1(self):
        self._p = rand_uint64()
        self._n = rand_uint64()
        self._g = rand_uint64()
        share = pow(self._g, self._p, self._n)
        return struct.pack("!QQQ", self._n, self._g, share)
        
    def step2(self, pkt):
        self._p = rand_uint64()
        self._n, self._g, other = struct.unpack("!QQQ", pkt)
        self._key = pow(other, self._p, self._n)
        share = pow(self._g, self._p, self._n)
        return struct.pack("!Q", share)
    
    def step3(self, pkt):
        other, = struct.unpack("!Q", pkt)
        self._key = pow(other, self._p, self._n)
    
    def key(self):
        return struct.pack("@Q", self._key)
    
# Helper for some RSA-related operations
class rsa:

    # Loads the key k if provided, or otherwise generates a new one
    def __init__(self, k=None):
        if k is None:
            self._key = RSA.generate(1024, randfunc=os.urandom)
        else:
            self._key = k

    # Returns the public part of this key    
    def pubkey(self):
        return self._key.publickey()
            
    # Encrypts a message
    def encrypt(self, msg):
        return PKCS1_OAEP.new(self._key, SHA256).encrypt(msg)
    
    # Decrypts a message
    def decrypt(self, msg):
        return PKCS1_OAEP.new(self._key, SHA256).decrypt(msg)
    
    # Signs a message, returning a packed struct which verify() can understand
    def sign(self, msg):
        h = SHA256.new()
        h.update(msg)
        sig = PKCS1_PSS.new(self._key).sign(h)
        return struct.pack("!Q", len(sig)) + sig + msg
    
    # Verifies a signature, returns the tuple (message, verified) where verified
    # is True or False
    def verify(self, msg):
        siglen, = struct.unpack("!Q", msg[:8])
        sig = msg[8:8+siglen]
        msg = msg[8+siglen:]
        h = SHA256.new()
        h.update(msg)
        ver = PKCS1_PSS.new(self._key).verify(h, sig)
        return msg, ver
        
