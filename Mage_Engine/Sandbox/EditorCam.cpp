#include "pch.h"
#include "EditorCam.h"
#include "Core/Entity.h"
#include "Core/Application.h"

void MouseMoveInputRotation::OnNotify(Mage::Maths::Vector3& vector)
{
	if (m_owner == nullptr)
		return;

	Transform* t = m_owner->m_entity->getComponent<Transform>();

	if (t == nullptr || m_app == nullptr)
		return;

	float pitchChange = -vector.y;

	if (t->m_forward.Dot(m_app->m_worldUp) > 0.7 && pitchChange < 0)
		pitchChange = 0;

	else if (t->m_forward.Dot(m_app->m_worldUp) < 0.3 && pitchChange > 0)
		pitchChange = 0;

	//rotating by euler
	t->Rotate(Mage::Maths::Vector3(-vector.x, pitchChange, 0));

	//rotating by quaternion
	//t->Rotate(Mage::Maths::Quaternion(-vector.x, m_app->m_worldUp));
	//t->Rotate(Mage::Maths::Quaternion(pitchChange, m_app->m_worldForward.Cross(m_app->m_worldUp)));

	//t->m_quatRotation = t->m_quatRotation * Mage::Maths::Quaternion(-vector.x, m_app->m_worldUp);
	//t->m_quatRotation = t->m_quatRotation * Mage::Maths::Quaternion(pitchChange, m_app->m_worldForward.Cross(m_app->m_worldUp));

	//t->m_rotation += vector;
	//t->updateDirection();
}

void EditorCamLockCursorResult::OnNotify(bool Pressed)
{
	if (m_app == nullptr || m_owner == nullptr)
		return;

	m_owner->m_locked = Pressed;
	glfwSetInputMode(m_app->m_viz->getWindow(), GLFW_CURSOR, Pressed ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
}

EditorCam::EditorCam(Entity& entity)
	: InputComponent(entity), m_mouseMoveResult(), m_cursorLock()
{
}

void EditorCam::Update(Application& app)
{
	double x, y;
	GLFWwindow* window = app.m_viz->getWindow();

	if (window == nullptr)
		return;

	auto checkButtons = [](std::vector<ButtonInput>& buttons, GLFWwindow* w, int(*getInput)(GLFWwindow*, int))
	{
		for (int i = 0; i < buttons.size(); i++)
		{
			int input = getInput(w, buttons[i].GetButton());
			switch (input)
			{
			case GLFW_PRESS:
				buttons[i].Notify(true);
				break;
			case GLFW_RELEASE:
				buttons[i].Notify(false);
				break;
			default:
				break;
			}
		}
	};

	checkButtons(m_mouseButtonInputs, window, glfwGetMouseButton);
	checkButtons(m_keyboardButtonInputs, window, glfwGetKey);

	if (!m_locked)
		return;

	glfwGetCursorPos(window, &x, &y);
	m_mouseMoveDetection.Notify(Mage::Maths::Vector3(y, x, 0));

	Transform* t = m_entity->getComponent<Transform>();

	if (t == nullptr)
		return;

	t->m_position += t->m_forward * m_MovementVector.z + t->m_forward.Cross(app.m_worldUp) * m_MovementVector.x + app.m_worldUp * m_MovementVector.y;

	/*
	auto checkButton = [](ButtonInput& i, GLFWwindow* w, const int button, int(*getInput)(GLFWwindow*, int))
	{
		int input = getInput(w, button);
		switch (input)
		{
		case GLFW_PRESS:
			i.Notify(true);
			break;
		case GLFW_RELEASE:
			i.Notify(false);
		default:
		}
	};
	*/

	/*
	for (int i = 0 ; i < m_mouseButtonInputs.size(); i++)
	{
		checkButton(m_mouseButtonInputs[i], window, m_mouseButtonInputs[i].GetButton(), glfwGetMouseButton);
	}

	for (int i = 0; i < m_keyboardButtonInputs.size(); i++)
	{
		checkButton(m_keyboardButtonInputs[i], window, m_keyboardButtonInputs[i].GetButton(), glfwGetKey);
	}
	*/

	/*
	for (int i = 0; i < m_mouseButtonInputs.size(); i++)
	{
		int input = glfwGetMouseButton(window, m_mouseButtonInputs[i].GetButton());
		switch (input)
		{
		case GLFW_PRESS:
			m_mouseButtonInputs[i].Notify(true);
			break;
		case GLFW_RELEASE:
			m_mouseButtonInputs[i].Notify(false);
		default:
		}
	}

	for (int i = 0; i < m_keyboardButtonInputs.size(); i++)
	{
		int input = glfwGetKey(window, m_keyboardButtonInputs[i].GetButton());
		switch (input)
		{
		case GLFW_PRESS:
			m_mouseButtonInputs[i].Notify(true);
			break;
		case GLFW_RELEASE:
			m_mouseButtonInputs[i].Notify(false);
		default:
		}
	}
	*/
}

void EditorCam::OnGUI(Application& app)
{
	app.m_viz->GUIVector3("movementVector", m_MovementVector);
	app.m_viz->GUICheckbox("isCamLocked", m_locked);
}

void EditorCam::Initialize(Application& app)
{
	m_mouseMoveResult.AssignApplication(app, *this);
	m_cursorLock.AssignApplication(app, *this);

	m_mouseButtonInputs.emplace_back(GLFW_MOUSE_BUTTON_RIGHT);
	m_mouseButtonInputs[0].Subscribe(&m_cursorLock);
	m_mouseMoveDetection.Subscribe(&m_mouseMoveResult);

	std::pair<int, Mage::Maths::Vector3> axisKeys[]{
		{GLFW_KEY_W, Mage::Maths::Vector3(0.f, 0.f, 1.f)},
		{GLFW_KEY_S, Mage::Maths::Vector3(0.f, 0.f, -1.f)},
		{GLFW_KEY_D, Mage::Maths::Vector3(1.f, 0.f, 0.f)},
		{GLFW_KEY_A, Mage::Maths::Vector3(-1.f, 0.f, 0.f)},
	};
	m_keyboardButtonInputs.reserve(5);
	m_axisInputs.reserve(5);
	for (int i = 0; i < 4; i++)
	{
		m_keyboardButtonInputs.emplace_back(axisKeys[i].first);
		m_axisInputs.emplace_back(app, *this, axisKeys[i].second);
		m_keyboardButtonInputs[i].Subscribe(&m_axisInputs[i]);
	}
}
