#ifndef FORTUNA_V1_0_PY_AESDECRYPTOR_H
#define FORTUNA_V1_0_PY_AESDECRYPTOR_H
#include "../Injector.h"

class AESDecryptor: public Decryptor{
public:
    void Decrypt (unsigned char * payload, unsigned int payload_len, unsigned  char * key, unsigned int keylen) override;
};
#endif //FORTUNA_V1_0_PY_AESDECRYPTOR_H
