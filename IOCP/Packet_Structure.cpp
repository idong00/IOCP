
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

#include <iostream>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

// ���ݺ��� �����ϴ� ��� ����ü�� alignment ��Ŵ(�����Ϸ�����)
// 1 ����Ʈ ����
#pragma pack(push, 1)

struct CData
{
	char m_char;
	float m_data;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & m_char;   // �����͸� Archive�� ��
		ar & m_data;
	}
};

#pragma pack(pop)

int main()
{
	// CData�� stringstream ���ۿ� ä�￹��
	CData d;
	d.m_char = 1;
	d.m_data = 2.3f;

	std::stringstream ss;

	boost::archive::text_oarchive oa{ ss };	// archive�� ���۸� ����
	oa << d;	// archive�� �̿��� ���ۿ� ä��

	// ss ���۸� ���´ٰ� ����

	CData d2;

	// �������̶� ����
	boost::archive::text_iarchive ia{ ss };
	ia >> d2 ;	// deserialization

	return 0;
}