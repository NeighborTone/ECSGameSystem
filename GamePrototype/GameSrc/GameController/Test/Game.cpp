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
		//ˆ—•‰‰×‚ğŒv‘ª‚·‚é
		ProcessingTime<std::chrono::milliseconds> time;
		pManager = &ECS::EcsSystem::GetManager();
		white = &pManager->AddEntity();
		red = &pManager->AddEntity();
		white->AddComponent<ECS::Position>(System::SCREEN_WIDIH / 2.f, System::SCREEN_HEIGHT / 2.f);
		red->AddComponent<ECS::Position>(0.f, 0.f);

		ResourceManager::GetGraph().Load("Resource/a.png", "a");
		red->AddComponent<ECS::SimpleDraw>("a");
	}
}

void Game::Initialize()
{
	pManager->Initialize();
}

void Game::Update()
{
	pManager->Refresh();
	pManager->UpDate();

	Event::AddWhiteBox()(*white);
	Event::AddRedBox()(*red);

	if (GetAsyncKeyState(VK_BACK) & 0x8001)
	{
		ResourceManager::GetGraph().ReMoveGraph("a");
	}
}

void Game::Draw()
{
	pManager->Draw2D();
	
}