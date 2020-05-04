#include "MiniginPCH.h"
#include "Game.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "SpriteComponent.h"
#include "KeyboardInputComponent.h"
#include "ColliderComponent.h"
#include "TileComponent.h"
#include "RigidBodyComponent.h"
#include "Level.h"

using namespace dae;

Game::Game()
{
	InitializeLevel01();
	InitializeLevel02();
	InitializeLevel03();
}

void Game::InitializeLevel01()
{

	auto& scene = SceneManager::GetInstance().CreateScene("Demo00");

	//add textures
	ResourceManager::GetInstance().AddTexture("dino", "./RESIZE/dino.png");
	ResourceManager::GetInstance().AddTexture("apple", "/RESIZE/apple.png");
	ResourceManager::GetInstance().AddTexture("clockworker", "/RESIZE/clockworker.png");
	ResourceManager::GetInstance().AddTexture("tiles", "/RESIZE/tiles.png");

	std::shared_ptr<Level> pLevel = std::make_shared<Level>("tiles", 1,32, scene);
	pLevel->LoadMap("./Data/RESIZE/level.map", 20, 15);

	//gameobject
	//!!!! think about move semantics / copyping
	std::shared_ptr<GameObject> apple = std::make_shared<GameObject>("apple", false);
	apple->AddComponent<Transform>(30, 20, 0, 0, 32, 32, 1);
	apple->AddComponent<SpriteComponent>("apple");
	scene.Add(apple);

	std::shared_ptr<GameObject> dino = std::make_shared<GameObject>("dino", false);
	dino->AddComponent<Transform>(320, 240, 0, 0, 32, 32, 1);
	auto &sprite = dino->AddComponent<SpriteComponent>("dino","idle", 2, 200, false);
	dino->AddComponent<ColliderComponent>("PLAYER");
	dino->AddComponent<KeyboardInputComponent>("left", "right", "up", "space");
	dino->AddComponent<RigidBodyComponent>();
	sprite.AddAnimation("moving", 1, 2, 200);
	sprite.AddAnimation("jumping", 3, 1, 100);
	sprite.AddAnimation("shoot", 2, 1, 10);
	scene.Add(dino);

	std::shared_ptr<GameObject> clockworker = std::make_shared<GameObject>("clockworker", false);
	clockworker->AddComponent<Transform>(100, 100, 0, 0, 32, 32, 1);
	clockworker->AddComponent<SpriteComponent>("clockworker", "standard", 2,200, false);
	clockworker->AddComponent<RigidBodyComponent>();
	clockworker->AddComponent<ColliderComponent>("ENEMY");
	scene.Add(clockworker);

	//std::shared_ptr<GameObject> dino1 = std::make_shared<GameObject>("dino1", false);
	//dino1->AddComponent<Transform>(420, 300, 0, 0, 32, 32, 1);
	//auto& sprite1 = dino1->AddComponent<SpriteComponent>("dino", "idle", 2, 200, false);
	////dino1->AddComponent<ColliderComponent>("PLAYER");
	//dino1->AddComponent<KeyboardInputComponent>("a", "d", "w", "s");
	//sprite1.AddAnimation("moving", 1, 2, 200);
	//sprite1.AddAnimation("jumping", 3, 1, 100);
	//sprite1.AddAnimation("shoot", 2, 1, 10);
	//scene.Add(dino1);


	//auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	//std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>("hello");
	//auto& transformComponent = gameObject->AddComponent<Transform>(0,0,20,20,50,50,1);
	//scene.Add(gameObject);
	//std::cout << "name of the gameObject: " << transformComponent.m_Owner->GetName() << std::endl;


	//gameObject->AddRenderComponent(std::make_shared<RenderComponent>());
	//gameObject->GetRenderComponent()->AddTexture("background.jpg");
	//int idLogo = gameObject->GetRenderComponent()->AddTexture("logo.png");
	//gameObject->GetRenderComponent()->SetPositionTexture(idLogo, 216, 180, 0);
	//auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//gameObject->AddTextComponent("Programming 4 Assignment", font);
	//gameObject->GetTextComponent()->SetPosition(80, 20);
	//scene.Add(gameObject);

	//change the content of gameObject to a new pointer that points to a different gameobject with no components yet
	//gameObject = std::make_shared<GameObject>();
	//gameObject->AddTextComponent("0", font);
	//pFrameCounter = gameObject->GetTextComponent();
	//scene.Add(gameObject);
}

void Game::InitializeLevel02()
{
}

void Game::InitializeLevel03()
{
	//auto& scene = SceneManager::GetInstance().CreateScene("Demo");
}