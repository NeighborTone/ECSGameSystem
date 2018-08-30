#pragma once
#include "../GameSrc/ECS/ECS.hpp"
#include "../GameSrc/Components/Components.hpp"
#include "../GameSrc/System/System.hpp"
#define ENTITY_GROUP (ECS::Group)Game::GameGroup	

class Game
{
public:
	enum Scene
	{
		Reset,
		Title,
		Play,
		Pause,
		End
	};
private:
	ECS::EntityManager* pManager;
	ECS::Entity* player;
	ECS::Entity* hitBox[5];
	ECS::Entity* ground;
	bool isReset = true;
	void EventUpDate(); 

	class SceneManager
	{
	public:
		const Scene& PrevScene() const
		{
			return prevScene;
		}
		const Scene& Current() const
		{
			return scene;
		}
		void Change(const Scene& next)
		{
			prevScene = scene;
			scene = next;
		}
	private:
		Scene scene = Title;
		Scene prevScene = Title;
	};
public:
	enum class GameGroup : ECS::Group
	{
		Player,
		Enemy,
		Map,
		PlayerAttackCollision,
	};
	Game();
	//Entity�̏����������𖾎��I�ɌĂяo���܂�
	void ResetGame();
	//Entity�����Event�̍X�V�������s���܂�
	void Update();
	//Entity�̕`����s���܂�
	void Draw();

	static SceneManager& GetScene()
	{
		static std::unique_ptr<SceneManager> inst =
			std::make_unique<SceneManager>();
		return *inst;
	}
};