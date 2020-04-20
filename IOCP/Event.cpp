
/* Event */
//
// 스레드 간 타이밍 동기화

// WaitForSingleObject
// signal 상태를 기다림
// non signal 상태로 만듬 ( 런 )

// SetEvent
// signal 상태로 만듬 ( 블럭 상태 )

/* 수동 / 자동 이벤트 */
//
// 수동 : 명시적으로 SetEvent
// 자동 : signal 상태가 되면 자동

////////////////////////////////////////////////
////////////////////////////////////////////////

/* deadlock */
//
// 서로를 기다리고 있는 상태 

#include <iostream>
#include <Windows.h>

HANDLE g_hEvents[2];
int g_value = 1;

DWORD WINAPI TreadProc(LPVOID);

int main()
{
	HANDLE hThread;
	DWORD i, dwEvent, dwTreadID;

	for (int i = 0; i < 2; i++)
	{
		g_hEvents[i] = CreateEvent(
			NULL,	// default security attributes
			FALSE,	// auto reset event object
			FALSE,	// initial state is non-signaled
			NULL);	// unnamed object
	}

	hThread = CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)TreadProc,
		NULL,
		0,
		&dwTreadID);

	Sleep(2000);

	while (g_value < 10)
	{
		std::cout << g_value << std::endl;
		g_value++;
		Sleep(1000);
		SetEvent(g_hEvents[1]);
		WaitForSingleObject(g_hEvents[0], INFINITE);
	}

	for (int i = 0; i < 2; i++)
	{
		// 중요
		// 카운터 --
		CloseHandle(g_hEvents[i]);
	}

	return 0;
}

DWORD WINAPI TreadProc(LPVOID lpParam)
{
	while (g_value < 10)
	{
		WaitForSingleObject(g_hEvents[1], INFINITE);
		std::cout << g_value << std::endl;
		g_value++;
		Sleep(1000);
		SetEvent(g_hEvents[0]);
	}
	return 0;
}