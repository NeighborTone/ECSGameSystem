#include "Game.h"
#include "../GameSrc/Utility/Utility.hpp"
#include "../GameSrc/System/System.hpp"
#include "../GameSrc/Components/Transform.hpp"
#include "../GameSrc/Events/Event1.hpp"
#include "../GameSrc/Events/Event2.hpp"
#include "../GameSrc/ResourceManager/ResourceManager.hpp"
#include <atltime.h>
Game::Game()
{
	{
		//�������ׂ��v������
		ProcessingTime<std::chrono::milliseconds> time;
		pManager = &ECS::EcsSystem::GetManager();
		//�𒼂�Entity����遫
		white = &pManager->AddEntity();
		red = &pManager->AddEntity();
		white->AddComponent<ECS::Position>(System::SCREEN_WIDIH / 2.f, System::SCREEN_HEIGHT / 2.f);
		white->AddGroup(GameGroup::Box);
		red->AddComponent<ECS::Position>(0.f, 0.f);
		red->AddGroup(GameGroup::Box);
		//���\�[�X�N���X�̃e�X�g
		ResourceManager::GetGraph().Load("Resource/a.png", "a");
		red->AddComponent<ECS::SimpleDraw>("a");
		
		//ArcheType(���^)�����遫
		green = GreenBoxArcheType()();
		for (int i = 0; i < 5; ++i)
		{
			blue[i] = BlueBoxArcheType()((float)i * 20 + 200 ,(float)i * 20 + 200);
		}
		hoge = ColorBoxArcheType()(128, 243, 213);
	}
}

void Game::Initialize()
{
	pManager->Initialize();
}

void Game::Update()
{
	pManager->Refresh();
	//pManager->UpDate();
	const auto& b = pManager->GetEntitiesByGroup(GameGroup::Box);
	for (const auto& it : b) { it->UpDate(); }
	//Event�ɂ����Box�����
	Event::AddWhiteBox()(*white);
	Event::AddRedBox()(*red);
	//���\�[�X�N���X�̓���e�X�g
	if (GetAsyncKeyState(VK_BACK) & 0x8001)
	{
		ResourceManager::GetGraph().ReMoveGraph("a");
	}
}

void Game::Draw()
{
	//pManager->Draw2D();
	const auto& b = pManager->GetEntitiesByGroup(GameGroup::Box);
	for (const auto& it : b) { it->Draw2D(); }
}