
/* Ŭ������ �ϳ��� å���� ������ �Ѵ� */

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

	// ��� ��Ŷ�� ȣ��
	// �������̽��� ����
	// �ܺ�(����)���� ó��
	template <typename FUNCTOR>
	void ProcessAllPacket(FUNCTOR callback)
	{
		while (!m_queue.empty())
		{
			PacketPtr spPacket;
			CSLOCK(m_csQueue)
			{
				// ���Ⱑ �Ӱ迵��
				// while ������ �����ϸ� �ݹ� ó���ϴ� ����
				// �ٸ� �����尡 ��� ���
				spPacket = m_queue.front();
				m_queue.pop();
			}

			// ����Ʈ�����ͷ� �̹� ������ ������
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
		// ���� ��ü��
		// OnPacket ȣ��
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