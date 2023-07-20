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
	virtual void Subscribe(IObserver* newListener) = 0;
	virtual void UnSubscribe(IObserver* listener) = 0;
	virtual void UnSubscribeAll() = 0;
	virtual void Notify() = 0;
	
protected:
	std::vector<IObserver*> m_Observers;
};

class IInputSubject
{
	virtual void Notify(Mage::Maths::Vector3& vector) = 0;
	virtual void Notify(bool Pressed) = 0;
};

class Observer : IObserver
{
	virtual void OnNotify() override;
};

class InputObserver : IObserver, IInputObserver
{
public:
	virtual void OnNotify() override;
	virtual void OnNotify(Mage::Maths::Vector3& vector) override;
	virtual void OnNotify(bool Pressed) override;
};

class Subject : ISubject
{
	virtual void Subscribe(IObserver* newListener) override;
	virtual void UnSubscribe(IObserver* listener) override;
	virtual void UnSubscribeAll() override;
	virtual void Notify() override;
};

class InputSubject : ISubject, IInputSubject
{
	virtual void Notify() override;
	virtual void Notify(Mage::Maths::Vector3& vector) override;
	virtual void Notify(bool Pressed) override;
};