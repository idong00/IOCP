
/* Ŭ������ �ϳ��� å���� ������ �Ѵ� */

#include <iostream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <algorithm>
#include <queue>

struct Packet;
typedef boost::shared_ptr<Packet> PacketPtr;
struct Packet
{

};

// �߸��� ����
class CQueue
{
private:
	std::queue<PacketPtr> m_queue;

public:
	// ���� ť�� ����
	void AddPacket(PacketPtr spPacket)
	{
		m_queue.push(spPacket);
	}

	// ��� ��Ŷ�� ȣ��
	void ProcessAllPacket()
	{
		while (!m_queue.empty())
		{
			PacketPtr spPacket = m_queue.front();
			m_queue.pop();
			OnPacket(spPacket);
		}
	}

	// ���� ��Ŷ�� ��� �޴´ٸ� �����Լ��� ��������
	void OnPacket(PacketPtr spPacket)
	{
		std::cout << __FUNCTION__ << '\n';
	}
};

int main()
{
	PacketPtr spPacket;
	spPacket.reset(new Packet());

	CQueue queue;
	queue.AddPacket(spPacket);
	queue.AddPacket(spPacket);
	// ..
	queue.ProcessAllPacket();

	return 0;
}