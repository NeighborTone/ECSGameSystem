#include "Game.h"
#include "../GameSrc/ArcheType/ArcheType.hpp"
#include "../GameSrc/Utility/Utility.hpp"
#include "../GameSrc/Components/Components.hpp"
#include "../GameSrc/Events/Event1.hpp"
#include "../GameSrc/Events/Event2.hpp"
#include "../GameSrc/Events/Event3.hpp"
#include "../GameSrc/Events/Event4.hpp"
#include "../GameSrc/ResourceManager/ResourceManager.hpp"
#include "../../Collision/Collision.hpp"
#include "../GameSrc/Camera/Camera.hpp"
#include <atltime.h>

Game::Game()
{
	{
		//処理負荷を計測する
		ProcessingTime<std::chrono::milliseconds> time;
		pManager = &ECS::EcsSystem::GetManager();
		//リソースクラスのテスト
		ResourceManager::GetGraph().LoadDiv("Resource/Act_Chara2.png", "PlayerGraphic", 48, 6, 8, 64, 64);
		ResourceManager::GetGraph().Load("Resource/map2.png", "back");
		ResourceManager::GetGraph().Load("Resource/gothic-castle-tileset.png", "map1");
		ResourceManager::GetSound().Load("Resource/Grass.wav", "bgm");
		ResourceManager::GetSound().Load("Resource/タマネギ.ogg", "hitSE");
		//PlaySoundMem(ResourceManager::GetSound().GetHandle("bgm"), DX_PLAYTYPE_LOOP);
		//ArcheType(原型)から作る
		player = ECS::PlayerArcheType()(100.f, 500.f, "PlayerGraphic");
		//ground[0] = ECS::GreenBoxArcheType()(0.f, 600.f, 1280.f, 100.f);
		ground[1] = ECS::GreenBoxArcheType()(300.f, 480.f, 80.f, 64.f);
		ground[2] = ECS::GreenBoxArcheType()(400.f, 380.f, 80.f, 10.f);
		ground[3] = ECS::GreenBoxArcheType()(500.f, 280.f, 80.f, 10.f);
		ground[4] = ECS::GreenBoxArcheType()(600.f, 180.f, 80.f, 10.f);
		for (auto i(0u); i < std::size(hitBox); ++i)
		{
			hitBox[i] = ECS::BlueBoxArcheType()((float)i * 100.f + 200.f, 532.f);
		}
		back = ECS::BackArcheType()();
		for (int i = 0; i < 400; ++i)
		{
			ECS::MapArcheType()("map1",(float)i * 32.f, 632.f, 0, 0, 32, 32);
			
		}
		for (int i = 0; i < 200; ++i)
		{
			ECS::MapArcheType()("map1", (float)i * 64.f, 664.f, 96, 0, 64, 64);
		}
	
	}
}

void Game::ResetGame()
{
	{
		//処理負荷を計測する
		ProcessingTime<std::chrono::milliseconds> time;
		pManager->Initialize();
		const auto& b = pManager->GetEntitiesByGroup(ENTITY_GROUP::Enemy);
		const auto& p = pManager->GetEntitiesByGroup(ENTITY_GROUP::Player);
		const auto& c = pManager->GetEntitiesByGroup(ENTITY_GROUP::PlayerAttackCollision);
		for (auto& it : p) { it->Destroy(); }
		for (auto& it : b) { it->Destroy(); }
		for (auto& it : c) { it->Destroy(); }

		player = ECS::PlayerArcheType()(100.f, 500.f, "PlayerGraphic");
		for (auto i(0u); i < std::size(hitBox); ++i)
		{
			hitBox[i] = ECS::BlueBoxArcheType()((float)i * 100.f + 200.f, 532.f);
		}

		isReset = true;
	}
}

void Game::EventUpDate()
{
	Event::CreateAttackCollision()();
	Event::SceneChange()();
	Event::HitEvent()();
	Event::PlayerLandingCheke()();
	Event::PlayerHeadCheck()();
	Event::PlayerSideCheck()();
}

void Game::Update()
{
	const auto& backs = pManager->GetEntitiesByGroup(ENTITY_GROUP::Back);
	const auto& enemys = pManager->GetEntitiesByGroup(ENTITY_GROUP::Enemy);
	const auto& players = pManager->GetEntitiesByGroup(ENTITY_GROUP::Player);
	const auto& maps = pManager->GetEntitiesByGroup(ENTITY_GROUP::Map);
	const auto& collisions = pManager->GetEntitiesByGroup(ENTITY_GROUP::PlayerAttackCollision);
	Input::Get().Update_Key();

	pManager->Refresh();
	EventUpDate();
	for (const auto& it : backs) { it->UpDate(); }
	switch (Game::GetScene().Current())
	{
	case Scene::Reset:
		ResetGame();
		Game::GetScene().Change(Game::Scene::Title);
		break;
	case Scene::Title:
		if (!isReset)
		{
			ResetGame();
			isReset = true;
		}
		break;
	case Scene::Play:
		//カメラの制限
		Camera::Get().pos.x = player->GetComponent<ECS::Position>().val.x - 400;
		//Camera::Get().pos.y = player->GetComponent<ECS::Position>().val.y - 480;
		Camera::Get().UpDate();
		Camera::Get().SetTopEnd(0);
		Camera::Get().SetBottomEnd(720);

		for (const auto& it : maps) { it->UpDate(); }
		for (const auto& it : players) { it->UpDate(); }
		for (const auto& it : collisions) { it->UpDate(); }
		for (const auto& it : enemys) { it->UpDate(); }
		isReset = false;
		break;
	case Scene::Pause:
		break;
	case Scene::End:
		break;
	}
}

void Game::Draw()
{
	
	switch (Game::GetScene().Current())
	{
	case Scene::Title:
		back->Draw2D();
		break;
	case Scene::Pause:
		[[fallthrough]];
	case Scene::Play:
		pManager->OrderByDraw(ENTITY_GROUP::Max);
		break;
	case Scene::End:
		back->Draw2D();
		break;
	}
#ifdef _DEBUG
	//デバッグ用
	switch (Game::GetScene().Current())
	{
	case Scene::Title:
		DrawFormatString(0, 0, 0, "Title");
		break;
	case Scene::Pause:
		DrawFormatString(50, 0, 0, "Puase");
		[[fallthrough]];
	case Scene::Play:
		DrawFormatString(0, 0, 0, "Play");
		DrawFormatString(200, 630, 0, "左右キーで移動\nZキーで攻撃\nスペースでジャンプ");
		break;
	case Scene::End:
		DrawFormatString(0, 0, 0, "End");
		break;
	}
	DrawFormatString(0, 400, 0,
		R"(Xキーで次のシーン
Rキーでリセット
ポーズ中にZキーでエンドシーン)");
#endif
}