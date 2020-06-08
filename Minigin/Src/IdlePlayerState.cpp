#include "MiniginPCH.h"
#include "IdlePlayerState.h"
#include "SpriteComponent.h"
#include "GameObject.h"
#include "PlayerSM.h"
#include "ColliderComponent.h"
#include "Scene.h"
#include "SceneManager.h"

void dae::IdlePlayerState::Entry()
{
	if (m_PlayerSM->m_Owner->HasComponent<SpriteComponent>())
	{
		auto sprite = m_PlayerSM->m_Owner->GetComponent<SpriteComponent>();
		sprite->Play("idle");
	}
	if (m_pTransform != nullptr)
	{
		m_pTransform->SetVelocity(0.0f, m_pTransform->GetVelocity().y);
	}
}

void dae::IdlePlayerState::JumpIntoAir()
{
	m_PlayerSM->SwitchState(m_PlayerSM->GetJumpState());
}

void dae::IdlePlayerState::ShootBell()
{
	//instantiate object
	if (m_PlayerSM->m_Owner->HasComponent<SpriteComponent>())
	{
		auto sprite = m_PlayerSM->m_Owner->GetComponent<SpriteComponent>();
		sprite->Play("shoot");
		float offset = 10.f;
		float shootingVelocity = 100.f;

		if (m_PlayerSM->GetDirection() == Direction::left)
		{
			offset = -offset;
			shootingVelocity = -shootingVelocity;
		}

		//create bell
		std::shared_ptr<GameObject> bubble{ std::make_shared<GameObject>("bubble", false) };
		bubble->AddComponent<Transform>(static_cast<int>(m_pTransform->GetPosition().x + offset), static_cast<int>(m_pTransform->GetPosition().y), static_cast<int>(shootingVelocity), 0, 32, 32, 1);
		bubble->AddComponent<ColliderComponent>("BUBBLE");
		bubble->AddComponent<SpriteComponent>("bubble");
		SceneManager::GetInstance().GetActiveScene()->Add(bubble);
	}
}

void dae::IdlePlayerState::Displace(Direction direction)
{
	m_PlayerSM->SwitchState(m_PlayerSM->GetMoveState());
	UNREFERENCED_PARAMETER(direction);
}

