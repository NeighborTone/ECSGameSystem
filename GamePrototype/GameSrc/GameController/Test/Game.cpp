#include "Game.h"
#include "../GameSrc/ArcheType/ArcheType.hpp"
#include "../GameSrc/Utility/Utility.hpp"
#include "../GameSrc/Components/Transform.hpp"
#include "../GameSrc/Events/Event1.hpp"
#include "../GameSrc/Events/Event2.hpp"
#include "../GameSrc/Events/Event3.hpp"
#include "../GameSrc/ResourceManager/ResourceManager.hpp"
#include "../../Collision/Collision.hpp"
#include <atltime.h>

Game::Game()
{
	{
		//処理負荷を計測する
		ProcessingTime<std::chrono::milliseconds> time;
		pManager = &ECS::EcsSystem::GetManager();
		//リソースクラスのテスト
		ResourceManager::GetGraph().LoadDiv("Resource/Act_Chara2.png", "PlayerGraphic",48,6,8,64,64);
		ResourceManager::GetSound().Load("Resource/Grass.wav","bgm");
		PlaySoundMem(ResourceManager::GetSound().GetHandle("bgm"), DX_PLAYTYPE_LOOP);
		//ArcheType(原型)から作る↓
		player = ECS::PlayerArcheType()(100.f, 100.f, "PlayerGraphic");
		hitBox = ECS::BlueBoxArcheType()(200.f,100.f);
	}
}

//まだ呼んでない
void Game::Initialize()
{
	pManager->Initialize();
}

void Game::EventUpDate()
{
	const auto& b = pManager->GetEntitiesByGroup(GameGroup::Box);
	const auto& c = pManager->GetEntitiesByGroup(GameGroup::PlayerAttackCollision);
	Event::CreateAttackCollision()();
	for (const auto& col : c)
	{
		for (const auto& box : b)
		{
			if (Collision::BoxAndBox(*col, *box))
			{
				//まずコリジョンを消す
				col->DeleteComponent<ECS::HitBase>();
				box->DeleteComponent<ECS::HitBase>();
				//何らかの死亡演出がある場合こんな感じにする
				box->AddComponent<ECS::KillEntity>(10);  //10フレーム後に死ぬ
				DOUT << "hit!!" << std::endl;
			}
		}
	}
}

void Game::Update()
{
	Input::Update_Key();
	pManager->Refresh();
	EventUpDate();
	pManager->UpDate();
}

void Game::Draw()
{
	//pManager->Draw2D();
	const auto& b = pManager->GetEntitiesByGroup(GameGroup::Box);
	const auto& p = pManager->GetEntitiesByGroup(GameGroup::Player);
	const auto& c = pManager->GetEntitiesByGroup(GameGroup::PlayerAttackCollision);
	for (const auto& it : b) { it->Draw2D(); }
	for (const auto& it : p) { it->Draw2D(); }
	for (const auto& it : c) { it->Draw2D(); }
}