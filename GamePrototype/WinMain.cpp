#include <DxLib.h>
#include "GameSrc/GameController/GameMain.hpp"
#include "GameSrc/Utility/Console.hpp"
//���������[�N���m�p
#include <memory>
#include <new>
#include <crtdbg.h>
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	ShowConsole();
	GameMain game;
	game.Run();
}