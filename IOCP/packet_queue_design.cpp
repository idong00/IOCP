
/* 클래스는 하나의 책임을 가져야 한다 */

#include <iostream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <algorithm>
#include <queue>
#include "CriticalSection.h"


struct Packet;
typedef boost::shared_ptr<Packet> PacketPtr;
struct Packet // object pool
{
	// need to apply serialization
};

class CQueue
{
private:
	std::queue<PacketPtr>	m_queue;
	CCriticalSection		m_csQueue;

public:
	void AddPacket(PacketPtr spPacket)
	{
		CCriticalSectionLock lock(m_csQueue);
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
			PacketPtr spPacket;
			CSLOCK(m_csQueue)
			{
				// 여기가 임계영역
				// while 위에서 선언하면 콜백 처리하는 동안
				// 다른 스레드가 계속 대기
				spPacket = m_queue.front();
				m_queue.pop();
			}

			// 스마트포인터로 이미 스레드 세이프
			callback(spPacket);
		}
	}
};

class CSession
{
public:
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