#include "MiniginPCH.h"
#include "KeyboardInputComponent.h"
#include "Transform.h"
#include "SpriteComponent.h"
#include <SDL.h>
#include "SDL_keycode.h"
#include "InputManager.h"
#include "GameObject.h"
#include "ColliderComponent.h"
#include "PlayerSM.h"
#include "VersusEnemySM.h"


//programm so you can add buttons easily

dae::KeyboardInputComponent::KeyboardInputComponent(const std::string& leftKey, const std::string& rightKey, const std::string& shootKey, const std::string& jumpKey)
{
	m_LeftInput = GetSDLKeyStringCode(leftKey);
	m_RightInput = GetSDLKeyStringCode(rightKey);
	m_ShootInput = GetSDLKeyStringCode(shootKey);
	m_JumpInput = GetSDLKeyStringCode(jumpKey);
}

std::string dae::KeyboardInputComponent::GetSDLKeyStringCode(const std::string& input)
{
	//TODO: all the different keys
	if (input.compare("left") == 0) return std::to_string(SDLK_LEFT);
	if (input.compare("right") == 0) return std::to_string(SDLK_RIGHT);
	if (input.compare("up") == 0) return std::to_string(SDLK_UP);
	if (input.compare("space") == 0) return std::to_string(SDLK_SPACE);
	return std::to_string(static_cast<int>(input[0]));

}

void dae::KeyboardInputComponent::Initialize()
{
	m_pTransform = m_Owner->GetComponent<Transform>();
}

void dae::KeyboardInputComponent::Update(float deltaTime)
{
	UNREFERENCED_PARAMETER(deltaTime);

	PlayerSM* playerSM = nullptr;
	VersusEnemySM* versusEnemySM = nullptr;
	if (m_Owner->HasComponent<PlayerSM>())
	{
		playerSM = m_Owner->GetComponent<PlayerSM>();

		if (InputManager::GetInstance().m_Event.type == SDL_KEYDOWN)
		{
			std::string keyCode = std::to_string(InputManager::GetInstance().m_Event.key.keysym.sym);

			if (keyCode.compare(m_LeftInput) == 0)
			{
				m_LeftPress = true;
				playerSM->SetDirection(Direction::left);
				playerSM->Displace();
			}
			else if (keyCode.compare(m_RightInput) == 0)
			{
				m_RightPress = true;
				playerSM->SetDirection(Direction::right);

				playerSM->Displace();
			}

			if (keyCode.compare(m_ShootInput) == 0)
			{
				if (playerSM->ReadyToShoot())
				{
					playerSM->ShootBell();
				}
			}

			if (keyCode.compare(m_JumpInput) == 0)
			{
				playerSM->JumpIntoAir();
			}

		}

		if (InputManager::GetInstance().m_Event.type == SDL_KEYUP)
		{
			std::string keyCode = std::to_string(InputManager::GetInstance().m_Event.key.keysym.sym);

			if (keyCode.compare(m_LeftInput) == 0)
			{
				m_LeftPress = false;
				if (!m_LeftPress && !m_RightPress)
				{
					playerSM->StandingStill();

				}
			}
			if (keyCode.compare(m_RightInput) == 0)
			{
				m_RightPress = false;
				if (!m_LeftPress && !m_RightPress)
				{
					playerSM->StandingStill();
				}
			}

			if (keyCode.compare(m_ShootInput) == 0)
			{
				if (m_pTransform->GetVelocity().x > 0)
				{
					playerSM->SetDirection(Direction::right);
					playerSM->Displace();
				}
				else if(m_pTransform->GetVelocity().x < 0)
				{
					playerSM->SetDirection(Direction::left);
					playerSM->Displace();
				}
				else
				{
					playerSM->StandingStill();
				}
			}

		}
	}
	else if (m_Owner->HasComponent<VersusEnemySM>())
	{
		versusEnemySM = m_Owner->GetComponent<VersusEnemySM>();

		if (InputManager::GetInstance().m_Event.type == SDL_KEYDOWN)
		{
			std::string keyCode = std::to_string(InputManager::GetInstance().m_Event.key.keysym.sym);

			if (keyCode.compare(m_LeftInput) == 0)
			{
				m_LeftPress = true;
				versusEnemySM->SetDirection(Direction::left);
				versusEnemySM->Displace();
			}
			else if (keyCode.compare(m_RightInput) == 0)
			{
				m_RightPress = true;
				versusEnemySM->SetDirection(Direction::right);

				versusEnemySM->Displace();
			}

			if (keyCode.compare(m_ShootInput) == 0)
			{
				if (versusEnemySM->ReadyToShoot())
				{
					versusEnemySM->ShootProjectile();
				}
			}

			if (keyCode.compare(m_JumpInput) == 0)
			{
				versusEnemySM->JumpIntoAir();
			}

		}

		if (InputManager::GetInstance().m_Event.type == SDL_KEYUP)
		{
			std::string keyCode = std::to_string(InputManager::GetInstance().m_Event.key.keysym.sym);

			if (keyCode.compare(m_LeftInput) == 0)
			{
				m_LeftPress = false;
				if (!m_LeftPress && !m_RightPress)
				{
					versusEnemySM->StandingStill();

				}
			}
			if (keyCode.compare(m_RightInput) == 0)
			{
				m_RightPress = false;
				if (!m_LeftPress && !m_RightPress)
				{
					versusEnemySM->StandingStill();
				}
			}

			if (keyCode.compare(m_ShootInput) == 0)
			{
				if (m_pTransform->GetVelocity().x > 0)
				{
					versusEnemySM->SetDirection(Direction::right);
					versusEnemySM->Displace();
				}
				else if (m_pTransform->GetVelocity().x < 0)
				{
					versusEnemySM->SetDirection(Direction::left);
					versusEnemySM->Displace();
				}
				else
				{
					versusEnemySM->StandingStill();
				}
			}
		}


	}
}
