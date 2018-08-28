#pragma once
#include "../GameSrc/ECS/ECS.hpp"
class Game
{
private:
	ECS::EntityManager* pManager;
	ECS::Entity* white;
	ECS::Entity* red;
public:
	Game();
	//Entityの初期化処理を明示的に呼び出します
	void Initialize();
	//EntityおよびEventの更新処理を行います
	void Update();
	//Entityの描画を行います
	void Draw();
};