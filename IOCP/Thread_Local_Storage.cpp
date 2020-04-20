
/* Thread Local Storage */
//
// 전역변수를 여러 스레드에서 사용

#include <iostream>
#include <thread>
#include <Windows.h>

// 스레드마다 초기화
//__declspec(thread) int g_value = 4;
thread_local int* g_pData = nullptr;

void foo(int i)
{
	if (g_pData == nullptr)
	{
		g_pData = new int[11];
		printf("g_pData allocated %i\r\n", i);
	}

	Sleep(500);
	printf("%i\r\n", i);
	Sleep(500);
	if (g_pData != nullptr)
	{
		delete[] g_pData;
		printf("g_pData destroyed %i\r\n", i);
	}
}

int main()
{
	std::thread first(foo, 1); // spawn
	std::thread second(foo, 3); // spawn

	std::cout << "main, foo wnd foo new execute concurrently..\n";

	// synchronize threads
	first.join();  // pause until first finishes
	second.join(); // pause until second finishes

	std::cout << "foo and bar completed\n";

	return 0;
}