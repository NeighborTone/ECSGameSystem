#pragma once
#include "../GameSrc/ECS/ECS.hpp"
#include "../GameSrc/Components/Transform.hpp"
#include "../GameSrc/System/System.hpp"
class Game
{
private:
	ECS::EntityManager* pManager;
	ECS::Entity* player;
	ECS::Entity* hitBox;
	void EventUpDate();
public:
	enum GameGroup : ECS::Group
	{
		Player,
		Box,
		PlayerAttackCollision,
	};
	Game();
	//Entityの初期化処理を明示的に呼び出します
	void Initialize();
	//EntityおよびEventの更新処理を行います
	void Update();
	//Entityの描画を行います
	void Draw();
};



