#include "AESDecryptor.h"
using namespace std;

void AESDecryptor::Decrypt (unsigned char *payload, unsigned int payload_len, unsigned char *key, unsigned int keylen) {
	HCRYPTPROV hProv;
	HCRYPTHASH hHash;
	HCRYPTKEY hKey;
    CryptAcquireContextW(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT);
    CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash);
	CryptHashData(hHash, (BYTE*) key, (DWORD) keylen, 0);
    CryptDeriveKey(hProv, CALG_AES_256, hHash, 0,&hKey);
    if (!CryptDecrypt(hKey, (HCRYPTHASH) NULL, 0, 0, (BYTE *) payload, (DWORD *) &payload_len)){
        cout << "Failed Decrypting!" << endl;
        cout << GetLastError() << endl;
    }
    else 
        cout << "Decrypted Successfuly" << endl;

	CryptReleaseContext(hProv, 0);
	CryptDestroyHash(hHash);
	CryptDestroyKey(hKey);
}