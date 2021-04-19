
#include "default.h"
#include "modified.h"
#include "condensation.h"

LRESULT CALLBACK windowprocessforwindow1(HWND handleforwindow1, UINT message, WPARAM wParam, LPARAM lParam);

#define createwindowbuttoninwindow1 101
#define createwindowbuttoninwindow2 201
#define createwindowbuttoninwindow3 301

bool window1open = false;
bool windowclass1registeredbefore, windowclass2registeredbefore,
windowclass3registeredbefore, windowclass4registeredbefore = false;

typedef enum windowtoopenenumt { none, window2, window3, window4 } windowtoopenenumt;
windowtoopenenumt windowtoopenenum = none;

void defaultGraphWin(HWND *hwnd, HINSTANCE hInst, int nShowCmd);
void modifiedGraphWin(HWND *hwnd, HINSTANCE hInst, int nShowCmd);
void condensationGraphWin(HWND *hwnd, HINSTANCE hInst, int nShowCmd);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    AllocConsole();
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
    coninfo.dwSize.Y = 500;
    AttachConsole(ATTACH_PARENT_PROCESS);

    bool endprogram = false;
    HWND handleforwindow2;
    HWND handleforwindow3;
    HWND handleforwindow4;

    //create window 1
    MSG msg;
    WNDCLASSEX windowclassforwindow1;
    ZeroMemory(&windowclassforwindow1, sizeof(WNDCLASSEX));
    windowclassforwindow1.cbClsExtra = NULL;
    windowclassforwindow1.cbSize = sizeof(WNDCLASSEX);
    windowclassforwindow1.cbWndExtra = NULL;
    windowclassforwindow1.hbrBackground = (HBRUSH)COLOR_WINDOW;
    windowclassforwindow1.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowclassforwindow1.hIcon = NULL;
    windowclassforwindow1.hIconSm = NULL;
    windowclassforwindow1.hInstance = hInst;
    windowclassforwindow1.lpfnWndProc = (WNDPROC)windowprocessforwindow1;
    windowclassforwindow1.lpszClassName = L"window class 1";
    windowclassforwindow1.lpszMenuName = NULL;
    windowclassforwindow1.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClassEx(&windowclassforwindow1))
    {
        int nResult = GetLastError();
        MessageBox(NULL,
            L"Window class creation failed",
            L"Window Class Failed",
            MB_ICONERROR);
    }

    HWND handleforwindow1 = CreateWindowEx(NULL,
        windowclassforwindow1.lpszClassName,
        L"Window 1",
        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
        200,
        150,
        640,
        480,
        NULL,
        NULL,
        hInst,
        NULL                /* No Window Creation data */
    );

    if (!handleforwindow1)
    {
        int nResult = GetLastError();

        MessageBox(NULL,
            L"Window creation failed",
            L"Window Creation Failed",
            MB_ICONERROR);
    }

    ShowWindow(handleforwindow1, nShowCmd);
    bool endloop = false;
    while (endloop == false) {
        if (GetMessage(&msg, NULL, 0, 0));
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (windowtoopenenum != none) {
            switch (windowtoopenenum) {
            case window2:
                defaultGraphWin(&handleforwindow2, hInst, nShowCmd);
                break;
            case window3:
                modifiedGraphWin(&handleforwindow3, hInst, nShowCmd);
                break;
            case window4:
                condensationGraphWin(&handleforwindow4, hInst, nShowCmd);
                break;
            }
            windowtoopenenum = none;
        }
        if (window1open == false)
            endloop = true;

    }

}

void defaultGraphWin(HWND *hwnd, HINSTANCE hInst, int nShowCmd) {
    WNDCLASSEX wc;
    if (windowclass2registeredbefore == false) {
        ZeroMemory(&wc, sizeof(WNDCLASSEX));
        wc.cbClsExtra = NULL;
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.cbWndExtra = NULL;
        wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hIcon = NULL;
        wc.hIconSm = NULL;
        wc.hInstance = hInst;
        wc.lpfnWndProc = (WNDPROC)defaultGraphProc;
        wc.lpszClassName = L"wc2";
        wc.lpszMenuName = NULL;
        wc.style = CS_HREDRAW | CS_VREDRAW;

        if (!RegisterClassEx(&wc))
        {
            int nResult = GetLastError();
            MessageBox(NULL,
                L"Window class creation failed",
                L"Window Class Failed",
                MB_ICONERROR);
        }
        else
            windowclass2registeredbefore = true;
    }
    hwnd = CreateWindowEx(NULL,
        wc.lpszClassName,
        L"Default graph",
        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
        100, //положение окна на экране по х
        100, //положение по у
        width + 650, //ширина
        width, //висота
        NULL,
        NULL,
        hInst,
        NULL                /* No Window Creation data */
    );

    if (!hwnd)
    {
        int nResult = GetLastError();

        MessageBox(NULL,
            L"Window creation failed",
            L"Window Creation Failed",
            MB_ICONERROR);
    }

    ShowWindow(hwnd, nShowCmd);
}

void modifiedGraphWin(HWND *hwnd, HINSTANCE hInst, int nShowCmd) {
    WNDCLASSEX wc;
    if (windowclass3registeredbefore == false) {
        ZeroMemory(&wc, sizeof(WNDCLASSEX));
        wc.cbClsExtra = NULL;
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.cbWndExtra = NULL;
        wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hIcon = NULL;
        wc.hIconSm = NULL;
        wc.hInstance = hInst;
        wc.lpfnWndProc = (WNDPROC)modifiedGraphProc;
        wc.lpszClassName = L"window class 3";
        wc.lpszMenuName = NULL;
        wc.style = CS_HREDRAW | CS_VREDRAW;

        if (!RegisterClassEx(&wc))
        {
            int nResult = GetLastError();
            MessageBox(NULL,
                L"Window class creation failed",
                L"Window Class Failed",
                MB_ICONERROR);
        }
        else
            windowclass3registeredbefore = true;
    }
    hwnd = CreateWindowEx(NULL,
        wc.lpszClassName,
        L"Modified graph",
        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
        100, //положение окна на экране по х
        100, //положение по у
        width + 650, //ширина
        width, //висота
        NULL,
        NULL,
        hInst,
        NULL                /* No Window Creation data */
    );

    if (!hwnd)
    {
        int nResult = GetLastError();

        MessageBox(NULL,
            L"Window creation failed",
            L"Window Creation Failed",
            MB_ICONERROR);
    }

    ShowWindow(hwnd, nShowCmd);
}

void condensationGraphWin(HWND *hwnd, HINSTANCE hInst, int nShowCmd) {
    WNDCLASSEX wc;
    if (windowclass4registeredbefore == false) {
        ZeroMemory(&wc, sizeof(WNDCLASSEX));
        wc.cbClsExtra = NULL;
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.cbWndExtra = NULL;
        wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hIcon = NULL;
        wc.hIconSm = NULL;
        wc.hInstance = hInst;
        wc.lpfnWndProc = (WNDPROC)condensationGraphProc;
        wc.lpszClassName = L"window class 4";
        wc.lpszMenuName = NULL;
        wc.style = CS_HREDRAW | CS_VREDRAW;

        if (!RegisterClassEx(&wc))
        {
            int nResult = GetLastError();
            MessageBox(NULL,
                L"Window class creation failed",
                L"Window Class Failed",
                MB_ICONERROR);
        }
        else
            windowclass4registeredbefore = true;
    }
    hwnd = CreateWindowEx(NULL,
        wc.lpszClassName,
        L"Condensation graph",
        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
        100, //положение окна на экране по х
        100, //положение по у
        width + 650, //ширина
        width, //висота
        NULL,
        NULL,
        hInst,
        NULL                /* No Window Creation data */
    );

    if (!hwnd)
    {
        int nResult = GetLastError();

        MessageBox(NULL,
            L"Window creation failed",
            L"Window Creation Failed",
            MB_ICONERROR);
    }

    ShowWindow(hwnd, nShowCmd);
}

// windows process functions

HWND defButton, modButton, condButton;

LRESULT CALLBACK windowprocessforwindow1(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        window1open = true;
        defButton = CreateWindowEx(NULL,
            L"BUTTON",
            L"Default graph",
            WS_TABSTOP | WS_VISIBLE |
            WS_CHILD | BS_DEFPUSHBUTTON,
            25,
            220,
            150,
            24,
            hwnd,
            (HMENU)createwindowbuttoninwindow1,
            GetModuleHandle(NULL),
            NULL);
        modButton = CreateWindowEx(NULL,
            L"BUTTON",
            L"Modified graph",
            WS_TABSTOP | WS_VISIBLE |
            WS_CHILD | BS_DEFPUSHBUTTON,
            235,
            220,
            150,
            24,
            hwnd,
            (HMENU)createwindowbuttoninwindow2,
            GetModuleHandle(NULL),
            NULL);
        condButton = CreateWindowEx(NULL,
            L"BUTTON",
            L"Condensation graph",
            WS_TABSTOP | WS_VISIBLE |
            WS_CHILD | BS_DEFPUSHBUTTON,
            450,
            220,
            150,
            24,
            hwnd,
            (HMENU)createwindowbuttoninwindow3,
            GetModuleHandle(NULL),
            NULL);
        break;
    case WM_DESTROY:
        window1open = false;
        break;
    case WM_COMMAND:
        switch LOWORD(wParam) {
        case createwindowbuttoninwindow1:
            EnableWindow(defButton, false);
            windowtoopenenum = window2;
            break;
        case createwindowbuttoninwindow2:
            EnableWindow(modButton, false);
            windowtoopenenum = window3;
            break;
        case createwindowbuttoninwindow3:
            EnableWindow(condButton, false);
            windowtoopenenum = window4;
            break;
        }

    }
    return DefWindowProc(hwnd, message, wParam, lParam);

}