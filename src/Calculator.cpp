// Calculator.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <string>
#include <CommCtrl.h>
#include "Calculator.h"
#include "CalcCore.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
const int MAXDIGITS = 8;						// Max number of digits
int numDigits = 0;								// Current number of digits
std::string displayText = "";					// the number string diplayed in the textbox
std::string operand = "0";						// the number sent to CalcCore
CalcCore calc;									// used to store past numbers, operators, and do calculations
bool buttonPressed = false;						// helps prevents operators being pressed right after another operator
bool equalPressed = false;						// helps for when a number is pressed after equal sign is used

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void				SetFont(HWND hWnd, HFONT& hfDefault);
std::wstring		s2ws();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
	InitCommonControls();
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CALCULATOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CALCULATOR));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CALCULATOR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNFACE+1);
    wcex.lpszMenuName   = nullptr;
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
      CW_USEDEFAULT, 0, 415, 540, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

bool CheckforNumber(std::string& s)
{
	bool isNumber = false;
	if (s.back() == '+')
	{
		return isNumber;
	}
	else if (s.back() == '-')
	{
		return isNumber;
	}
	else if (s.back() == '*')
	{
		return isNumber;
	}
	else if (s.back() == '/')
	{
		return isNumber;
	}
	else if (s == "0")
	{
		return isNumber;
	}
	else if (s.back() == ' ')
		return isNumber;
	else
	{
		isNumber = true;
		return isNumber;
	}
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
	static HWND textBox;
	RECT text{ 0,0, 400, 100 };

    switch (message)
    {
	case WM_CREATE:
		{
		HWND button1, button2,
			button3, button4, button5,
			button6, button7, button8,
			button9, button0, buttonPlus,
			buttonMinus, buttonMultiply,
			buttonDivide, buttonClear,
			buttonDelete, buttonEqual,
			buttonDecimal, buttonPosNeg;
		HFONT hfDefault;
		SetFont(hWnd, hfDefault);
			textBox = CreateWindowEx(WS_EX_STATICEDGE, L"EDIT", L"",
				WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | ES_RIGHT,
				0, 0, 400, 100, hWnd, (HMENU)IDD_TEXTBOX, GetModuleHandle(nullptr), nullptr);
			SendMessage(textBox, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
			
			buttonClear = CreateWindowEx(0, L"BUTTON",
				L"C", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
				0, 100, 100, 80, hWnd, (HMENU)IDD_CLEAR_NUMBERS, GetModuleHandle(nullptr), nullptr);
			SendMessage(buttonClear, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
			buttonDelete = CreateWindowEx(0, L"BUTTON",
				L"Delete", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
				100, 100, 100, 80, hWnd, (HMENU)IDD_DELETE, GetModuleHandle(nullptr), nullptr);
			SendMessage(buttonDelete, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
			buttonDivide = CreateWindowEx(0, L"BUTTON",
				L"/", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
				200, 100, 100, 80, hWnd, (HMENU)IDD_DIVIDE, GetModuleHandle(nullptr), nullptr);
			SendMessage(buttonDivide, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
			buttonMultiply = CreateWindowEx(0, L"BUTTON",
				L"*", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
				300, 100, 100, 80, hWnd, (HMENU)IDD_MULTIPLY, GetModuleHandle(nullptr), nullptr);
			SendMessage(buttonMultiply, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
			
			button7 = CreateWindowEx(0, L"BUTTON",
				L"7", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
				0, 180, 100, 80, hWnd, (HMENU)IDD_NUMBER_7, GetModuleHandle(nullptr), nullptr);
			SendMessage(button7, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
			button8 = CreateWindowEx(0, L"BUTTON",
				L"8", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
				100, 180, 100, 80, hWnd, (HMENU)IDD_NUMBER_8, GetModuleHandle(nullptr), nullptr);
			SendMessage(button8, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
			button9 = CreateWindowEx(0, L"BUTTON",
				L"9", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
				200, 180, 100, 80, hWnd, (HMENU)IDD_NUMBER_9, GetModuleHandle(nullptr), nullptr);
			SendMessage(button9, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
			buttonMinus = CreateWindowEx(0, L"BUTTON",
				L"-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
				300, 180, 100, 80, hWnd, (HMENU)IDD_MINUS, GetModuleHandle(nullptr), nullptr);
			SendMessage(buttonMinus, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
			
			button4 = CreateWindowEx(0, L"BUTTON",
				L"4", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
				0, 260, 100, 80, hWnd, (HMENU)IDD_NUMBER_4, GetModuleHandle(nullptr), nullptr);
			SendMessage(button4, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
			button5 = CreateWindowEx(0, L"BUTTON",
				L"5", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
				100, 260, 100, 80, hWnd, (HMENU)IDD_NUMBER_5, GetModuleHandle(nullptr), nullptr);
			SendMessage(button5, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
			button6 = CreateWindowEx(0, L"BUTTON",
				L"6", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
				200, 260, 100, 80, hWnd, (HMENU)IDD_NUMBER_6, GetModuleHandle(nullptr), nullptr);
			SendMessage(button6, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
			buttonPlus = CreateWindowEx(0, L"BUTTON",
				L"+", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
				300, 260, 100, 80, hWnd, (HMENU)IDD_PLUS, GetModuleHandle(nullptr), nullptr);
			SendMessage(buttonPlus, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
			
			button1 = CreateWindowEx(0, L"BUTTON",
				L"1", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
				0, 340, 100, 80, hWnd, (HMENU)IDD_NUMBER_1, GetModuleHandle(nullptr), nullptr);
			SendMessage(button1, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
			button2 = CreateWindowEx(0, L"BUTTON",
				L"2", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
				100, 340, 100, 80, hWnd, (HMENU)IDD_NUMBER_2, GetModuleHandle(nullptr), nullptr);
			SendMessage(button2, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
			button3 = CreateWindowEx(0, L"BUTTON",
				L"3", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
				200, 340, 100, 80, hWnd, (HMENU)IDD_NUMBER_3, GetModuleHandle(nullptr), nullptr);
			SendMessage(button3, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
			buttonEqual = CreateWindowEx(0, L"BUTTON",
				L"=", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
				300, 340, 100, 160, hWnd, (HMENU)IDD_EQUAL, GetModuleHandle(nullptr), nullptr);
			SendMessage(buttonEqual, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));

			buttonPosNeg = CreateWindowEx(0, L"BUTTON",
				L"+/-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
				0, 420, 100, 80, hWnd, (HMENU)IDD_POSITIVENEGATIVE, GetModuleHandle(nullptr), nullptr);
			SendMessage(buttonPosNeg, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
			button0 = CreateWindowEx(0, L"BUTTON",
				L"0", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
				100, 420, 100, 80, hWnd, (HMENU)IDD_NUMBER_0, GetModuleHandle(nullptr), nullptr);
			SendMessage(button0, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
			buttonDecimal = CreateWindowEx(0, L"BUTTON",
				L".", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
				200, 420, 100, 80, hWnd, (HMENU)IDD_DECIMAL, GetModuleHandle(nullptr), nullptr);
			SendMessage(buttonDecimal, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));

			std::wstring stemp = s2ws();
			SetWindowText(textBox, stemp.c_str());
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
			switch (wmId)
			{
			case IDD_NUMBER_0:
			{
				if (numDigits != MAXDIGITS)
				{
					if (operand != "0")
					{
						operand += "0";
						numDigits++;
					}
					else if (equalPressed)
					{
						operand = "0";
						calc.clear();
					}
				}
				std::wstring stemp = s2ws();
				SetWindowText(textBox, stemp.c_str());
				buttonPressed = true;
				equalPressed = false;
			}
			break;
			case IDD_NUMBER_1:
			{
				if (numDigits != MAXDIGITS)
				{
					if (operand == "0")
					{
						operand = "1";
					}
					else if (equalPressed)
					{
						operand = "1";
						calc.clear();
					}
					else
					{
						operand += "1";
						numDigits++;
					}
				}
				std::wstring stemp = s2ws();
				SetWindowText(textBox, stemp.c_str());
				buttonPressed = true;
				equalPressed = false;
			}
			break;
			case IDD_NUMBER_2:
			{
				if (numDigits != MAXDIGITS)
				{
					if (operand == "0")
					{
						operand = "2";
					}
					else if (equalPressed)
					{
						operand = "2";
						calc.clear();
					}
					else
					{
						operand += "2";
						numDigits++;
					}
				}
				std::wstring stemp = s2ws();
				SetWindowText(textBox, stemp.c_str());
				buttonPressed = true;
				equalPressed = false;
			}
			break;
			case IDD_NUMBER_3:
			{
				if (numDigits != MAXDIGITS)
				{
					if (operand == "0")
					{
						operand = "3";
					}
					else if (equalPressed)
					{
						operand = "3";
						calc.clear();
					}
					else
					{
						operand += "3";
						numDigits++;
					}
				}
				std::wstring stemp = s2ws();
				SetWindowText(textBox, stemp.c_str());
				buttonPressed = true;
				equalPressed = false;
			}
			break;
			case IDD_NUMBER_4:
			{
				if (numDigits != MAXDIGITS)
				{
					if (operand == "0")
					{
						operand = "4";
					}
					else if (equalPressed)
					{
						operand = "4";
						calc.clear();
					}
					else
					{
						operand += "4";
						numDigits++;
					}
				}
				std::wstring stemp = s2ws();
				SetWindowText(textBox, stemp.c_str());
				buttonPressed = true;
				equalPressed = false;
			}
			break;
			case IDD_NUMBER_5:
			{
				if (numDigits != MAXDIGITS)
				{
					if (operand == "0")
					{
						operand = "5";
					}
					else if (equalPressed)
					{
						operand = "5";
						calc.clear();
					}
					else
					{
						operand += "5";
						numDigits++;
					}
				}
				std::wstring stemp = s2ws();
				SetWindowText(textBox, stemp.c_str());
				buttonPressed = true;
				equalPressed = false;
			}
			break;
			case IDD_NUMBER_6:
			{
				if (numDigits != MAXDIGITS)
				{
					if (operand == "0")
					{
						operand = "6";
					}
					else if (equalPressed)
					{
						operand = "6";
						calc.clear();
					}
					else
					{
						operand += "6";
						numDigits++;
					}
				}
				std::wstring stemp = s2ws();
				SetWindowText(textBox, stemp.c_str());
				buttonPressed = true;
				equalPressed = false;
			}
			break;
			case IDD_NUMBER_7:
			{
				if (numDigits != MAXDIGITS)
				{
					if (operand == "0")
					{
						operand = "7";
					}
					else if (equalPressed)
					{
						operand = "7";
						calc.clear();
					}
					else
					{
						operand += "7";
						numDigits++;
					}
				}
				std::wstring stemp = s2ws();
				SetWindowText(textBox, stemp.c_str());
				buttonPressed = true;
				equalPressed = false;
			}
			break;
			case IDD_NUMBER_8:
			{
				if (numDigits != MAXDIGITS)
				{
					if (operand == "0")
					{
						operand = "8";
					}
					else if (equalPressed)
					{
						operand = "8";
						calc.clear();
					}
					else
					{
						operand += "8";
						numDigits++;
					}
				}
				std::wstring stemp = s2ws();
				SetWindowText(textBox, stemp.c_str());
				buttonPressed = true;
				equalPressed = false;
			}
			break;
			case IDD_NUMBER_9:
			{
				if (numDigits != MAXDIGITS)
				{
					if (operand == "0")
					{
						operand = "9";
					}
					else if (equalPressed)
					{
						operand = "9";
						calc.clear();
					}
					else
					{
						operand += "9";
						numDigits++;
					}
				}
				std::wstring stemp = s2ws();
				SetWindowText(textBox, stemp.c_str());
				buttonPressed = true;
				equalPressed = false;
			}
				break;
			case IDD_CLEAR_NUMBERS:
			{
				if (displayText != "")
				{
					displayText = "";
					operand = "0";
					calc.clear();
				}
				std::wstring stemp = s2ws();
				SetWindowText(textBox, stemp.c_str());
				equalPressed = false;
			}
				break;
			case IDD_DECIMAL:
			{
				operand += ".";
				std::wstring stemp = s2ws();
				SetWindowText(textBox, stemp.c_str());
			}
				break;
			case IDD_DELETE:
			{
				if (operand[0] != '-')
				{
					if (operand.size() != 1)
					{
						operand.pop_back();
					}
					else if (operand.size() == 1)
					{
						operand = "0";
					}
				}
				else if (operand[0] == '-')
				{
					if (operand.size() != 2)
					{
						operand.pop_back();
					}
					else
					{
						operand = "0";
					}
				}
				std::wstring stemp = s2ws();
				SetWindowText(textBox, stemp.c_str());
			}
				break;
				// Once =, +, -, *, or / is pressed the numbered stored in operand is sent to CalcCore.
				// Then the operator is sent to CalcCore, and determines if an operation occurs.
				// After that operand and displayText are reset and the new text is displayed.
			case IDD_EQUAL:
			{
				if (buttonPressed)
				{
					calc.SetOperands(std::stof(operand));

					Oper eq;    // Making Operator object, then sending it to CalcCore to be stored and check for operations.
					eq.op = Prec::equate;
					calc.SetOperators(eq);

					operand = calc.GetString();
					displayText = operand;

					numDigits = 0;
					equalPressed = true;
				}
				std::wstring stemp = s2ws();
				SetWindowText(textBox, stemp.c_str());
			}
				break;
			case IDD_PLUS:
			{
				if (buttonPressed)
				{
					if (!equalPressed)
					{
						displayText += operand + " + ";
					}
					calc.SetOperands(std::stof(operand));
					operand = "0";


					Oper add;
					add.op = Prec::add;
					calc.SetOperators(add);

					buttonPressed = false;
					equalPressed = false;
					numDigits = 0;
				}
				std::wstring stemp = s2ws();
				SetWindowText(textBox, stemp.c_str());
			}
				break;
			case IDD_MINUS:
			{
				if (buttonPressed)
				{
					if (!equalPressed)
					{
						displayText += operand + " - ";
					}

					calc.SetOperands(std::stof(operand));
					operand = "0";

					Oper minus;
					minus.op = Prec::subtract;
					calc.SetOperators(minus);

					buttonPressed = false;
					equalPressed = false;
					numDigits = 0;
				}
				std::wstring stemp = s2ws();
				SetWindowText(textBox, stemp.c_str());
			}
				break;
			case IDD_MULTIPLY:
			{
				if (buttonPressed)
				{
					if (!equalPressed)
					{
						displayText += operand + " * ";
					}
					calc.SetOperands(std::stof(operand));
					operand = "0";

					Oper mult;
					mult.op = Prec::multiply;
					calc.SetOperators(mult);

					buttonPressed = false;
					equalPressed = false;
					numDigits = 0;
				}
				std::wstring stemp = s2ws();
				SetWindowText(textBox, stemp.c_str());
			}
				break;
			case IDD_DIVIDE:
			{
				if (buttonPressed)
				{
					if (!equalPressed)
					{
						displayText += operand + " / ";
					}
					calc.SetOperands(std::stof(operand));
					operand = "0";

					Oper div;
					div.SetWindow(hWnd);
					div.op = Prec::divide;
					calc.SetOperators(div);

					buttonPressed = false;
					equalPressed = false;
					numDigits = 0;
				}
				std::wstring stemp = s2ws();
				SetWindowText(textBox, stemp.c_str());
			}
				break;
			case IDD_POSITIVENEGATIVE:
			{
				if (buttonPressed && operand != "0")
				{
					if (operand[0] != '-')
					{
						operand = "-" + operand;
					}
					else
					{
						operand.erase(operand[0]);
					}
				}
				std::wstring stemp = s2ws();
				SetWindowText(textBox, stemp.c_str());
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
			std::wstring stemp = s2ws();
			
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

void SetFont(HWND hWnd, HFONT& hfDefault)
{
	HFONT hf;
	HDC hdc;
	long lfHeight;

	hdc = GetDC(nullptr);
	lfHeight = -MulDiv(12, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	ReleaseDC(nullptr, hdc);

	hf = CreateFont(lfHeight, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, L"Segou UI Black");

	if (hf)
	{
		DeleteObject(hfDefault);
		hfDefault = hf;
	}
	else
	{
		MessageBox(hWnd, L"Font creation Failed!", L"Error", MB_OK | MB_ICONEXCLAMATION);
	}
}

std::wstring s2ws()
{
	std::string temp = displayText + "\r\n\r\n" + operand;
	int len;
	int slength = (int)temp.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, temp.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, temp.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}