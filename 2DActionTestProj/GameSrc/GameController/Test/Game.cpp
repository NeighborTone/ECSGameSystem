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
#include <fstream>
#include <atltime.h>
#include "MapLoader.hpp"
void Game::ResourceLoad()
{
	//リソースクラスのテスト
	ResourceManager::GetGraph().LoadDiv("Resource/Act_Chara2.png", "PlayerGraphic", 48, 6, 8, 64, 64);
	ResourceManager::GetGraph().Load("Resource/map2.png", "back");
	ResourceManager::GetGraph().Load("Resource/back.png", "bg1");
	ResourceManager::GetGraph().Load("Resource/retroTile_A4.png", "map1");
	ResourceManager::GetGraph().Load("Resource/gothic-castle-background.png", "map2");
	ResourceManager::GetSound().Load("Resource/Town.wav", "bgm");
	ResourceManager::GetSound().Load("Resource/タマネギ.ogg", "hitSE");
	ResourceManager::GetSound().Load("Resource/slashing04.ogg", "atkSE");
}

Game::Game()
{
	{
		//処理負荷を計測する
		ProcessingTime<std::chrono::milliseconds> time;
		pManager = &ECS::EcsSystem::GetManager();
		ResourceLoad();
		PlaySoundMem(ResourceManager::GetSound().GetHandle("bgm"), DX_PLAYTYPE_LOOP);
		ChangeVolumeSoundMem(255, ResourceManager::GetSound().GetHandle("bgm"));
		//ArcheType(原型)から作る
		//ECS::BackArcheType()("bg1");
		player = ECS::PlayerArcheType()(100.f, 800.f, "PlayerGraphic");
		for (auto i(0u); i < std::size(hitBox); ++i)
		{
			hitBox[i] = ECS::BlueBoxArcheType()((float)i * 100.f + 200.f, 800.f);
		}
		MapLoader("Resource/untitled.csv",50,15,64,ECS::TileMapArcheType(),"map1");
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
			hitBox[i] = ECS::BlueBoxArcheType()((float)i * 100.f + 200.f, 800.f);
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
	const auto& enemys = pManager->GetEntitiesByGroup(ENTITY_GROUP::Enemy);
	const auto& players = pManager->GetEntitiesByGroup(ENTITY_GROUP::Player);
	const auto& maps = pManager->GetEntitiesByGroup(ENTITY_GROUP::Map);
	const auto& collisions = pManager->GetEntitiesByGroup(ENTITY_GROUP::PlayerAttackCollision);
	Input::Get().Update_Key();

	pManager->Refresh();
	EventUpDate();
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
		Camera::Get().pos.x = player->GetComponent<ECS::Position>().val.x - System::SCREEN_WIDIH / 3;
		Camera::Get().pos.y = 0;
		Camera::Get().UpDate();
		Camera::Get().SetTopEnd(0);
		Camera::Get().SetBottomEnd(System::SCREEN_HEIGHT);
		Camera::Get().SetRightEnd(1640);
		Camera::Get().SetLeftEnd(0);
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
		break;
	case Scene::Pause:
		[[fallthrough]];
	case Scene::Play:
		pManager->OrderByDraw(ENTITY_GROUP::Max);
		break;
	case Scene::End:
		break;
	}
#ifdef _DEBUG
	//デバッグ用
	switch (Game::GetScene().Current())
	{
	case Scene::Title:
		DrawFormatString(0, 0, 0xffffffffu, "Title");
		break;
	case Scene::Pause:
		DrawFormatString(50, 0, 0xffffffffu, "Puase");
		[[fallthrough]];
	case Scene::Play:
		DrawFormatString(0, 0, 0xffffffffu, "Play");
		DrawFormatString(200, 630, 0xffffffffu, "左右キーで移動\nZキーで攻撃\nスペースでジャンプ");
		break;
	case Scene::End:
		DrawFormatString(0, 0, 0xffffffffu, "End");
		break;
	}

	DrawFormatString(0, 400, 0xffffffffu,
R"(Xキーで次のシーン
Rキーでリセット
ポーズ中にZキーでエンドシーン)");
#endif
}