#pragma once

#include "Core/Component.h"
#include "Core/Transform.h"
#include "Core/InputComponent.h"
#include "Observer.h"

//todo
// - make proper input component
// - move struct to proper input component
// - bind on notify bool in inputComponent to the press and release of the keycode

class MouseMoveInputRotation : public InputResult
{
public:
	virtual void OnNotify(Mage::Maths::Vector3& vector) override;
};

class EditorCamLockCursorResult : public InputResult
{
public:
	virtual void OnNotify(bool Pressed) override;
};

class EditorCam : public InputComponent
{
public:
	EditorCam(Entity& entity);
	virtual void Update(Application& app) override;
	virtual void OnGUI(Application& app) override;
	virtual void Initialize(Application& app) override;
protected:
	MouseMoveInputRotation m_mouseMoveResult;
	EditorCamLockCursorResult m_cursorLock;
};

