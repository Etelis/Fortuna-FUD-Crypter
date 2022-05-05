//
// Created by Eteli on 08/03/2022.
//
#include "../Injector.h"
#ifndef UNTITLED_REMOTETHREADSUSPENDEDINJECTION_H
#define UNTITLED_REMOTETHREADSUSPENDEDINJECTION_H


class RemoteThreadSuspendedInjection: public Injector{
    char *procName_;
public:
    void Inject() override;
    RemoteThreadSuspendedInjection(unsigned char* key, unsigned char *payload, Decryptor *dec, char *procName): Injector(key,payload,dec), procName_(procName) {}
};


#endif //UNTITLED_REMOTETHREADSUSPENDEDINJECTION_H
