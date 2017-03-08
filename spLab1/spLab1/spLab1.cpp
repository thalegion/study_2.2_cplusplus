// spLab1.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "spLab1.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HWND hWnd;
LPCWSTR msgBoxCaption;
LPCWSTR msgBoxContent;
std::wstring msgWBoxContent;
TCHAR currentFile[MAX_PATH];
TCHAR currentDirectory[MAX_PATH];

// Идентификаторы меню:
const UINT ID_PROCESS_NOTEPAD = 4;
const UINT ID_PROCESS_NOTEPAD_TEXT = 5;
const UINT ID_PROCESS_CALCULATOR = 6;
const UINT ID_PROCESS_CALCULATOR_CLOSE = 7;

const UINT ID_PROCESS_INFO_CURRENT = 0;
const UINT ID_PROCESS_INFO_NOTEPAD = 1;
const UINT ID_PROCESS_INFO_NOTEPAD_TEXT = 2;
const UINT ID_PROCESS_INFO_CALCULATOR = 3;

const UINT ID_FILENAME_INPUT = 8;

//Массивы для лабы:
HANDLE ProcHandle[4];// для дескрипторов процессов;
DWORD ProcId[4];// для идентификаторов процессов;
HANDLE ThreadHandle[4];// для дескрипторов потоков;
DWORD ThreadId[4];// для.идентификаторов потоков;
LPTSTR ProcImage[4];// для указателей строк, идентифицирущих файлы запускаемых программ;
TCHAR CmdParam[4][260];// для строк c параметрами запускаемых программ.

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
void				ProcessMenu(int);
void				GetProcessInfo(int);
std::wstring		convert2Wstring(LPDWORD);
std::wstring		convert2Wstring(DWORD);
std::wstring		convert2Wstring(SYSTEMTIME*);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ProcessInfo(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SPLAB1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
	
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPLAB1));

    MSG msg;

    // Цикл основного сообщения:
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
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPLAB1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPLAB1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   
   LPCWSTR mainWindowTitle = L"SP-LB2-1 Фут Д.С. Гр. 581061";
   SetWindowText(hWnd, mainWindowTitle);

   HMENU mainMenu;
   HMENU processesMenu, informationProcessesMenu;

   mainMenu = CreateMenu();
   processesMenu = CreatePopupMenu();
   informationProcessesMenu = CreatePopupMenu();

   AppendMenu(processesMenu, MF_STRING, ID_PROCESS_NOTEPAD, L"Блокнот");
   AppendMenu(processesMenu, MF_STRING, ID_PROCESS_NOTEPAD_TEXT, L"Блокнот с текстом");
   AppendMenu(processesMenu, MF_STRING, ID_PROCESS_CALCULATOR, L"Калькулятор");
   AppendMenu(processesMenu, MF_STRING, ID_PROCESS_CALCULATOR_CLOSE, L"Закрыть калькулятор");

   AppendMenu(informationProcessesMenu, MF_STRING, ID_PROCESS_INFO_CURRENT, L"Текущий процесс");
   AppendMenu(informationProcessesMenu, MF_STRING, ID_PROCESS_INFO_NOTEPAD, L"Блокнот");
   AppendMenu(informationProcessesMenu, MF_STRING, ID_PROCESS_INFO_NOTEPAD_TEXT, L"Блокнот с текстом");
   AppendMenu(informationProcessesMenu, MF_STRING, ID_PROCESS_INFO_CALCULATOR, L"Калькулятор");

   AppendMenu(mainMenu, MF_STRING | MF_POPUP, (UINT)processesMenu, L"Процессы");
   AppendMenu(mainMenu, MF_STRING | MF_POPUP, (UINT)informationProcessesMenu, L"Информация о процессах");
   
   SetMenu(hWnd, processesMenu);
   SetMenu(hWnd, informationProcessesMenu);
   SetMenu(hWnd,mainMenu);

   TCHAR buffer[MAX_PATH];
   GetModuleFileNameW(NULL, buffer, MAX_PATH);
   wcscpy_s(currentFile, buffer);

   ProcImage[0] = currentFile;
   ProcImage[1] = L"C:\\Windows\\notepad.exe";
   ProcImage[2] = L"C:\\Windows\\notepad.exe";
   ProcImage[3] = L"C:\\Windows\\System32\\calc.exe";

   GetCurrentDirectoryW(MAX_PATH,buffer);
   wcscpy_s(currentDirectory, buffer);

   wcscpy_s(CmdParam[2], (L" " + std::wstring(currentDirectory) + std::wstring(L"\\spLab1.cpp")).c_str()); //(std::wstring(currentDirectory + std::wstring(L"spLab1.cpp"))).c_str());

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND — обработать меню приложения
//  WM_PAINT — отрисовать главное окно
//  WM_DESTROY — отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
			case ID_PROCESS_NOTEPAD:
				ProcessMenu(1);
				break;
			case ID_PROCESS_NOTEPAD_TEXT:
				ProcessMenu(2);
				break;
			case ID_PROCESS_CALCULATOR:
				ProcessMenu(3);
				break;
			case ID_PROCESS_CALCULATOR_CLOSE:
				TerminateProcess(ProcHandle[3], 61);
				break;
			case ID_PROCESS_INFO_CURRENT:
			case ID_PROCESS_INFO_NOTEPAD:
			case ID_PROCESS_INFO_NOTEPAD_TEXT:
			case ID_PROCESS_INFO_CALCULATOR:
				GetProcessInfo(wmId);
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
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
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

void ProcessMenu(int index) {
	STARTUPINFO startInfo;
	ZeroMemory(&startInfo, sizeof(startInfo));
	PROCESS_INFORMATION procInfo;

	if (CreateProcess(ProcImage[index], CmdParam[index], NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
	{
		ProcHandle[index] = procInfo.hProcess;
		ThreadHandle[index] = procInfo.hThread;
		ProcId[index] = procInfo.dwProcessId;
		ThreadId[index] = procInfo.dwThreadId;

		if (index == 1)
			CloseHandle(ProcHandle[index]);
			CloseHandle(ThreadHandle[index]);
	}
}
void GetProcessInfo(int index) {
	//DialogBox(hInst, (LPCWSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)ProcessInfo);
	msgBoxCaption = L"Информация о процессе";
	DWORD procExitCode = NULL, 
		threadExitCode = NULL,
		priorityCode = NULL;

	msgWBoxContent = L"Имя файла: " + std::wstring(ProcImage[index]);
	msgWBoxContent += L"\nПараметры командной строки: " + std::wstring(CmdParam[index]);
	
	BOOL exitProcCheck = GetExitCodeProcess(ProcHandle[index], &procExitCode);
	DWORD code;
	if (exitProcCheck) {
		if (procExitCode == STILL_ACTIVE)
			msgWBoxContent += L"\nКод завершения процесса: Состояние - Активен ";
		else
			msgWBoxContent += L"\nКод завершения процесса: " + convert2Wstring(procExitCode);
	}
	else
		code = GetLastError();
	BOOL exitThreadCheck = GetExitCodeThread(ThreadHandle[index], &threadExitCode);
	if (exitThreadCheck == 1) {
		if (threadExitCode == STILL_ACTIVE)
			msgWBoxContent += L"\nКод завершения потока: Состояние - Активен ";
		else
			msgWBoxContent += L"\nКод завершения потока: " + convert2Wstring(threadExitCode);
	}
	else
		code = GetLastError();

	priorityCode = GetPriorityClass(ProcHandle[index]);
	msgWBoxContent += L"\nИдентификатор класса приоритета процесса: " + convert2Wstring(priorityCode);

	FILETIME ft[4];
	SYSTEMTIME st[4];
	TCHAR strTime[4][MAX_PATH];

	if (GetProcessTimes(ProcHandle[index], &ft[0], &ft[1], &ft[2], &ft[3]) > 0) {
		if (ft[1].dwHighDateTime == ft[1].dwLowDateTime) {
			SYSTEMTIME sTemp;
			GetSystemTime(&sTemp);
			SystemTimeToFileTime(&sTemp, &ft[1]);
		}

		for (int i = 2; i < 4; i++) {
			FileTimeToSystemTime(&ft[i], &st[i]);

			GetTimeFormat(LOCALE_NAME_USER_DEFAULT, NULL, &st[i], NULL, strTime[i], MAX_PATH);
		}

		ULARGE_INTEGER ul1;
			ul1.LowPart = ft[0].dwLowDateTime;
			ul1.HighPart = ft[0].dwHighDateTime;
		ULARGE_INTEGER ul2;
			ul2.LowPart = ft[1].dwLowDateTime;
			ul2.HighPart = ft[1].dwHighDateTime;

			ul2.QuadPart -= ul1.QuadPart;
			ul2.QuadPart /= 10*1000*1000;

		msgWBoxContent += L"\nВременные характеристики: ";
		msgWBoxContent += L"\n\tВремя жизни процесса: " + convert2Wstring(ul2.QuadPart) + L"c";
		//msgWBoxContent += L"\n\tВремя создания процесса: " + std::wstring(strTime[0]);
		//msgWBoxContent += L"\n\tВремя закрытия процесса: " + std::wstring(strTime[1]);
		msgWBoxContent += L"\n\tВремя в режиме пользователя: " + std::wstring(strTime[2]);
		msgWBoxContent += L"\n\tВремя в режиме ядра: " + std::wstring(strTime[3]);
	}

	msgBoxContent = msgWBoxContent.c_str();

	MessageBox(hWnd, msgBoxContent, msgBoxCaption, 0);
}

std::wstring convert2Wstring(LPDWORD value) {
	std::wstringstream str;
	str << value;
	return str.str();
}
std::wstring convert2Wstring(DWORD value) {
	std::wstringstream str;
	str << value;
	return str.str();
}

// Обработчик сообщений для окна "О программе".
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
