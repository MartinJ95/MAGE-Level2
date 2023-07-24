#pragma once
#include "Core/Component.h"
#include "Observer.h"

class MouseMoveInput : InputSubject
{
public:
    MouseMoveInput();
    virtual void Notify(const Mage::Maths::Vector3& vector) override;
private:
    Mage::Maths::Vector3 m_lastMousePos;
};

class MouseButtonInput : InputSubject
{
public:
    MouseButtonInput(int mouseButton);
private:
    int m_mouseButton;
};

class InputComponent :
    public Component, public InputObserver
{
public:
    virtual void Update(Application& app) override;
private:
    MouseMoveInput m_mouseMoveDetection;
};

