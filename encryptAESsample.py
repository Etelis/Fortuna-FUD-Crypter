import os
import sys
import hashlib
from Cryptodome import Random
from Cryptodome.Cipher import AES
from Cryptodome.Util.Padding import pad
from Cryptodome.Random import get_random_bytes
import binascii


sys.stdout = open('file', 'w')

pure_key = get_random_bytes(16)
key = hashlib.sha256(pure_key).digest()
iv = 16 * b'\x00'
cipher = AES.new(key, AES.MODE_CBC, iv)
pure_shell = b'hey'

print(binascii.hexlify(key).decode("ascii"))

ciphertext = cipher.encrypt(pad(pure_shell, AES.block_size))
converted_key = 'unsigned char key[] = { 0x' + ', 0x'.join(hex(x)[2:] for x in key) + ' };'

print("NOW CONVERTED KEY::::::: ----------------------------- ")

print(converted_key)
encrypted_payload = 'unsigned char payload[] = { 0x' + ', 0x'.join(hex(x)[2:] for x in ciphertext) + ' };'

print("NOW ENCRYPTED TEXT::::::---------")
print(binascii.hexlify(ciphertext))

sys.stdout.close()