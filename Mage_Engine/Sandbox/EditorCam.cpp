#include "pch.h"
#include "EditorCam.h"
#include "Core/Entity.h"

void EditorCam::OnNotify(Mage::Maths::Vector3& vector)
{
	Transform* t = m_entity.getComponent<Transform>();
	if (t == nullptr)
		return;
	t->m_rotation = t->m_rotation + vector;
	//t->updateRotation();
}
