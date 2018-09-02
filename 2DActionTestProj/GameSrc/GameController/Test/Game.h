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
	ECS::Entity* hitBox[6];
	bool isReset = true;
	void EventUpDate(); 
	void ResourceLoad();
	class SceneManager
	{
	public:
		SceneManager() = default;
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
		Scene scene = Play;
		Scene prevScene = Title;
	};
	
public:
	enum class GameGroup : ECS::Group
	{
		//�ԍ����傫���قǎ�O�ɕ`�悳���(1u����w�肷��)
		Back = 1u,
		Map,
		Enemy,
		Player,
		PlayerAttackCollision,
		Max,
	};
	Game();
	//Entity�̏����������𖾎��I�ɌĂяo���܂�
	void ResetGame();
	//Entity�����Event�̍X�V�������s���܂�
	void Update();
	//Entity�̕`����s���܂�
	void Draw();

	inline static SceneManager& GetScene()
	{
		static std::unique_ptr<SceneManager> inst =
			std::make_unique<SceneManager>();
		return *inst;
	}
};
