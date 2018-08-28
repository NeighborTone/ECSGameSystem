#include "Game.h"
#include "../GameSrc/System/System.hpp"
#include "../GameSrc/Components/Transform.hpp"
#include "../GameSrc/Events/Event1.hpp"
#include "../GameSrc/Events/Event2.hpp"
Game::Game()
{
	pManager = &ECS::EcsSystem::GetManager();
	white = &pManager->AddEntity();
	red = &pManager->AddEntity();
	white->AddComponent<ECS::Position>(System::SCREEN_WIDIH / 2.f, System::SCREEN_HEIGHT / 2.f);
	red->AddComponent<ECS::Position>(0.f,0.f);
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
}

void Game::Draw()
{
	pManager->Draw2D();
}