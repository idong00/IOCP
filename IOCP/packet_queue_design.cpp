
/* 클래스는 하나의 책임을 가져야 한다 */

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

// 잘못된 설계
class CQueue
{
private:
	std::queue<PacketPtr> m_queue;

public:
	// 세션 큐에 넣음
	void AddPacket(PacketPtr spPacket)
	{
		m_queue.push(spPacket);
	}

	// 모든 패킷을 호출
	void ProcessAllPacket()
	{
		while (!m_queue.empty())
		{
			PacketPtr spPacket = m_queue.front();
			m_queue.pop();
			OnPacket(spPacket);
		}
	}

	// 만약 패킷이 상속 받는다면 가상함수로 만들어야함
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