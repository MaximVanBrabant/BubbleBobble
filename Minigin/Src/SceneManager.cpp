#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update(float deltaTime)
{
	if (m_Scenes.size() != 0)
		m_Scenes[m_ActiveSceneId]->Update(deltaTime);
}

void dae::SceneManager::Render()
{
	if(m_Scenes.size() != 0)
		m_Scenes[m_ActiveSceneId]->Render();
}

void dae::SceneManager::SetActiveScene(int id)
{
	m_ActiveSceneId = id;

	//when it switches over to a new scene we have to add the UI back which are the Observers for the score
	SceneManager::GetInstance().GetEntityCounter()->InitializeObserversInNewScene();
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes.push_back(scene);
	return *scene;
}
