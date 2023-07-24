#pragma once

#include <vector>
#include "Maths/Vector.h"

class IObserver
{
public:
	virtual void OnNotify() = 0;
};

class IInputObserver
{
public:
	virtual void OnNotify(Mage::Maths::Vector3& vector) = 0;
	virtual void OnNotify(bool Pressed) = 0;
};

class ISubject
{
public:
	virtual void Subscribe(const IObserver* newListener) = 0;
	virtual void UnSubscribe(const IObserver* listener) = 0;
	virtual void UnSubscribeAll() = 0;
	virtual void Notify() = 0;
protected:
	std::vector<IObserver*> m_Observers;
};

class IInputSubject
{
	virtual void Notify(const Mage::Maths::Vector3& vector) = 0;
	virtual void Notify(const bool Pressed) = 0;
};

class Observer : public IObserver
{
	virtual void OnNotify() override;
};

class InputObserver : public Observer, public IInputObserver
{
public:
	virtual void OnNotify(Mage::Maths::Vector3& vector) override;
	virtual void OnNotify(bool Pressed) override;
};

class Subject :public ISubject
{
	virtual void Subscribe(const IObserver* newListener) override;
	virtual void UnSubscribe(const IObserver* listener) override;
	virtual void UnSubscribeAll() override;
	virtual void Notify() override;
};

class InputSubject : public Subject, public IInputSubject
{
public:
	virtual void Notify(const Mage::Maths::Vector3& vector) override;
	virtual void Notify(const bool Pressed) override;
private:
	inline bool IsInputObserver(const int i, InputObserver* o);
};