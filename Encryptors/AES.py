import os
import sys
import hashlib
from Cryptodome import Random
from Cryptodome.Cipher import AES
from Cryptodome.Util.Padding import pad
from Cryptodome.Random import get_random_bytes

pure_key = get_random_bytes(16)
key = hashlib.sha256(pure_key).digest()
iv = 16 * b'\x00'
cipher = AES.new(key, AES.MODE_CBC, iv)

if os.path.isfile(sys.argv[1]):
    # open text file in read mode
    with open(sys.argv[1], "rb") as shell_file:
        # read whole file to a string
        pure_shell = shell_file.read()
        ciphertext = cipher.encrypt(pad(pure_shell, AES.block_size))
        converted_key = 'unsigned char key[] = { 0x' + ', 0x'.join(hex(x)[2:] for x in pure_key) + ' };'
        encrypted_payload = 'unsigned char payload[] = { 0x' + ', 0x'.join(hex(x)[2:] for x in ciphertext) + ' };'

    with open("keys.txt", 'w') as keyFile:
        keyFile.writelines([converted_key, "\n",  encrypted_payload])

else:
    print("--- FAILURE --- ERROR READING PROVIDED FILE FOR ENCRYPTION --- FAILURE --- ")
    exit(0)