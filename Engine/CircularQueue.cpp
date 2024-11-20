#include "pch.h"
#include "CircularQueue.h"

Curtaincall::CircularQueue::CircularQueue(int maxQueueSize)
	:m_MaxQueueSize(maxQueueSize)
{
	m_Queue = new char*[maxQueueSize];
	m_Front = 0;
	m_Rear = 0;
}

Curtaincall::CircularQueue::~CircularQueue()
{
	// todo 채원 : 고치기
	for (int i = 0; i < m_Rear - m_Front; i++)
	{
		delete m_Queue[i];
	}
	delete[] m_Queue;
}

bool Curtaincall::CircularQueue::isEmpty() {
	if (m_Front == m_Rear)
	{
		return true;
	}
	return false;
}

bool Curtaincall::CircularQueue::isFull()
{
	if (m_Front == (m_Rear + 1) % m_MaxQueueSize)
	{
		return true;
	}
	return false;
}

void Curtaincall::CircularQueue::enQueue(char* item) {
	if (isFull())
		return;
	else 
	{
		m_Queue[m_Rear] = item;
		m_Rear = ++m_Rear % m_MaxQueueSize;
	}
}

void Curtaincall::CircularQueue::deQueue() {
	if (isEmpty())
		return;

	else 
		m_Front = ++m_Front % m_MaxQueueSize;
}

char* Curtaincall::CircularQueue::Peek() const
{
	return m_Queue[m_Front % m_MaxQueueSize];
}

int Curtaincall::CircularQueue::PeekSize() const
{
	return strlen(m_Queue[m_Front % m_MaxQueueSize]);
}
