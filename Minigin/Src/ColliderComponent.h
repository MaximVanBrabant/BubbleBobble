#pragma once
#include "Component.h"
#include <SDL_rect.h>
namespace dae
{
	class Transform;
	class ColliderComponent : public Component
	{
	public:
		ColliderComponent() = default;
		ColliderComponent(const std::string& tag);
		void Initialize() override;
		void Update(float deltaTime) override;
		void Render() const override;
		const SDL_Rect& GetCollider() const { return m_Collider; }
		const std::string& GetTag() const { return m_Tag; }
		const float GetCenterX() const{ return m_CenterX; }
		const float GetCenterY() const{ return m_CenterY; }
		void SetIsGrounded(bool isGrounded) { m_IsGrounded = isGrounded; }
		bool GetIsGrounded() const { return m_IsGrounded; }
		void SetIsColliding(bool isColliding) { m_IsColliding = isColliding; }
		bool GetIsColliding() const { return m_IsColliding; }

	private:
		//SDL_Rect m_CollisionBox;
		Transform* m_pTransform = nullptr;
		std::string m_Tag;
		SDL_Rect m_Collider;
		float m_CenterX;
		float m_CenterY;
		bool m_IsGrounded = false;
		bool m_IsColliding = false;
	};
}
