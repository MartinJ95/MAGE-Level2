#include "InputComponent.h"
#include "Core/Application.h"

MouseMoveInput::MouseMoveInput() : m_lastMousePos(0.f, 0.f, 0.f)
{
}

void MouseMoveInput::Notify(const Mage::Maths::Vector3& vector)
{
	if (m_lastMousePos == vector)
		return;


	Mage::Maths::Vector3 v = vector - m_lastMousePos;

	if ((vector - m_lastMousePos).Length() > 50)
	{
		m_lastMousePos = vector;
		v = Mage::Maths::Vector3(0.f, 0.f, 0.f);
	}
	
	InputSubject::Notify(Mage::Maths::Vector3(-v.x, v.y, v.z));
	m_lastMousePos = vector;
}

ButtonInput::ButtonInput(int buttonID) : m_buttonID(buttonID)
{}

const int ButtonInput::GetButton() const
{
	return m_buttonID;
}

InputComponent::InputComponent(Entity& entity)
	: Component(entity), InputObserver(), m_mouseMoveDetection(),
	m_mouseButtonInputs(),m_keyboardButtonInputs(), m_axisInputs(),
	m_MovementVector(0.f, 0.f, 0.f), m_locked(false)
{}

void InputComponent::OnGUI(Application& app)
{}

InputComponent::~InputComponent()
{}

void InputComponent::Update(Application& app)
{}

void InputComponent::Initialize(Application& app)
{}

InputResult::InputResult() : m_app(nullptr), m_owner(nullptr)
{}

InputResult::InputResult(InputComponent& owner)
	: m_app(nullptr), m_owner(&owner)
{}

InputResult::InputResult(Application& app, InputComponent& owner)
	: m_app(&app), m_owner(&owner)
{}

AxisInput::AxisInput(Application& app, InputComponent& owner, Mage::Maths::Vector3& axis)
	: InputResult(app, owner), m_axis(axis), m_toggle(false)
{}

void AxisInput::OnNotify(bool Pressed)
{
	if (m_app == nullptr || m_owner == nullptr || m_toggle == Pressed)
		return;

	m_toggle = Pressed;
	m_owner->m_MovementVector += Pressed ? m_axis : -m_axis;

	double xPos;
	double yPos;

	glfwGetCursorPos(m_app->m_viz->getWindow(), &xPos, &yPos);

	m_owner->m_mouseMoveDetection.SetLastMousePosition(Mage::Maths::Vector3(xPos, yPos, 0));
}

void InputResult::AssignApplication(Application& app)
{
	m_app = &app;
}

void InputResult::AssignApplication(Application& app, InputComponent& owner)
{
	m_app = &app;
	m_owner = &owner;
}

