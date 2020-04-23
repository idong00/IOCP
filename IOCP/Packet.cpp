
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <sstream>


enum EPacketType
{
	ECLGS_VERIFY_ACCOUNT_REQ,
	ECLGS_LOGIN,
};

struct CPacketVerifyAccount
{
	std::string m_login;
	int m_id;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & m_login;
		ar & m_id;
	}
};

struct CPacketLogin
{
	std::string m_login;
	std::string m_password;
	int m_id;
	int m_age;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & m_login;
		ar & m_password;
		ar & m_id;
		ar & m_age;
	}
};

template<typename CharT, typename TraitsT = std::char_traits<CharT> >
class vectorwrapbuf :
	public std::basic_streambuf<CharT, TraitsT>
{
public:
	vectorwrapbuf(std::vector<CharT>& vec)
	{
		vectorwrapbuf::setg(vec.data(), vec.data(), vec.data() + vec.size());
	}
};

#pragma pack(push, 1)

class CPacket;
typedef boost::shared_ptr<CPacket> CPacketPtr;
class CPacket
{
public:
	template<class T>
	void SetData(unsigned int nSenderId, unsigned short usPacketId, const T& data);

	unsigned int		m_nSenderId;
	unsigned short		m_usPacketId;
	std::vector<char>	m_buffer;	// 일반적인 버퍼 형식
};

#pragma pack(pop)

template<class T>
void CPacket::SetData(unsigned int nSenderId, unsigned short usPacketId, const T & data_)
{
	m_nSenderId = nSenderId;
	m_usPacketId = usPacketId;

	std::stringstream   ss;
	boost::archive::text_oarchive oa{ ss };
	oa << data_;

	std::string str = ss.str();
	m_buffer.reserve(str.size());
	m_buffer.assign(str.begin(), str.end());
}

template<typename Archive>
void serialize(Archive& ar, CPacket& a, const unsigned int version)
{
	ar & a.m_nSenderId;		// 데이터를 Archive에 줌
	ar & a.m_usPacketId;
	ar & a.m_buffer;		// 벡터를 직렬화하기 위해 헤더 선언해야함
}

template <typename T>
void BufferToPacket(std::vector<char>& buffer, T& data)
{
	// 복사안하고 T로 패킷을 보냄

	// alternative (slow) implementation. jintaeks on 2017-08-24_20-08
	//std::stringstream ss;
	//std::copy(buffer.begin(), buffer.end(), std::ostream_iterator<char>(ss));
	//boost::archive::text_iarchive ia{ ss };
	//ia >> data;

	vectorwrapbuf<char> databuf(buffer);
	std::istream is(&databuf);
	boost::archive::text_iarchive ia{ is };
	ia >> data;
}//BufferToPacket()


template <typename T>
void BufferToPacket(std::stringstream& ss_, T& packet_)
{
	boost::archive::text_iarchive ia{ ss_ };
	ia >> packet_;
}//BufferToPacket()


template<typename T>
void PacketToBuffer(T& packet_, std::vector<char>& buffer_)
{
	// 패킷 데이터를 버퍼로 보냄

	std::stringstream   ss;
	boost::archive::text_oarchive oa{ ss };
	oa << packet_;

	// set [out] parameter
	std::string& str = ss.str();
	buffer_.reserve(str.size());
	buffer_.assign(str.begin(), str.end());
}//PacketToBuffer()


template<typename T>
void PacketToBuffer(T& packet_, std::stringstream& ss_)
{
	// 패킷 데이터를 버퍼로 보냄

	boost::archive::text_oarchive oa{ ss_ };
	oa << packet_;
}//PacketToBuffer()

int main()
{
	CPacket     packets[2];		// 패킷 2개

	CPacketVerifyAccount    verifyAccount;
	{
		verifyAccount.m_login = "KDH";
		verifyAccount.m_id = 48;
		packets[0].SetData(0, ECLGS_VERIFY_ACCOUNT_REQ, verifyAccount);
	}


	CPacketLogin        login;
	{
		login.m_login = "KDH\0hello";
		login.m_password = "hello world";
		login.m_id = 99;
		login.m_age = 48;
		packets[1].SetData(0, ECLGS_LOGIN, login);
	}

	//
	// 받는 쪽

	// write to archive
	//
	std::stringstream ss;
	PacketToBuffer(packets[1], ss);

	// read from archive
	//
	CPacket     packetNew;
	BufferToPacket(ss, packetNew);

	if (packetNew.m_usPacketId == ECLGS_VERIFY_ACCOUNT_REQ)
	{
		CPacketVerifyAccount    verifyNew;
		BufferToPacket(packetNew.m_buffer, verifyNew);
	}
	else if (packetNew.m_usPacketId == ECLGS_LOGIN)
	{
		CPacketLogin    loginNew;
		BufferToPacket(packetNew.m_buffer, loginNew);
	}//if.. else if..

	return 0;
}
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

//#include <iostream>
//#include <sstream>
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
//
//// 지금부터 선언하는 모든 구조체를 alignment 시킴(컴파일러에게)
//// 1 바이트 단위
//#pragma pack(push, 1)
//
//struct CData
//{
//	char m_char;
//	float m_data;
//
//	template<typename Archive>
//	void serialize(Archive& ar, const unsigned int version)
//	{
//		ar & m_char;   // 데이터를 Archive에 줌
//		ar & m_data;
//	}
//};
//
//#pragma pack(pop)
//
//int main()
//{
//	// CData를 stringstream 버퍼에 채울예정
//	CData d;
//	d.m_char = 1;
//	d.m_data = 2.3f;
//
//	std::stringstream ss;
//
//	boost::archive::text_oarchive oa{ ss };	// archive에 버퍼를 연결
//	oa << d;	// archive를 이용해 버퍼에 채움
//
//	// ss 버퍼를 보냈다고 가정
//
//	CData d2;
//
//	// 서버측이라 가정
//	boost::archive::text_iarchive ia{ ss };
//	ia >> d2 ;	// deserialization
//
//	return 0;
//}


// 서버는 클라이언트 또는 다른 서버로 부터 패킷을 받음
// DB 서버(중앙)는 누가 보냈는지 알아야함 ( sender ID 필요 )
// 클라이언트는 채팅, 아이템 등 다양한 정보(패킷)를 보냄 ( packet ID )
// 모든 데이터는 직렬화하여 버퍼에 들어감 ( 버퍼 )