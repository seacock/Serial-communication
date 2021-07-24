// ComPort.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "ComPort.h"

#include <iostream>
#include <string>
#include <Windows.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hDlgComPort;
TCHAR ListItem[256];
::std::wstring wTot;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK ComPortBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void ShowComPort();
bool ComPort();
int ReadByte();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_COMPORT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COMPORT));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COMPORT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_COMPORT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);

            HWND hwndGoto = NULL;  // Window handle of dialog box 

            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDM_COM_PORT:
                if (!IsWindow(hwndGoto))
                {
                    hwndGoto = CreateDialog(hInst,
                        MAKEINTRESOURCE(IDD_DLG_COM_PORT),
                        hWnd,
                        (DLGPROC)ComPortBox);
                    ShowWindow(hwndGoto, SW_SHOW);
                }
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// Message handler for COM PORT box.
INT_PTR CALLBACK ComPortBox(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{    
    hDlgComPort = hDlg;
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
    {
        wTot = L"";
        return (INT_PTR)TRUE;
    }    
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDC_BUTTON_COM_PORT)
        {
            ShowComPort();
            return (INT_PTR)TRUE;
        }
        if (HIWORD(wParam) == CBN_SELCHANGE)
        {
            int ItemIndex = (int)SendMessage((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);                
            (TCHAR)SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT, (WPARAM)ItemIndex, (LPARAM)ListItem);
            ComPort();
        }
        break;

    }
    return (INT_PTR)FALSE;
}

bool ComPort()
{
    DCB dcb;
    DWORD byteswritten;
    HANDLE hPort = CreateFile(ListItem, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    
    if (!GetCommState(hPort, &dcb))
        return false;

    dcb.BaudRate = CBR_9600; //9600 Baud
    dcb.ByteSize = 8; //8 data bits
    dcb.Parity = NOPARITY; //no parity
    dcb.StopBits = ONESTOPBIT; //1 stop
    if (!SetCommState(hPort, &dcb))
        return false;

    bool retVal = WriteFile(hPort, "From ComPort", 12, &byteswritten, NULL);
    CloseHandle(hPort); //close the handle

    ReadByte();

    return retVal;    
}

int ReadByte()
{
    DCB dcb;
    int retVal = 0;
    BYTE Byte[20] = {0};
    DWORD dwBytesTransferred, dwError;
    DWORD dwCommModemStatus = 0;
    HANDLE hPort = CreateFile(ListItem, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (!GetCommState(hPort, &dcb))
        return 0x100;
    dcb.BaudRate = CBR_9600; //9600 Baud
    dcb.ByteSize = 8; //8 data bits
    dcb.Parity = NOPARITY; //no parity
    dcb.StopBits = ONESTOPBIT; //1 stop
    if (!SetCommState(hPort, &dcb))
        return 0x100;
    SetCommMask(hPort, EV_RXCHAR | EV_ERR); //receive character event
    WaitCommEvent(hPort, &dwCommModemStatus, 0); //wait for character
    if (dwCommModemStatus & EV_RXCHAR)
        bool retVal = ReadFile(hPort, Byte, 9, &dwBytesTransferred, 0); //read 1
    else if (dwCommModemStatus & EV_ERR)
        retVal = 0x101;

    retVal = Byte[0];
    CloseHandle(hPort);

    Byte[9] = '\r';
    Byte[10] = '\n';

    HWND hWndEdit = GetDlgItem(hDlgComPort, IDC_EDIT_COM_PORT);   

    ::std::wstring wideWhat;
    int convertResult = MultiByteToWideChar(CP_UTF8, 0, (LPCCH)Byte, (int)strlen((const char*)Byte), NULL, 0);
    if (convertResult == 0)        
        dwError = GetLastError();
    else
    {
        wideWhat.resize((size_t)convertResult);
        convertResult = MultiByteToWideChar(CP_UTF8, 0, (LPCCH)Byte, (int)strlen((const char*)Byte), &wideWhat[0], (int)wideWhat.size());
        if (convertResult == 0)
            dwError = GetLastError();
    }
    wTot += wideWhat;

    SendMessage(hWndEdit, WM_SETTEXT, 0, (LPARAM)wTot.c_str());// Add text to the window. 

    return retVal;
}

void ShowComPort() // Find the present serial.
{
    ULONG pPortNumbers[20], uPortNumbersCount = 20, uPortNumbersFound;
    GetCommPorts(pPortNumbers, uPortNumbersCount, &uPortNumbersFound);
    HWND hWndComboBox = GetDlgItem(hDlgComPort, IDC_COMBO_COM_PORT);

    // load the combobox with item list.
    TCHAR A[15];    
    for (ULONG k = 0; k < uPortNumbersFound; k++)
    {
        swprintf_s(A, sizeof(A) / sizeof(TCHAR), L"COM%d", pPortNumbers[k]);        
        SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)A);    // Add string to combobox.
    }
        
    SendMessage(hWndComboBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);  // Display an initial item in the selection field. 
}