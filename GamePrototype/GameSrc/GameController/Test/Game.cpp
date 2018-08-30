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
#include <atltime.h>

Game::Game()
{
	{
		//�������ׂ��v������
		ProcessingTime<std::chrono::milliseconds> time;
		pManager = &ECS::EcsSystem::GetManager();
		//���\�[�X�N���X�̃e�X�g
		ResourceManager::GetGraph().LoadDiv("Resource/Act_Chara2.png", "PlayerGraphic",48,6,8,64,64);
		ResourceManager::GetSound().Load("Resource/Grass.wav","bgm");
		ResourceManager::GetSound().Load("Resource/�^�}�l�M.ogg", "hitSE");
		PlaySoundMem(ResourceManager::GetSound().GetHandle("bgm"), DX_PLAYTYPE_LOOP);
		//ArcheType(���^)������
		player = ECS::PlayerArcheType()(100.f, 500.f, "PlayerGraphic");
		ground = ECS::GreenBoxArcheType()(0.f, 600.f);
		for (auto i(0u); i < std::size(hitBox); ++i)
		{
			hitBox[i] = ECS::BlueBoxArcheType()((float)i * 100.f + 200.f, 500.f);
		}
	}
}

void Game::ResetGame()
{
	//pManager->Initialize();
	const auto& b = pManager->GetEntitiesByGroup(ENTITY_GROUP::Enemy);
	const auto& p = pManager->GetEntitiesByGroup(ENTITY_GROUP::Player);
	const auto& c = pManager->GetEntitiesByGroup(ENTITY_GROUP::PlayerAttackCollision);
	for (auto& it : p) { it->Destroy(); }
	for (auto& it : b) { it->Destroy(); }
	for (auto& it : c) { it->Destroy(); }

	player = ECS::PlayerArcheType()(100.f, 500.f, "PlayerGraphic");
	for (auto i(0u); i < std::size(hitBox); ++i)
	{
		hitBox[i] = ECS::BlueBoxArcheType()((float)i * 100.f + 200.f, 500.f);
	}
	
	isReset = true;
}

void Game::EventUpDate()
{
	Event::CreateAttackCollision()();
	Event::SceneChange()();
	Event::HitEvent()();
	Event::PlayerLanding()();
}

void Game::Update()
{
	const auto& b = pManager->GetEntitiesByGroup(ENTITY_GROUP::Enemy);
	const auto& p = pManager->GetEntitiesByGroup(ENTITY_GROUP::Player);
	const auto& m = pManager->GetEntitiesByGroup(ENTITY_GROUP::Map);
	const auto& c = pManager->GetEntitiesByGroup(ENTITY_GROUP::PlayerAttackCollision);
	Input::Update_Key();
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
		for (const auto& it : m) { it->UpDate(); }
		for (const auto& it : p) { it->UpDate(); }
		for (const auto& it : c) { it->UpDate(); }
		for (const auto& it : b) { it->UpDate(); }
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
	//pManager->Draw2D();
	const auto& m = pManager->GetEntitiesByGroup(ENTITY_GROUP::Map);
	const auto& b = pManager->GetEntitiesByGroup(ENTITY_GROUP::Enemy);
	const auto& p = pManager->GetEntitiesByGroup(ENTITY_GROUP::Player);
	const auto& c = pManager->GetEntitiesByGroup(ENTITY_GROUP::PlayerAttackCollision);

	
	switch (Game::GetScene().Current())
	{
	case Scene::Title:
		DrawFormatString(0, 0, 0xffffffffu, "Title");
		break;
	case Scene::Pause:
		DrawFormatString(50, 0, 0xffffffffu, "Puase");
	case Scene::Play:
		DrawFormatString(0, 0, 0xffffffffu, "Play");
		DrawFormatString(200, 630, 0xffffffffu, "���E�L�[�ňړ�\nZ�L�[�ōU��\n�X�y�[�X�ŃW�����v");
		for (const auto& it : m) { it->Draw2D(); }
		for (const auto& it : b) { it->Draw2D(); }
		for (const auto& it : c) { it->Draw2D(); }
		for (const auto& it : p) { it->Draw2D(); }
		break;
	case Scene::End:
		DrawFormatString(0, 0, 0xffffffffu, "End");
		break;
	}
	DrawFormatString(0, 400, 0xffffffffu, 
R"(X�L�[�Ŏ��̃V�[��
R�L�[�Ń��Z�b�g
�|�[�Y����Z�L�[�ŃG���h�V�[��)");
}