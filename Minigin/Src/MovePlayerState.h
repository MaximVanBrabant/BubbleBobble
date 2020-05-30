#pragma once
#include "PlayerState.h"
namespace dae
{
	class MovePlayerState : public PlayerState
	{
	public:
		MovePlayerState(PlayerSM* playerSM) :PlayerState(playerSM) {}
		void Entry() override;
		void Update(float deltaTime) override { UNREFERENCED_PARAMETER(deltaTime); }
		void Exit() override {}
		void JumpIntoAir() override;
		void StandingStill() override;
		void ShootBell(Direction direction) override;
		void Displace(Direction direction) override;
	private:
		const float m_MovingSpeed = 200.f;
	};
}

