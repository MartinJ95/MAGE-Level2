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

class ButtonInput : public InputSubject
{
public:
    ButtonInput(int buttonID);
private:
    int m_buttonID;
};

class InputComponent :
    public Component, public InputObserver
{
public:
    virtual void Update(Application& app) override;
private:
    MouseMoveInput m_mouseMoveDetection;
};

