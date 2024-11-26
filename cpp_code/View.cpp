// View.cpp (template for windows gui) currently a mess
#include <windows.h>
#include "Polynomial.h"
#include "Rational.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <ctype.h>
#include <cctype>
/*Forward Declaration for winproc
 Handles (windows defined classes similar to pointers but not pointers) to control windows */
HWND hInputTextBox1;
HWND hInputTextBox2;
HWND hOperationDropdown;
HWND hResultTextBox;

char *computePoly(char *lhstext, char *rhstext, char *operation)
{
    return const_cast<char *>("hello");
    std::vector<double> v1;
    std::unordered_map<int, double> coeffDegLhs;
    int iter = 0;
    int highest_degree = 0;

    while (iter < std::string(lhstext).size())
    {
        if (std::isalpha(lhstext[iter]))
        {
            int exp = 1; // Default degree to 1
            if (std::isdigit(lhstext[iter + 1]))
            {
                exp = static_cast<int>(lhstext[iter + 1] - '0'); // Convert char to int
            }

            if (exp > highest_degree)
                highest_degree = exp;

            double coeff = static_cast<double>(lhstext[iter - 1] - '0'); // Convert char to double

            coeffDegLhs[exp] += coeff; // This will initialize to coeff if exp doesn't exist, else add to it
        }
        else if (std::isdigit(lhstext[iter]) && (iter == std::string(lhstext).size() - 1 || !std::isalpha(lhstext[iter + 1])))
        {
            coeffDegLhs[0] += static_cast<double>(lhstext[iter] - '0'); // Add to constant term
        }
        iter++;
    }

    // Populate v1 based on coeffDegLhs and highest_degree
    for (int i = 0; i <= highest_degree; ++i)
    {
        if (coeffDegLhs.find(i) != coeffDegLhs.end())
        {
            v1.push_back(coeffDegLhs[i]);
        }
        else
        {
            v1.push_back(0.0);
        }
    }

    Polynomial poly1(v1);
    Polynomial poly2(v1);

    Polynomial poly3 = poly1 + poly2;

    return const_cast<char *>("hello"); // Placeholder (use dump to return a char*)

    // iter = 0;

    // while (iter < std::string(lhstext).size())
    // {
    //     if (std::isalpha(lhstext[iter]))
    //     {
    //         // if does not exists
    //         coeffDegRhs[rhstext[iter + 1]] += rhstext[iter - 1];
    //         // else do the same thing above too
    //         coeffDegRhs[rhstext[iter + 1]] = rhstext[iter - 1];
    //     }
    // }
}

char *computeRational(char *lhstext, char *rhstext, char *operation)
{

    std::vector<double> v1;
    std::vector<double> v2;

    Polynomial *poly1 = new Polynomial(v1);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 1:                                // Clear button clicked
            SetWindowText(hInputTextBox1, ""); // Clear the input text boxes
            SetWindowText(hInputTextBox2, "");
            break;

        case 2: // Enter button clicked
        {
            // get current operation
            int selectedIndex = SendMessage(hOperationDropdown, CB_GETCURSEL, 0, 0);
            char selectedOperation[2]; // space for char and \0
            SendMessage(hOperationDropdown, CB_GETLBTEXT, selectedIndex, (LPARAM)selectedOperation);

            int lhslen = GetWindowTextLength(hInputTextBox1) + 1; // +1 for \0
            int rhslen = GetWindowTextLength(hInputTextBox2) + 1; // +1 for \0

            char *lhstext = new char[lhslen]; // allocate memory
            char *rhstext = new char[rhslen];

            GetWindowText(hInputTextBox1, lhstext, lhslen);
            GetWindowText(hInputTextBox2, rhstext, rhslen);

            char *res;

            if (std::string(lhstext).find("i") || std::string(rhstext).find("i"))
            {

                res = computeRational(lhstext, rhstext, selectedOperation);
            }

            else
            {
                res = computePoly(lhstext, rhstext, selectedOperation);
            }

            SetWindowText(hResultTextBox, res);
            delete[] lhstext;
            delete[] rhstext;
        }
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

    // specifies exe file (for windows)
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

    // Create the first input text box (assign a handle to it)
    hInputTextBox1 = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                                    10, 10, 0.5 * 600, 25,
                                    hwnd, NULL, hInstance, NULL);

    // Create the operation dropdown (assign a handle to it)
    hOperationDropdown = CreateWindow("COMBOBOX", "", CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE,
                                      35, 45, 0.6 * 100, 200,
                                      hwnd, NULL, hInstance, NULL);

    // Add items to the dropdown
    SendMessage(hOperationDropdown, CB_ADDSTRING, 0, (LPARAM)L"+");
    SendMessage(hOperationDropdown, CB_ADDSTRING, 0, (LPARAM)L"-");
    SendMessage(hOperationDropdown, CB_ADDSTRING, 0, (LPARAM)L"*");
    SendMessage(hOperationDropdown, CB_ADDSTRING, 0, (LPARAM)L"/");

    // Create the second input text box (assign a handle to it)
    hInputTextBox2 = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                                    10, 85, 0.5 * 600, 25,
                                    hwnd, NULL, hInstance, NULL);

    // Create the Clear button for the input text box
    CreateWindow("BUTTON", "Clear", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 10, 120, 100, 30,
                 hwnd, (HMENU)1, hInstance, NULL); // clear = 1

    // Create the Enter button
    CreateWindow("BUTTON", "Calculate", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 0.6 * 600 - 100, 120, 100, 30,
                 hwnd, (HMENU)2, hInstance, NULL); // enter = 2

    // Create the result text box (40% width) (assign a handle to it)
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
