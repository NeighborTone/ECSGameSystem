#pragma once
#include "../GameSrc/System/System.hpp"
#include "./Test/Game.h"
#include "../GameSrc/Utility/Utility.hpp"
class GameMain
{
private:
	std::unique_ptr<System> system;
	std::unique_ptr<Game> game;

	void Update()
	{
		game->Update();
	}
	void Draw()
	{
		game->Draw();
	}
	const bool PushEscape() const
	{
		return GetAsyncKeyState(VK_ESCAPE) & 0x8001;
	}
public:
	GameMain()
	{
		system = std::make_unique<System>();
		game = std::make_unique<Game>();
	}
	void Run()
	{
		while (system->IsOk() && !PushEscape())
		{
			{
				//ˆ—•‰‰×‚ğŒv‘ª‚·‚é
				ProcessingTime<std::chrono::milliseconds> time;
				Update();
				Draw();
				System::GetFPS().Wait();
			}
		}
	}
};