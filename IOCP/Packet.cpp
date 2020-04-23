
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
	std::vector<char>	m_buffer;	// �Ϲ����� ���� ����
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
	ar & a.m_nSenderId;		// �����͸� Archive�� ��
	ar & a.m_usPacketId;
	ar & a.m_buffer;		// ���͸� ����ȭ�ϱ� ���� ��� �����ؾ���
}

template <typename T>
void BufferToPacket(std::vector<char>& buffer, T& data)
{
	// ������ϰ� T�� ��Ŷ�� ����

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
	// ��Ŷ �����͸� ���۷� ����

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
	// ��Ŷ �����͸� ���۷� ����

	boost::archive::text_oarchive oa{ ss_ };
	oa << packet_;
}//PacketToBuffer()

int main()
{
	CPacket     packets[2];		// ��Ŷ 2��

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
	// �޴� ��

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
/* �߿� */
/* ����ü �е� */

// CPU�� �޸𸮴� ������ ���� �����͸� �ְ� ����
// ���� ȿ������ �޸� ũ��( 4 ����Ʈ ) - 32��Ʈ
// �����Ϸ��� alignment �ǽ�

// ���� ��ſ��� �޸� alignment�� �� �������

// ���ÿ��� �ٲٱ� ����
// �Ϲ������� ����Ʈ
// ���α׷��� �ܰ迡�� ����


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


/* �߿� */
/* ����ȭ ( serialization ) */
// CData�� �����ϱ� ���� ���ۿ� ä������
// �Ϸķ� ���� ���ۿ� ä������
// �ν�Ʈ ���

//#include <iostream>
//#include <sstream>
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
//
//// ���ݺ��� �����ϴ� ��� ����ü�� alignment ��Ŵ(�����Ϸ�����)
//// 1 ����Ʈ ����
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
//		ar & m_char;   // �����͸� Archive�� ��
//		ar & m_data;
//	}
//};
//
//#pragma pack(pop)
//
//int main()
//{
//	// CData�� stringstream ���ۿ� ä�￹��
//	CData d;
//	d.m_char = 1;
//	d.m_data = 2.3f;
//
//	std::stringstream ss;
//
//	boost::archive::text_oarchive oa{ ss };	// archive�� ���۸� ����
//	oa << d;	// archive�� �̿��� ���ۿ� ä��
//
//	// ss ���۸� ���´ٰ� ����
//
//	CData d2;
//
//	// �������̶� ����
//	boost::archive::text_iarchive ia{ ss };
//	ia >> d2 ;	// deserialization
//
//	return 0;
//}


// ������ Ŭ���̾�Ʈ �Ǵ� �ٸ� ������ ���� ��Ŷ�� ����
// DB ����(�߾�)�� ���� ���´��� �˾ƾ��� ( sender ID �ʿ� )
// Ŭ���̾�Ʈ�� ä��, ������ �� �پ��� ����(��Ŷ)�� ���� ( packet ID )
// ��� �����ʹ� ����ȭ�Ͽ� ���ۿ� �� ( ���� )