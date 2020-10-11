#include "MiniginPCH.h"
#include "RigidBodyComponent.h"
#include "Transform.h"
#include "GameObject.h"

void dae::RigidBodyComponent::Initialize()
{
	m_pTransform = m_Owner->GetComponent<Transform>();
}

void dae::RigidBodyComponent::Update(float deltaTime)
{
	if (m_GravityOn)
	{
		m_pTransform->ApplyForceToVelocity(0, m_Gravity * deltaTime);
	}
}