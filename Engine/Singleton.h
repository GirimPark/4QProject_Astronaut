#pragma once

///<summary>
///싱글톤 패턴으로 만들 클래스는 해당 클래스를 상속 받아 사용한다.
///rawPtr이므로 반드시 Finalize 필요
///</summary>

template <typename T>
class Singleton
{
protected:
	Singleton() = default;

	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	static T* m_pInstance;

public:
	static T* GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new T;

		return m_pInstance;
	}

	virtual void Finalize() abstract
	{
		delete m_pInstance;
	}
};

template <typename T>
T* Singleton<T>::m_pInstance = nullptr;