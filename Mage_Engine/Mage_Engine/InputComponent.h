#pragma once
#include "Core/Component.h"
#include "Observer.h"

class MouseMoveInput : public InputSubject
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
    const int GetButton() const;
private:
    int m_buttonID;
};

class InputComponent;

class InputResult : public InputObserver
{
public:
    InputResult();
    InputResult(InputComponent& owner);
    InputResult(Application& app, InputComponent& owner);
    void AssignApplication(Application& app);
    void AssignApplication(Application& app, InputComponent& owner);
protected:
    Application* m_app;
    InputComponent* m_owner;
};

class MouseMoveInputRotation : public InputResult
{
public:
    virtual void OnNotify(Mage::Maths::Vector3 &vector) override;
};

class EditorCamLockCursorResult : public InputResult
{
public:
    virtual void OnNotify(bool Pressed) override;
};

class AxisInput : public InputResult
{
public:
    AxisInput(Application& app, InputComponent& owner, Mage::Maths::Vector3 &axis);
    virtual void OnNotify(bool Pressed) override;
private:
    Mage::Maths::Vector3 m_axis;
    bool m_toggle;
};

//todo
//split up editor cam specific to derived class (when it all works)
class InputComponent :
    public Component, public InputObserver
{
public:
    friend class AxisInput;
    friend class EditorCamLockCursorResult;
    InputComponent(Entity& entity);
    virtual void Update(Application& app) override;
    virtual void OnGUI(Application& app) override;
    virtual void Initialize(Application& app);
    ~InputComponent();
protected:
    MouseMoveInput m_mouseMoveDetection;
    MouseMoveInputRotation m_mouseMoveResult;
    EditorCamLockCursorResult m_cursorLock;
    std::vector<ButtonInput> m_mouseButtonInputs;
    std::vector<ButtonInput> m_keyboardButtonInputs;
    std::vector<AxisInput>  m_axisInputs;
    Mage::Maths::Vector3 m_MovementVector;
    bool m_locked;
};

