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

const int ButtonInput::GetButton() const
{
	return m_buttonID;
}

void InputComponent::Update(Application& app)
{
	double x, y;
	GLFWwindow* window = app.m_viz->getWindow();
	glfwGetCursorPos(window, &x, &y);
	m_mouseMoveDetection.Notify(Mage::Maths::Vector3(x, y, 0));

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
			default:
			}
		}
	};

	checkButtons(m_mouseButtonInputs, window, glfwGetMouseButton);
	checkButtons(m_keyboardButtonInputs, window, glfwGetKey);

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

