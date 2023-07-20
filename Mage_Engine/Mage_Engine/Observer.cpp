#include "Observer.h"

void Observer::OnNotify()
{

}

void InputObserver::OnNotify()
{
}

void InputObserver::OnNotify(Mage::Maths::Vector3& vector)
{
}

void InputObserver::OnNotify(bool Pressed)
{
}

void Subject::Subscribe(IObserver* newListener)
{
	for (int i = 0; i < m_Observers.size(); i++)
	{
		if (m_Observers[i] == newListener)
		{
			return;
		}
	}

	m_Observers.emplace_back(newListener);
}

void Subject::UnSubscribe(IObserver* listener)
{
	for (int i = 0; i < m_Observers.size(); i++)
	{
		if (m_Observers[i] == listener)
		{
			m_Observers.erase(m_Observers.begin()+i);
			return;
		}
	}
}

void Subject::UnSubscribeAll()
{
	m_Observers.clear();
}

void Subject::Notify()
{
	for (int i = 0; i < m_Observers.size(); i++)
	{
		m_Observers[i]->OnNotify();
	}
}

void InputSubject::Notify()
{
	for (int i = 0; i < m_Observers.size(); i++)
	{
		m_Observers[i]->OnNotify();
	}
}

void InputSubject::Notify(Mage::Maths::Vector3& vector)
{
	for (int i = 0; i < m_Observers.size(); i++)
	{
		InputObserver *o = dynamic_cast<InputObserver*>(m_Observers[i]);
		if (o == nullptr)
			continue;

		o->OnNotify(vector);
	}
}

void InputSubject::Notify(bool Pressed)
{
	for (int i = 0; i < m_Observers.size(); i++)
	{
		InputObserver* o = dynamic_cast<InputObserver*>(m_Observers[i]);
		if (o == nullptr)
			continue;

		o->OnNotify(Pressed);
	}
}
