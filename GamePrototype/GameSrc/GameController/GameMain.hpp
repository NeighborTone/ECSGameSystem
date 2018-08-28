#pragma once
#include "../GameSrc/System/System.hpp"
#include "./Test/Game.h"
#include "../GameSrc/Utility/Utility.hpp"
class GameMain
{
private:
	System system;
	Game game;
	void Update()
	{
		game.Update();
	}
	void Draw()
	{
		game.Draw();
	}
	const bool PushEscape() const
	{
		return GetAsyncKeyState(VK_ESCAPE) & 0x8001;
	}
public:
	void Run()
	{
		while (system.IsOk() && !PushEscape())
		{
			{
				//ˆ—•‰‰×‚ğŒv‘ª‚·‚é
				ProcessingTime<std::chrono::nanoseconds> time;
				Update();
				Draw();
				//DOUT << System::GetFPS().GetFrameRate() << std::endl;
			}
		}
	}
};