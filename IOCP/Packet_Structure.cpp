
/* 중요 */
/* 구조체 패딩 */

// CPU와 메모리는 버스를 통해 데이터를 주고 받음
// 가장 효율적인 메모리 크기( 4 바이트 ) - 32비트
// 컴파일러가 alignment 실시

// 서버 통신에서 메모리 alignment를 잘 맞춰야함

// 세팅에서 바꾸기 가능
// 일반적으로 디폴트
// 프로그래밍 단계에서 설정


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


/* 중요 */
/* 직렬화 ( serialization ) */
// CData를 전송하기 위해 버퍼에 채워야함
// 일렬로 세워 버퍼에 채워야함
// 부스트 사용

#include <iostream>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

// 지금부터 선언하는 모든 구조체를 alignment 시킴(컴파일러에게)
// 1 바이트 단위
#pragma pack(push, 1)

struct CData
{
	char m_char;
	float m_data;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & m_char;   // 데이터를 Archive에 줌
		ar & m_data;
	}
};

#pragma pack(pop)

int main()
{
	// CData를 stringstream 버퍼에 채울예정
	CData d;
	d.m_char = 1;
	d.m_data = 2.3f;

	std::stringstream ss;

	boost::archive::text_oarchive oa{ ss };	// archive에 버퍼를 연결
	oa << d;	// archive를 이용해 버퍼에 채움

	// ss 버퍼를 보냈다고 가정

	CData d2;

	// 서버측이라 가정
	boost::archive::text_iarchive ia{ ss };
	ia >> d2 ;	// deserialization

	return 0;
}