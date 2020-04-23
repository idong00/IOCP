
/* 중요 */
/* 구조체 패딩 */

// CPU와 메모리는 버스를 통해 데이터를 주고 받음
// 가장 효율적인 메모리 크기( 4 바이트 ) - 32비트
// 컴파일러가 alignment 실시

// 서버 통신에서 메모리 alignment를 잘 맞춰야함

// 세팅에서 바꾸기 가능
// 일반적으로 디폴트
// 프로그래밍 단계에서 설정

#include <iostream>

// 지금부터 선언하는 모든 구조체를 alignment 시킴(컴파일러에게)
// 1 바이트 단위
#pragma pack(push, 1)

struct CData
{
	char m_char;
	float m_data;
};

// 끝
#pragma pack(pop)

int main()
{
	int size = sizeof(CData);
	std::cout << size;

	return 0;
}