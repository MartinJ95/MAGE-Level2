#pragma once

#include "Core/Component.h"
#include "Core/Transform.h"
#include "InputComponent.h"
#include "Observer.h"

//todo
// - make proper input component
// - move struct to proper input component
// - bind on notify bool in inputComponent to the press and release of the keycode
class InputButton : InputSubject
{
public:
	InputButton(int keyCode) : m_keyCode(keyCode)
	{}
public:
private:
	int m_keyCode;
};

class EditorCam : InputObserver, Component
{
public:
	virtual void OnNotify(Mage::Maths::Vector3& vector) override;

public:
	bool m_locked{false};
};

