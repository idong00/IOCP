
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
	void AddPacket(PacketPtr spPacket)
	{
		m_queue.push(spPacket);
	}

	// 모든 패킷을 호출
	// 인터페이스만 제공
	// 외부(세션)에서 처리
	template <typename FUNCTOR>
	void ProcessAllPacket(FUNCTOR callback)
	{
		while (!m_queue.empty())
		{
			PacketPtr spPacket = m_queue.front();
			m_queue.pop();
			callback(spPacket);
		}
	}
};

class CSession
{
public:
	// 세션 큐에 넣음
	void AddPacket(PacketPtr spPacket)
	{
		m_cQueue.AddPacket(spPacket);
	}

	void Update()
	{
		// 현재 객체의
		// OnPacket 호출
		m_cQueue.ProcessAllPacket(boost::bind(&CSession::OnPacket, this, _1));
	}

	virtual void OnPacket(PacketPtr spPacket)
	{
		std::cout << __FUNCTION__ << '\n';
	}

private:
	CQueue m_cQueue;
};

int main()
{
	PacketPtr spPacket;
	spPacket.reset(new Packet());

	CSession session;
	session.AddPacket(spPacket);
	session.AddPacket(spPacket);
	//..
	session.Update();

	return 0;
}