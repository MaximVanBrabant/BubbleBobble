#include "MiniginPCH.h"
#include "CollisionManager.h"
#include "Collision.h"
#include "ColliderComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "RigidBodyComponent.h"
#include "SpriteComponent.h"
#include "EnemySM.h"
#include "JumpEnemyState.h"
#include <thread>


void dae::CollisionManager::CollisionUpdate()
{
	CheckGameObjectCollisions();
}

void dae::CollisionManager::ApplyCollisionEffects(CollisionInfo collisionInfo)
{

	if (collisionInfo.collisionType == CollisionType::NO_COLLISION)
	{
		return;
	}
	else if (collisionInfo.collisionType == CollisionType::PLAYER_ENEMY_COLLISION)
	{
		//collisionInfo.firstGameObject->Destroy();
	}
	else if (collisionInfo.collisionType == CollisionType::PLAYER_PROJECTILE_COLLISION)
	{
		//collisionInfo.firstGameObject->Destroy();
		collisionInfo.secondGameObject->Destroy();
	}
	else if (collisionInfo.collisionType == CollisionType::PLAYER_ITEM_COLLISION)
	{
		//add score
		if(collisionInfo.secondGameObject->GetComponent<ColliderComponent>()->GetIsGrounded())
			collisionInfo.secondGameObject->Destroy();

	}
	else if (collisionInfo.collisionType == CollisionType::ENEMY_BUBBLE_COLLISION)
	{
		auto enemySM = collisionInfo.firstGameObject->GetComponent<EnemySM>();
		enemySM->TrapInBell();
		collisionInfo.secondGameObject->Destroy();
	}
	else if (collisionInfo.collisionType == CollisionType::PLAYER_BUBBLE_ENEMY_COLLISION)
	{

		//make food
		auto enemySM = collisionInfo.secondGameObject->GetComponent<EnemySM>();
		auto enemyTransform = collisionInfo.secondGameObject->GetComponent<Transform>();

		std::shared_ptr<GameObject> foodItem = std::make_shared<GameObject>("food", false);
		foodItem->AddComponent<Transform>(static_cast<int>(enemyTransform->GetPosition().x), static_cast<int>(enemyTransform->GetPosition().y), 0, 0, 32, 32, 1);
		foodItem->AddComponent<ColliderComponent>("ITEM");
		foodItem->AddComponent<RigidBodyComponent>();
		if (enemySM->GetEnemyType() == EnemyType::Charge)
		{
			foodItem->AddComponent<SpriteComponent>("melon");
		}
		else if (enemySM->GetEnemyType() == EnemyType::Throw)
		{
			foodItem->AddComponent<SpriteComponent>("fries");
		}

		collisionInfo.secondGameObject->Destroy();


		SceneManager::GetInstance().GetActiveScene()->Add(foodItem);


	}
	else if (collisionInfo.collisionType == CollisionType::RIGID_TILE_COLLISION)
	{
		//get colliders
		//first check if i have those components on the gameobjects
		ColliderComponent* playerCollider{ collisionInfo.firstGameObject->GetComponent<ColliderComponent>() };
		ColliderComponent* tileCollider{ collisionInfo.secondGameObject->GetComponent<ColliderComponent>() };
		Transform* playerTransform{ collisionInfo.firstGameObject->GetComponent<Transform>() };
		RigidBodyComponent* playerRigid{ collisionInfo.firstGameObject->GetComponent<RigidBodyComponent>() };

		if (abs(playerCollider->GetCenterX() - tileCollider->GetCenterX()) == abs(playerCollider->GetCenterY() - tileCollider->GetCenterY()))
		{
			return;
		}

		if (abs(playerCollider->GetCenterX() - tileCollider->GetCenterX()) < abs(playerCollider->GetCenterY() - tileCollider->GetCenterY()))
		{
			if (playerCollider->GetCenterY() < tileCollider->GetCenterY() && !playerCollider->GetIsGrounded() && playerTransform->GetVelocity().y > 0.0f)
			{
				playerTransform->SetPosition(playerTransform->GetPosition().x, static_cast<float>(tileCollider->GetCollider().y - tileCollider->GetCollider().h));
				playerTransform->SetVelocity(playerTransform->GetVelocity().x, 0);
				playerRigid->EnableGravity(false);
				playerCollider->SetIsGrounded(true);
			}
		}
		else
		{
			//the GetisGrounded makes sure i cant walk through walls
			if (playerTransform->GetVelocity().y > 0.0f || playerCollider->GetIsGrounded())
			{
				if (playerCollider->GetCenterX() < tileCollider->GetCenterX())
				{
					playerTransform->SetPosition(static_cast<float>(tileCollider->GetCollider().x - playerCollider->GetCollider().w), playerTransform->GetPosition().y);
				}
				if (playerCollider->GetCenterX() > tileCollider->GetCenterX())
				{
					playerTransform->SetPosition(static_cast<float>(tileCollider->GetCollider().x + tileCollider->GetCollider().w), playerTransform->GetPosition().y);
				}
			}

		}
	}
	else if (collisionInfo.collisionType == CollisionType::RIGID_HORIZONTAL_SOLID_TILE_COLLISION)
	{
		//get colliders
		//first check if i have those components on the gameobjects
		ColliderComponent* playerCollider{ collisionInfo.firstGameObject->GetComponent<ColliderComponent>() };
		ColliderComponent* tileCollider{ collisionInfo.secondGameObject->GetComponent<ColliderComponent>() };
		Transform* playerTransform{ collisionInfo.firstGameObject->GetComponent<Transform>() };
		RigidBodyComponent* playerRigid{ collisionInfo.firstGameObject->GetComponent<RigidBodyComponent>() };


		//getisgrouneded and > 0.0f makes sure this doesnt check when youre still on the ground + when you jump otherwise it would instantly stick it to the ground again
		if (playerCollider->GetCenterY() < tileCollider->GetCenterY() && !playerCollider->GetIsGrounded() && playerTransform->GetVelocity().y > 0.0f)
		{
			playerTransform->SetPosition(playerTransform->GetPosition().x, static_cast<float>(tileCollider->GetCollider().y - tileCollider->GetCollider().h));
			playerTransform->SetVelocity(playerTransform->GetVelocity().x, 0);
			playerRigid->EnableGravity(false);
			playerCollider->SetIsGrounded(true);
		}
		if (playerCollider->GetCenterY() > tileCollider->GetCenterY() && !playerCollider->GetIsGrounded() && playerTransform->GetVelocity().y < 0.0f)
		{
			playerTransform->SetPosition(playerTransform->GetPosition().x, static_cast<float>(tileCollider->GetCollider().y + tileCollider->GetCollider().h));
			playerTransform->SetVelocity(playerTransform->GetVelocity().x, 0);

		}

	}
	else if (collisionInfo.collisionType == CollisionType::RIGID_VERTICAL_SOLID_TILE_COLLISION)
	{
		//get colliders
		//first check if i have those components on the gameobjects
		ColliderComponent* playerCollider{ collisionInfo.firstGameObject->GetComponent<ColliderComponent>() };
		ColliderComponent* tileCollider{ collisionInfo.secondGameObject->GetComponent<ColliderComponent>() };
		Transform* playerTransform{ collisionInfo.firstGameObject->GetComponent<Transform>() };

		//the GetisGrounded makes sure i cant walk through walls

		if (playerCollider->GetCenterX() < tileCollider->GetCenterX())
		{
			playerTransform->SetPosition(static_cast<float>(tileCollider->GetCollider().x - playerCollider->GetCollider().w), playerTransform->GetPosition().y);
		}
		if (playerCollider->GetCenterX() > tileCollider->GetCenterX())
		{
			playerTransform->SetPosition(static_cast<float>(tileCollider->GetCollider().x + tileCollider->GetCollider().w), playerTransform->GetPosition().y);

		}


	}
	else if (collisionInfo.collisionType == CollisionType::ENEMY_ENEMY_COLLISION)
	{
		ColliderComponent* playerCollider{ collisionInfo.firstGameObject->GetComponent<ColliderComponent>() };
		ColliderComponent* tileCollider{ collisionInfo.secondGameObject->GetComponent<ColliderComponent>() };
		Transform* playerTransform{ collisionInfo.firstGameObject->GetComponent<Transform>() };
		//if (abs(playerCollider->GetCenterX() - tileCollider->GetCenterX()) == abs(playerCollider->GetCenterY() - tileCollider->GetCenterY()))
		//{
		//	return;
		//}

		if (abs(playerCollider->GetCenterX() - tileCollider->GetCenterX()) < abs(playerCollider->GetCenterY() - tileCollider->GetCenterY()))
		{
			if (playerCollider->GetCenterY() < tileCollider->GetCenterY() && !playerCollider->GetIsGrounded() && playerTransform->GetVelocity().y > 0.0f)
			{
				playerTransform->SetPosition(playerTransform->GetPosition().x, static_cast<float>(tileCollider->GetCollider().y - tileCollider->GetCollider().h));
				playerTransform->SetVelocity(playerTransform->GetVelocity().x, 0);
			}
		}
		else
		{
			if (playerCollider->GetCenterX() < tileCollider->GetCenterX())
			{
				playerTransform->SetPosition(static_cast<float>(tileCollider->GetCollider().x - playerCollider->GetCollider().w), playerTransform->GetPosition().y);
			}
			if (playerCollider->GetCenterX() > tileCollider->GetCenterX())
			{
				playerTransform->SetPosition(static_cast<float>(tileCollider->GetCollider().x + tileCollider->GetCollider().w), playerTransform->GetPosition().y);

			}
		}
	}
	else if (collisionInfo.collisionType == CollisionType::RIGID_NONE_COLLISION)
	{
		//you are in the air
		if (collisionInfo.firstGameObject->HasComponent<RigidBodyComponent>() && collisionInfo.firstGameObject->HasComponent<ColliderComponent>() && collisionInfo.firstGameObject->HasComponent<Transform>())
		{
			ColliderComponent* playerCollider{ collisionInfo.firstGameObject->GetComponent<ColliderComponent>() };
			RigidBodyComponent* playerRigid{ collisionInfo.firstGameObject->GetComponent<RigidBodyComponent>() };
			Transform* playerTransform{ collisionInfo.firstGameObject->GetComponent<Transform>() };

			if (playerCollider->GetIsGrounded())
			{
				playerCollider->SetIsGrounded(false);
				//instantly sets my velocity instead of gradually decreasing over time with the rigidbody gravity
				if (playerTransform->GetVelocity().y == 0)
					playerTransform->SetVelocity(playerTransform->GetVelocity().x, 200.f);
				playerRigid->EnableGravity(true);
			}
		}
	}
	else if (collisionInfo.collisionType == CollisionType::AI_FLOOR_TILE_COLLISION)
	{
		//std::cout << "ai collider collides with tile" << std::endl;
	}
	else if (collisionInfo.collisionType == CollisionType::AI_FLOOR_NONE_COLLISION)
	{
		if (collisionInfo.firstGameObject->HasComponent<EnemySM>())
		{
			auto enemySM = collisionInfo.firstGameObject->GetComponent<EnemySM>();
			auto enemyCollider = collisionInfo.firstGameObject->GetComponent<ColliderComponent>();
			if (enemyCollider->GetIsGrounded())
			{
				//here choose which jumpheight to use

				enemySM->SetJumpHeight(enemySM->GetHorizontalJumpHeight());
				enemySM->JumpIntoAir();
			}
		}
	}
	else if (collisionInfo.collisionType == CollisionType::AI_PLATFORM_TILE_COLLISION)
	{
		if (collisionInfo.firstGameObject->HasComponent<EnemySM>())
		{
			auto enemySM = collisionInfo.firstGameObject->GetComponent<EnemySM>();
			auto enemyCollider = collisionInfo.firstGameObject->GetComponent<ColliderComponent>();
			if (enemyCollider->GetIsGrounded())
			{
				//here choose which jumpheight to use
				enemySM->SetJumpHeight(enemySM->GetVerticalJumpHeight());
				enemySM->JumpIntoAir();
			}
		}
	}
	else if (collisionInfo.collisionType == CollisionType::AI_WALL_VERTICAL_SOLID_TILE_COLLISION)
	{
		if (collisionInfo.firstGameObject->HasComponent<EnemySM>())
		{
			auto enemySM = collisionInfo.firstGameObject->GetComponent<EnemySM>();
			auto enemyCollider = collisionInfo.firstGameObject->GetComponent<ColliderComponent>();
			if (enemyCollider->GetIsGrounded())
			{
				enemySM->SetInverseMovement(true);
			}
		}
	}
	else if (collisionInfo.collisionType == CollisionType::AI_WALL_ENEMY_COLLISION)
	{
		auto enemySM = collisionInfo.firstGameObject->GetComponent<EnemySM>();
		enemySM->SetInverseMovement(true);
	}

}

void dae::CollisionManager::CheckCollisionOnAIColliders(const std::vector<int>& vIndicesOfEnemies)
{
	//make a vector store all the enabled collider from the ai in it then iterate over them and give them specific tags so you know what to do
	//enable disable depending on the displacement vector
	//no need for the getter of the displacement vector

	auto& gameObjects = SceneManager::GetInstance().GetActiveScene()->GetGameObjects();

	for (int index : vIndicesOfEnemies)
	{
		auto& enemy = gameObjects[index];

		if (enemy->HasComponent<EnemySM>())
		{
			std::vector<ColliderComponent*> vAIColliders{};

			auto enemySM = enemy->GetComponent<EnemySM>();
			auto leftFloorCollider = enemySM->GetLeftFloorCollider();
			auto rightFloorCollider = enemySM->GetRightFloorCollider();
			auto leftPlatformCollider = enemySM->GetLeftPlatformCollider();
			auto rightPlatformCollider = enemySM->GetRightPlatformCollider();
			auto leftWallCollider = enemySM->GetLeftWallCollider();
			auto rightWallCollider = enemySM->GetRightWallCollider();


			if (leftFloorCollider->GetEnabled())
			{
				vAIColliders.push_back(leftFloorCollider);
			}
			if (rightFloorCollider->GetEnabled())
			{
				vAIColliders.push_back(rightFloorCollider);
			}
			if (leftPlatformCollider->GetEnabled())
			{
				vAIColliders.push_back(leftPlatformCollider);
			}
			if (rightPlatformCollider->GetEnabled())
				vAIColliders.push_back(rightPlatformCollider);
			if(rightWallCollider->GetEnabled())
				vAIColliders.push_back(rightWallCollider);
			if(leftWallCollider->GetEnabled())
				vAIColliders.push_back(leftWallCollider);

			for (auto AICollider : vAIColliders)
			{
				AICollider->SetIsColliding(false);
				const std::string& AITag = AICollider->GetTag();

				for (int i{ 0 }; i < gameObjects.size(); ++i)
				{
					auto& otherGameObject = gameObjects[i];
					if (otherGameObject == enemy)
						continue;

					if (otherGameObject->HasComponent<ColliderComponent>() && (otherGameObject->GetName().compare("tile") == 0 || otherGameObject->GetComponent<ColliderComponent>()->GetTag().compare("ENEMY") == 0))
					{
						ColliderComponent* otherCollider = otherGameObject->GetComponent<ColliderComponent>();
						const std::string& secondTag = otherCollider->GetTag();

						if (Collision::CheckRectangleCollision(AICollider->GetCollider(), otherCollider->GetCollider()))
						{
							AICollider->SetIsColliding(true);
							if (Collision::CheckCollisionTypeWithTags(AITag, secondTag, "AI_FLOOR", "TILE"))
							{
								ApplyCollisionEffects(CollisionInfo{ CollisionType::AI_FLOOR_TILE_COLLISION, enemy, otherGameObject });
							}
							else if (Collision::CheckCollisionTypeWithTags(AITag, secondTag, "AI_PLATFORM", "TILE"))
							{
								ApplyCollisionEffects(CollisionInfo{ CollisionType::AI_PLATFORM_TILE_COLLISION, enemy, otherGameObject });
							}
							else if (Collision::CheckCollisionTypeWithTags(AITag, secondTag, "AI_WALL", "VERTICAL_SOLID_TILE"))
							{
								ApplyCollisionEffects(CollisionInfo{ CollisionType::AI_WALL_VERTICAL_SOLID_TILE_COLLISION, enemy, otherGameObject });
							}
							else if (Collision::CheckCollisionTypeWithTags(AITag, secondTag, "AI_WALL", "ENEMY"))
							{
								ApplyCollisionEffects(CollisionInfo{ CollisionType::AI_WALL_ENEMY_COLLISION, enemy, otherGameObject });
							}
						}
					}
				}

				if (!AICollider->GetIsColliding())
				{
					if(AITag.compare("AI_FLOOR") == 0)
						ApplyCollisionEffects(CollisionInfo{ CollisionType::AI_FLOOR_NONE_COLLISION, enemy, nullptr });
				}
			}

		}
	}

}

void dae::CollisionManager::CheckGameObjectCollisions()
{
	auto& gameObjects = SceneManager::GetInstance().GetActiveScene()->GetGameObjects();
	std::vector<int> vEnemyIndices{};

	for (int i{ 0 }; i < gameObjects.size(); ++i)
	{
		if (gameObjects[i]->HasComponent<ColliderComponent>())
		{
			gameObjects[i]->GetComponent<ColliderComponent>()->SetIsColliding(false);
		}
		if (gameObjects[i]->HasComponent<EnemySM>())
		{
			vEnemyIndices.push_back(i);
		}
	}

	//HERE YOU CAN USE THREAD
	CheckCollisionOnAIColliders(vEnemyIndices);

	

	for (int i{ 0 }; i < gameObjects.size(); ++i)
	{
		auto& firstGameObject = gameObjects[i];
		if (firstGameObject->HasComponent<ColliderComponent>())
		{
			ColliderComponent* firstCollider = firstGameObject->GetComponent<ColliderComponent>();

			if (!firstCollider->GetEnabled())
				continue;

			for (int j{ i + 1 }; j < gameObjects.size(); ++j)
			{
				auto& secondGameObject = gameObjects[j];
				if (firstGameObject->GetName().compare(secondGameObject->GetName()) != 0 && secondGameObject->HasComponent<ColliderComponent>())
				{
					ColliderComponent* secondCollider = secondGameObject->GetComponent<ColliderComponent>();

					if (!secondCollider->GetEnabled())
						continue;

					const std::string& firstTag = firstCollider->GetTag();
					const std::string& secondTag = secondCollider->GetTag();

					if (Collision::CheckRectangleCollision(firstCollider->GetCollider(), secondCollider->GetCollider()))
					{
						firstCollider->SetIsColliding(true);
						secondCollider->SetIsColliding(true);

						if (Collision::CheckCollisionTypeWithTags(firstTag, secondTag, "ENEMY", "PLAYER"))
						{
							if (firstTag.compare("PLAYER") == 0)
								ApplyCollisionEffects(CollisionInfo(CollisionType::PLAYER_ENEMY_COLLISION, firstGameObject, secondGameObject));
							else
								ApplyCollisionEffects(CollisionInfo(CollisionType::PLAYER_ENEMY_COLLISION, secondGameObject, firstGameObject));
							continue;

						}
						if (Collision::CheckCollisionTypeWithTags(firstTag, secondTag, "PLAYER", "PROJECTILE"))
						{
							if (firstTag.compare("PLAYER") == 0)
								ApplyCollisionEffects(CollisionInfo(CollisionType::PLAYER_PROJECTILE_COLLISION, firstGameObject, secondGameObject));
							else
								ApplyCollisionEffects(CollisionInfo(CollisionType::PLAYER_PROJECTILE_COLLISION, secondGameObject, firstGameObject));
							continue;

						}
						if (Collision::CheckCollisionTypeWithTags(firstTag, secondTag, "ENEMY", "BUBBLE"))
						{
							if (firstTag.compare("ENEMY") == 0)
								ApplyCollisionEffects(CollisionInfo(CollisionType::ENEMY_BUBBLE_COLLISION, firstGameObject, secondGameObject));
							else
								ApplyCollisionEffects(CollisionInfo(CollisionType::ENEMY_BUBBLE_COLLISION, secondGameObject, firstGameObject));
							continue;

						}
						if (Collision::CheckCollisionTypeWithTags(firstTag, secondTag, "PLAYER", "ITEM"))
						{
							if (firstTag.compare("PLAYER") == 0)
								ApplyCollisionEffects(CollisionInfo(CollisionType::PLAYER_ITEM_COLLISION, firstGameObject, secondGameObject));
							else
								ApplyCollisionEffects(CollisionInfo(CollisionType::PLAYER_ITEM_COLLISION, secondGameObject, firstGameObject));
							continue;

						}
						if (Collision::CheckCollisionTypeWithTags(firstTag, secondTag, "PLAYER", "BUBBLE_ENEMY"))
						{
							if (firstTag.compare("PLAYER") == 0)
								ApplyCollisionEffects(CollisionInfo(CollisionType::PLAYER_BUBBLE_ENEMY_COLLISION, firstGameObject, secondGameObject));
							else
								ApplyCollisionEffects(CollisionInfo(CollisionType::PLAYER_BUBBLE_ENEMY_COLLISION, secondGameObject, firstGameObject));
							continue;

						}
						if (Collision::CheckCollisionTypeWithTags(firstTag, secondTag, "ENEMY", "ENEMY"))
						{
							ApplyCollisionEffects(CollisionInfo(CollisionType::ENEMY_ENEMY_COLLISION, firstGameObject, secondGameObject));
							firstCollider->SetIsColliding(false);
							secondCollider->SetIsColliding(false);
							continue;
						}
						if (firstTag.compare("TILE") == 0 || secondTag.compare("TILE") == 0)
						{
							if (firstTag.compare("TILE") == 0)
							{
								if (secondGameObject->HasComponent<RigidBodyComponent>())
								{
									ApplyCollisionEffects(CollisionInfo{ CollisionType::RIGID_TILE_COLLISION, secondGameObject, firstGameObject });
									continue;
								}
							}
							else
							{
								if (firstGameObject->HasComponent<RigidBodyComponent>())
								{
									ApplyCollisionEffects(CollisionInfo{ CollisionType::RIGID_TILE_COLLISION, firstGameObject, secondGameObject });
									continue;
								}
							}
						}
						if (firstTag.compare("HORIZONTAL_SOLID_TILE") == 0 || secondTag.compare("HORIZONTAL_SOLID_TILE") == 0)
						{
							if (firstTag.compare("HORIZONTAL_SOLID_TILE") == 0)
							{
								if (secondGameObject->HasComponent<RigidBodyComponent>())
								{
									ApplyCollisionEffects(CollisionInfo{ CollisionType::RIGID_HORIZONTAL_SOLID_TILE_COLLISION, secondGameObject, firstGameObject });
									continue;
								}
							}
							else
							{
								if (firstGameObject->HasComponent<RigidBodyComponent>())
								{
									ApplyCollisionEffects(CollisionInfo{ CollisionType::RIGID_HORIZONTAL_SOLID_TILE_COLLISION, firstGameObject, secondGameObject });
									continue;
								}
							}
						}
						if (firstTag.compare("VERTICAL_SOLID_TILE") == 0 || secondTag.compare("VERTICAL_SOLID_TILE") == 0)
						{
							if (firstTag.compare("VERTICAL_SOLID_TILE") == 0)
							{
								if (secondGameObject->HasComponent<RigidBodyComponent>())
								{
									ApplyCollisionEffects(CollisionInfo{ CollisionType::RIGID_VERTICAL_SOLID_TILE_COLLISION, secondGameObject, firstGameObject });
									continue;
								}
							}
							else
							{
								if (firstGameObject->HasComponent<RigidBodyComponent>())
								{
									ApplyCollisionEffects(CollisionInfo{ CollisionType::RIGID_VERTICAL_SOLID_TILE_COLLISION, firstGameObject, secondGameObject });
									continue;
								}
							}
						}
					}
				}

			}

			if (!firstCollider->GetIsColliding())
			{
				if (firstGameObject->HasComponent<RigidBodyComponent>())
				{
					//he didnt collide with anything so he is not grounded
					ApplyCollisionEffects(CollisionInfo{ CollisionType::RIGID_NONE_COLLISION, firstGameObject, nullptr });
				}
			}
		}
	}
}