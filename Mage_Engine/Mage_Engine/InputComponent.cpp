#include "InputComponent.h"
#include "Core/Application.h"

MouseMoveInput::MouseMoveInput() : m_lastMousePos(0.f, 0.f, 0.f)
{
}

void MouseMoveInput::Notify(const Mage::Maths::Vector3& vector)
{
	if (m_lastMousePos == vector)
		return;

	m_lastMousePos = vector;
	InputSubject::Notify(vector);
}

ButtonInput::ButtonInput(int buttonID) : m_buttonID(buttonID)
{
}

void InputComponent::Update(Application& app)
{
}

