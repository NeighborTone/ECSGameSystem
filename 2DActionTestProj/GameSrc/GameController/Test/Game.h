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
		//番号が大きいほど手前に描画される(1uから指定する)
		Back = 1u,
		Map,
		Enemy,
		Player,
		PlayerAttackCollision,
		Max,
	};
	Game();
	//Entityの初期化処理を明示的に呼び出します
	void ResetGame();
	//EntityおよびEventの更新処理を行います
	void Update();
	//Entityの描画を行います
	void Draw();

	inline static SceneManager& GetScene()
	{
		static std::unique_ptr<SceneManager> inst =
			std::make_unique<SceneManager>();
		return *inst;
	}
};
