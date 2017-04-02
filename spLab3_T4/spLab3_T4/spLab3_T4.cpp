// Консольное приложение, использующее ожидающий таймер
// Через 1 мин. после старта и каждые последующие 20 сек. выводит фразу и время

#define _WIN32_WINDOWS 0x0410 
#define WINVER 0x0400


#include "stdafx.h"
#include <windows.h>
#include <stdio.h>

void main(void)
{
	const int nTimerUnitsPerSecond = 10000000;
	int timerRunIndex = 0;
	SYSTEMTIME st;
	LARGE_INTEGER li,liClose;

	HANDLE hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
	HANDLE hCloseTimer = CreateWaitableTimer(NULL, FALSE, NULL);

	HWND notepad;

	LPCWSTR ProcImage = L"C:\\Windows\\notepad.exe";

	GetLocalTime(&st);
	li.QuadPart = -(15 * nTimerUnitsPerSecond);
	liClose.QuadPart = -(10 * nTimerUnitsPerSecond);

	STARTUPINFO startInfo;
	ZeroMemory(&startInfo, sizeof(startInfo));
	PROCESS_INFORMATION procInfo;

	if (SetWaitableTimer(hTimer, &li, 15 * 1000, NULL, NULL, FALSE)) {
		while (TRUE) {
			WaitForSingleObject(hTimer, INFINITE);
			timerRunIndex++;
			if (CreateProcess(ProcImage, NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &startInfo, &procInfo) == TRUE)
			{
				notepad = FindWindow(L"Notepad", NULL);
				SendMessage(notepad,WM_SETTEXT,0,(LPARAM)"Запуск №"+timerRunIndex);

				if (SetWaitableTimer(hCloseTimer, &liClose, 0, NULL, NULL, FALSE)) {
					WaitForSingleObject(hCloseTimer, INFINITE);
					TerminateProcess(procInfo.hProcess, 0);
				}

			}
		}
	}
}