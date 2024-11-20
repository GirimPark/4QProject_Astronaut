#pragma once

namespace Curtaincall
{
	class CircularQueue
	{
	private:
		char** m_Queue;
		int m_Front;
		int m_Rear;
		int m_MaxQueueSize;

	public:
		CircularQueue(int size);
		~CircularQueue();

		bool isEmpty();
		bool isFull();

		void enQueue(char* item);
		void deQueue();

		char* Peek() const;
		int PeekSize() const;
	};
}
