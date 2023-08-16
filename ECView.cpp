#include <windows.h>
#include <iostream>
#include <string>

HWND hInputTextBox;  // Handle for the input text box control
HWND hResultTextBox; // Handle for the result text box control

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 1: // Handle Clear button click for the input text box
            SetWindowText(hInputTextBox, ""); // Clear the input text box
            break;
        case 2: // Handle Enter button click
            int length = GetWindowTextLength(hInputTextBox) + 1;
            char *inputText = new char[length];
            GetWindowText(hInputTextBox, inputText, length);
            // Process the entered text
            // ... Your polynomial operations here ...

            // Set the result in the result text box
            SetWindowText(hResultTextBox, "Result goes here...");

            delete[] inputText;
            break;
        }
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    
    // Window structure (from win.h); hold info about window 
    WNDCLASS wc = {};

    // Long Pointer to Window Procedure
    wc.lpfnWndProc = WindowProc;

    //specifies exe file (for windows)
    wc.hInstance = hInstance;

    // long pointer to a null-terminated string class name
    wc.lpszClassName = "Calculator View"; 

    // Relay window info to os
    RegisterClass(&wc);

    // Create the main window
    HWND hwnd = CreateWindowEx(0, "Calculator View", "Polynomial Calculator", WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT, CW_USEDEFAULT, 600, 300,
                               NULL, NULL, hInstance, NULL);

    if (!hwnd)
    {
        DWORD error = GetLastError();
        MessageBox(NULL, ("Window creation failed! Error code: " + std::to_string(error)).c_str(), "Error", MB_ICONERROR);
        return 1;
    }


    // Create the input window
    hInputTextBox = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_MULTILINE,
                                   10, 10, 0.6 * 600, 100,
                                   hwnd, NULL, hInstance, NULL);

    // Create the Clear button for the input text box
    CreateWindow("BUTTON", "Clear", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 10, 120, 100, 30,
                 hwnd, (HMENU)1, hInstance, NULL);

    // Create the Enter button
    CreateWindow("BUTTON", "Enter", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 0.6 * 600 - 100, 120, 100, 30,
                 hwnd, (HMENU)2, hInstance, NULL);

    // Create the result text box (40% width)
    hResultTextBox = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_READONLY,
                                    0.6 * 600 + 10, 10, 0.4 * 600 - 20, 200,
                                    hwnd, NULL, hInstance, NULL);

    // Show the window
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Enter the message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
