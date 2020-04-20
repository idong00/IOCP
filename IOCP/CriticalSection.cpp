
/* 임계영역 */
//
// 공유 메모리 사용 시 문제되는 블럭
// 어셈블리어 연산과정에서 스레드 컨텍스트 스위칭 발생
// 레지스터에서 store를 하지 못하여(스택에 저장)
// 의도하지 않은 값이 나옴

/* 해결방법 */
//
// 접근
// 1. 유저 동기화
// 속도 빠름 ( 유저, 커널 이동이 적음 )
// 크리티컬 섹션
// 인터록

// 2. 커널 동기화
// 기능 많음
// 느림 ( 유저, 커널 이동 빈번 - 스레드를 가리키고 있어 스위칭이 잦음 )
// 세마포어 - 키 여러개
// 뮤텍스   - 키 1개
// 이름있는 뮤텍스

//
// 순서
// 타이머
// 이벤트

#include <iostream>
#include <Windows.h>
#include <conio.h>

struct CCriticalSection
	: public CRITICAL_SECTION
{
	CCriticalSection() { InitializeCriticalSection(this); }
	~CCriticalSection() { DeleteCriticalSection(this); }
};

// RAII
class CCriticalSectionLock
{
public:
	CCriticalSectionLock(CRITICAL_SECTION& cs) :
		m_pcs(&cs)
	{
		EnterCriticalSection(m_pcs);
	}
	~CCriticalSectionLock()
	{
		if (m_pcs != nullptr)
			LeaveCriticalSection(m_pcs);
	}
	explicit operator bool() { return true; } // 명시적

protected:
	CRITICAL_SECTION* m_pcs;
};

#define CSLOCK(cs_) if(CCriticalSectionLock lock = cs_)

int g_value = 0;
CCriticalSection g_csValue;

DWORD WINAPI TreadProc(LPVOID);

int main()
{
	HANDLE hThread;
	DWORD dwTreadID;

	hThread = CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)TreadProc,
		NULL,
		0,
		&dwTreadID);

	while (TRUE)
	{
		for (int i = 0; i < 1000000; ++i)
		{
			CSLOCK(g_csValue)
			{
				// 보호
				g_value -= 1;	// 임계영역
			}
		}
		break;
	}

	_getch();

	std::cout << g_value << std::endl;

	return 0;
}

DWORD WINAPI TreadProc(LPVOID)
{
	while (TRUE)
	{
		for (int i = 0; i < 1000000; ++i)
		{
			CSLOCK(g_csValue)
			{
				// 보호
				g_value += 1;	// 임계영역
			}
		}
		break;
	}
	return 0;
}