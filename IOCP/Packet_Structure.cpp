
/* �߿� */
/* ����ü �е� */

// CPU�� �޸𸮴� ������ ���� �����͸� �ְ� ����
// ���� ȿ������ �޸� ũ��( 4 ����Ʈ ) - 32��Ʈ
// �����Ϸ��� alignment �ǽ�

// ���� ��ſ��� �޸� alignment�� �� �������

// ���ÿ��� �ٲٱ� ����
// �Ϲ������� ����Ʈ
// ���α׷��� �ܰ迡�� ����

#include <iostream>

// ���ݺ��� �����ϴ� ��� ����ü�� alignment ��Ŵ(�����Ϸ�����)
// 1 ����Ʈ ����
#pragma pack(push, 1)

struct CData
{
	char m_char;
	float m_data;
};

// ��
#pragma pack(pop)

int main()
{
	int size = sizeof(CData);
	std::cout << size;

	return 0;
}