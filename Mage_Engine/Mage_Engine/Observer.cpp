#include "Observer.h"

void Observer::OnNotify()
{

}

void InputObserver::OnNotify(Mage::Maths::Vector3& vector)
{
}

void InputObserver::OnNotify(bool Pressed)
{
}

void Subject::Subscribe(const IObserver* newListener)
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

void Subject::UnSubscribe(const IObserver* listener)
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

void InputSubject::Notify(const Mage::Maths::Vector3& vector)
{
	std::vector<IObserver*> observers;
	for (int i = 0; i < m_Observers.size(); i++)
	{
		/*InputObserver* o = dynamic_cast<InputObserver*>(m_Observers[i]);
		if (o == nullptr)
			continue;

		o->OnNotify(vector);
		*/
		InputObserver* o{ nullptr };
		IsInputObserver(i, o) ? [](InputObserver* o, Mage::Maths::Vector3 vector) {o->OnNotify(vector); }(o, vector) : []() {}();

	}
}

void InputSubject::Notify(const bool Pressed)
{
	for (int i = 0; i < m_Observers.size(); i++)
	{
		/*InputObserver* o = dynamic_cast<InputObserver*>(m_Observers[i]);
		if (o == nullptr)
			continue;

		o->OnNotify(Pressed);
		*/
		InputObserver* o{ nullptr };
		IsInputObserver(i, o) ? [](InputObserver* o, bool Pressed) {o->OnNotify(Pressed); }(o, Pressed) : []() {}();
	}
}

inline bool InputSubject::IsInputObserver(const int i, InputObserver* o)
{
	o = dynamic_cast<InputObserver*>(m_Observers[i]);
	return o == nullptr ? false : true;
}
