// spLab1.cpp: ���������� ����� ����� ��� ����������.
//

#include "stdafx.h"
#include "spLab2.h"

#define MAX_LOADSTRING 100

// ���������� ����������:
HINSTANCE hInst;                                // ������� ���������
WCHAR szTitle[MAX_LOADSTRING];                  // ����� ������ ���������
WCHAR szWindowClass[MAX_LOADSTRING];            // ��� ������ �������� ����
HWND hWnd;
DWORD g_uThCount;								//	���������� ��������� �������

HANDLE hSecThread[3];							// ����������� �������
DWORD dwSecThreadId[3];							// id �������
DWORD dwSecThreadState[3];						// ��������� ������: 0 - �������, 1 - ����

// �������������� ����:
const UINT ID_FLOW1_ACTION1 = 1;
const UINT ID_FLOW1_ACTION2 = 2;
const UINT ID_FLOW1_ACTION3 = 3;
const UINT ID_FLOW1_ACTION4 = 4;
const UINT ID_FLOW1_ACTION5 = 5;
const UINT ID_FLOW1_ACTION6 = 6;
const UINT ID_FLOW1_ACTION7 = 7;

const UINT ID_FLOW2_ACTION1 = 8;
const UINT ID_FLOW2_ACTION2 = 9;
const UINT ID_FLOW2_ACTION3 = 10;
const UINT ID_FLOW2_ACTION4 = 11;
const UINT ID_FLOW2_ACTION5 = 12;
const UINT ID_FLOW2_ACTION6 = 13;
const UINT ID_FLOW2_ACTION7 = 14;

const UINT ID_FLOWINFO_PRIMARY = 15;
const UINT ID_FLOWINFO_FIRST = 16;
const UINT ID_FLOWINFO_SECOND = 17;

					   // ��������� ���������� �������, ���������� � ���� ������ ����:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ProcessInfo(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI		thread1Func(LPVOID);
DWORD WINAPI		thread2Func(LPVOID);
void				GetThreadInfo(int);
std::wstring		convert2Wstring(DWORD);
std::wstring		convert2Wstring(HANDLE);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: ���������� ��� �����.

	// ������������� ���������� �����
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_SPLAB2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ��������� ������������� ����������:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPLAB2));

	MSG msg;

	// ���� ��������� ���������:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  �������: MyRegisterClass()
//
//  ����������: ������������ ����� ����.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPLAB2));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SPLAB2);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   �������: InitInstance(HINSTANCE, int)
//
//   ����������: ��������� ��������� ���������� � ������� ������� ����.
//
//   �����������:
//
//        � ������ ������� ���������� ���������� ����������� � ���������� ����������, � �����
//        ��������� � ��������� �� ����� ������� ���� ���������.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // ��������� ���������� ���������� � ���������� ����������

	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	LPCWSTR mainWindowTitle = L"���_thread_lb";
	SetWindowText(hWnd, mainWindowTitle);

	HMENU mainMenu;
	HMENU flow1Menu,flow2Menu,flowInfoMenu;

	mainMenu = CreateMenu();
	flow1Menu = CreatePopupMenu();
	flow2Menu = CreatePopupMenu();
	flowInfoMenu = CreatePopupMenu();

	AppendMenu(flow1Menu, MF_STRING, ID_FLOW1_ACTION1, L"������� �����");
	AppendMenu(flow1Menu, MF_STRING, ID_FLOW1_ACTION2, L"������� ������ �����");
	AppendMenu(flow1Menu, MF_STRING, ID_FLOW1_ACTION3, L"������������� �����");
	AppendMenu(flow1Menu, MF_STRING, ID_FLOW1_ACTION4, L"���������� ������ ������");
	AppendMenu(flow1Menu, MF_STRING, ID_FLOW1_ACTION5, L"���������� �����");
	AppendMenu(flow1Menu, MF_STRING, ID_FLOW1_ACTION6, L"��������� ���������");
	AppendMenu(flow1Menu, MF_STRING, ID_FLOW1_ACTION7, L"��������� ���������");

	AppendMenu(flow2Menu, MF_STRING, ID_FLOW2_ACTION1, L"������� �����");
	AppendMenu(flow2Menu, MF_STRING, ID_FLOW2_ACTION2, L"������� ������ �����");
	AppendMenu(flow2Menu, MF_STRING, ID_FLOW2_ACTION3, L"������������� �����");
	AppendMenu(flow2Menu, MF_STRING, ID_FLOW2_ACTION4, L"���������� ������ ������");
	AppendMenu(flow2Menu, MF_STRING, ID_FLOW2_ACTION5, L"���������� �����");
	AppendMenu(flow2Menu, MF_STRING, ID_FLOW2_ACTION6, L"��������� ���������");
	AppendMenu(flow2Menu, MF_STRING, ID_FLOW2_ACTION7, L"��������� ���������");

	AppendMenu(flowInfoMenu, MF_STRING, ID_FLOWINFO_PRIMARY, L"��������� �����");
	AppendMenu(flowInfoMenu, MF_STRING, ID_FLOWINFO_FIRST, L"����� 1");
	AppendMenu(flowInfoMenu, MF_STRING, ID_FLOWINFO_SECOND, L"����� 2");

	AppendMenu(mainMenu, MF_STRING | MF_POPUP, (UINT)flow1Menu, L"����� 1");
	AppendMenu(mainMenu, MF_STRING | MF_POPUP, (UINT)flow2Menu, L"����� 2");
	AppendMenu(mainMenu, MF_STRING | MF_POPUP, (UINT)flowInfoMenu, L"���������� � �������");

	SetMenu(hWnd, flow1Menu);
	SetMenu(hWnd, flow2Menu);
	SetMenu(hWnd, flowInfoMenu);
	SetMenu(hWnd, mainMenu);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	hSecThread[0] = GetCurrentThread();
	dwSecThreadId[0] = GetCurrentThreadId();
	dwSecThreadState[0] = 0;

	return TRUE;
}

//
//  �������: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����������:  ������������ ��������� � ������� ����.
//
//  WM_COMMAND � ���������� ���� ����������
//  WM_PAINT � ���������� ������� ����
//  WM_DESTROY � ��������� ��������� � ������ � ���������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// ��������� ����� � ����:
		switch (wmId)
		{
		case ID_FLOW1_ACTION1:
			if (hSecThread[1] == NULL) {
				hSecThread[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread1Func, NULL, 0, &dwSecThreadId[1]);
				dwSecThreadState[1] = 0;
			}
			else
				MessageBox(hWnd, L"����� 1 ��� �������", NULL, NULL);
			break;
		case ID_FLOW1_ACTION2:
			if (hSecThread[1] == NULL) {
				hSecThread[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread1Func, NULL, CREATE_SUSPENDED, &dwSecThreadId[1]);
				dwSecThreadState[1] = 1;
			}
			else
				MessageBox(hWnd, L"����� 1 ��� �������", NULL, NULL);
			break;
		case ID_FLOW1_ACTION3:
			if (hSecThread[1] != NULL) {
				SuspendThread(hSecThread[1]);
				dwSecThreadState[1] = 1;
			}
			else
				MessageBox(hWnd, L"����� 1 �� �������", NULL, NULL);
			break;
		case ID_FLOW1_ACTION4:
			if (hSecThread[1] != NULL) {
				ResumeThread(hSecThread[1]);
				dwSecThreadState[1] = 0;
			}
			else
				MessageBox(hWnd, L"����� 1 �� �������", NULL, NULL);
			break;
		case ID_FLOW1_ACTION5:
			if (hSecThread[1] != NULL) {
				TerminateThread(hSecThread[1], 0);
				hSecThread[1] = NULL;
			}
			else
				MessageBox(hWnd, L"����� 1 �� �������", NULL, NULL);
			break;
		case ID_FLOW1_ACTION6:
			if (hSecThread[1] != NULL)
				SetThreadPriority(hSecThread[1], GetThreadPriority(hSecThread[1]) + 1);
			else
				MessageBox(hWnd, L"����� 1 �� �������", NULL, NULL);
			break;
		case ID_FLOW1_ACTION7:
			if (hSecThread[1] != NULL)
				SetThreadPriority(hSecThread[1], GetThreadPriority(hSecThread[1]) - 1);
			else
				MessageBox(hWnd, L"����� 1 �� �������", NULL, NULL);
			break;
		case ID_FLOW2_ACTION1:
			if (hSecThread[2] == NULL) {
				hSecThread[2] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread2Func, NULL, 0, &dwSecThreadId[2]);
				dwSecThreadState[2] = 0;
			}
			else
				MessageBox(hWnd, L"����� 2 ��� �������", NULL, NULL);
			break;
		case ID_FLOW2_ACTION2:
			if (hSecThread[2] == NULL) {
				hSecThread[2] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread2Func, NULL, CREATE_SUSPENDED, &dwSecThreadId[2]);
				dwSecThreadState[2] = 1;
			}
			else
				MessageBox(hWnd, L"����� 2 ��� �������", NULL, NULL);
			break;
		case ID_FLOW2_ACTION3:
			if (hSecThread[2] != NULL) {
				SuspendThread(hSecThread[2]);
				dwSecThreadState[2] = 1;
			}
			else
				MessageBox(hWnd, L"����� 2 �� �������", NULL, NULL);
			break;
		case ID_FLOW2_ACTION4:
			if (hSecThread[2] != NULL) {
				ResumeThread(hSecThread[2]);
				dwSecThreadState[2] = 0;
			}
			else
				MessageBox(hWnd, L"����� 2 �� �������", NULL, NULL);
			break;
		case ID_FLOW2_ACTION5:
			if (hSecThread[2] != NULL) {
				TerminateThread(hSecThread[2], 0);
				hSecThread[2] = NULL;
			}
			else
				MessageBox(hWnd, L"����� 2 �� �������", NULL, NULL);
			break;
		case ID_FLOW2_ACTION6:
			if (hSecThread[1] != NULL)
				SetThreadPriority(hSecThread[2], GetThreadPriority(hSecThread[2]) + 1);
			else
				MessageBox(hWnd, L"����� 2 �� �������", NULL, NULL);
			break;
		case ID_FLOW2_ACTION7:
			if (hSecThread[1] != NULL)
				SetThreadPriority(hSecThread[2], GetThreadPriority(hSecThread[2]) - 1);
			else
				MessageBox(hWnd, L"����� 2 �� �������", NULL, NULL);
			break;
		case ID_FLOWINFO_PRIMARY:
		case ID_FLOWINFO_FIRST:
		case ID_FLOWINFO_SECOND:
			GetThreadInfo(wmId);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
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
		// TODO: �������� ���� ����� ��� ����������, ������������ HDC...
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

DWORD WINAPI thread1Func(LPVOID lpParam) {
	//while (true) {
		int x, w, y, h;
		y = -100; h = 200;
		x = 100; w = 500;
		CreateWindow(L"static", L"������� ������ ������ ���", WS_VISIBLE | WS_CHILD, 0, 0, 490, 25,hWnd, NULL, NULL, NULL);

		return 0;
	//}
}

DWORD WINAPI thread2Func(LPVOID lpParam) {
	//while (true) {
	int x, w, y, h;
	y = -100; h = 200;
	x = 100; w = 500;
	CreateWindow(L"static", L"������� ������ ������ ���", WS_VISIBLE | WS_CHILD, 16, 16, 490, 25, hWnd, 0, NULL, NULL);

	return 0;
	//}
}
void GetThreadInfo(int index) {

	LPCWSTR msgBoxCaption;
	LPCWSTR msgBoxContent;
	msgBoxCaption = L"���������� � ������";
	std::wstring msgWBoxContent = L"";

	int key = 0;
	switch (index) {
		case ID_FLOWINFO_FIRST:
			key = 1;
			break;
		case ID_FLOWINFO_SECOND:
			key = 2;
			break;
	}

	msgWBoxContent = L"������������� ������: " + convert2Wstring(dwSecThreadId[key]);
	msgWBoxContent += L"\n���������� ������: " + convert2Wstring(hSecThread[key]);
	msgWBoxContent += L"\n��������� ������: ";

	if (hSecThread[key] == NULL) {
		DWORD threadExitCode = NULL;
		HANDLE temp = OpenThread(THREAD_ALL_ACCESS, false, dwSecThreadId[key]);
		BOOL exitThreadCheck = GetExitCodeThread(temp, &threadExitCode);
		if (exitThreadCheck == 0)
			msgWBoxContent += L"�� �������";
		else
			msgWBoxContent += L"������(" + convert2Wstring(threadExitCode) + L")";
	}
	else {
		if (dwSecThreadState[key] == 1)
			msgWBoxContent += L"�������������";
		else
			msgWBoxContent += L"�������";
	}

	msgWBoxContent += L"\n������������� ��������� ������: " + convert2Wstring(GetThreadPriority(hSecThread[key]));

	FILETIME ft[4];

	if (GetThreadTimes(hSecThread[key], &ft[0], &ft[1], &ft[2], &ft[3]) > 0) {
		if (hSecThread[key] != NULL) {
			SYSTEMTIME sTemp;
			GetSystemTime(&sTemp);
			SystemTimeToFileTime(&sTemp, &ft[1]);
		}

		ULARGE_INTEGER ul1;
		ul1.LowPart = ft[0].dwLowDateTime;
		ul1.HighPart = ft[0].dwHighDateTime;
		ULARGE_INTEGER ul2;
		ul2.LowPart = ft[1].dwLowDateTime;
		ul2.HighPart = ft[1].dwHighDateTime;

		ul2.QuadPart -= ul1.QuadPart;
		ul2.QuadPart /= 10 * 1000 * 1000;
		msgWBoxContent += L"\n����� ����� ������: " + convert2Wstring(ul2.QuadPart) + L"c";
	}

	msgBoxContent = msgWBoxContent.c_str();

	MessageBox(hWnd, msgBoxContent, msgBoxCaption, 0);
}


std::wstring convert2Wstring(DWORD value) {
	std::wstringstream str;
	str << value;
	return str.str();
}
std::wstring convert2Wstring(HANDLE value) {
	std::wstringstream str;
	str << value;
	return str.str();
}

// ���������� ��������� ��� ���� "� ���������".
INT_PTR CALLBACK ProcessInfo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
