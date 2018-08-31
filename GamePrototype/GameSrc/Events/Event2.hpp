#pragma once
#include "Observer.hpp"
#include "../ECS/ECS.hpp"
#include "EventList.hpp"
#include "../Components/Components.hpp"
#include "../Utility/Utility.hpp"
#include "../GameSrc/GameController/Test/Game.h"
#include <Windows.h>
namespace Event
{
	class SceneNotify : public Subject<SceneNotify>
	{
	public:
		//doSomething()‚Å’Ê’m
		void doSomething() {
			//ÀÛ‚É‚µ‚Ä‚Ù‚µ‚¢ˆ—(Observer)‚É’Ê’m‚ğ‘—‚é
			Call(Message::SceneChange);
		}
	};

	class SceneChange : public Observer<SceneNotify, Message::SceneChange>
	{
	private:
		SceneNotify * pTest;
	public:
		void operator()()
		{
			pTest = new SceneNotify();
			pTest->AddObserver(this);
			pTest->doSomething();
		}
		~SceneChange()
		{
			Memory::SafeDelete(pTest);

		}
		void UpDate([[maybe_unused]]SceneNotify* sender,
			[[maybe_unused]]const std::string& key_) override
		{
			if (Input::Get().GetKey(KEY_INPUT_R) == 1)
			{
				Game::GetScene().Change(Game::Scene::Reset);
				DOUT << "Reset" << std::endl;
			}
			switch (Game::GetScene().Current())
			{
			case Game::Scene::Title:
				if (Input::Get().GetKey(KEY_INPUT_X) == 1)
				{
					Game::GetScene().Change(Game::Scene::Play);
					DOUT << "Play" << std::endl;
				}
				break;
			case Game::Scene::Play:
				if (Input::Get().GetKey(KEY_INPUT_X) == 1)
				{
					Game::GetScene().Change(Game::Scene::Pause);
					DOUT << "Pause" << std::endl;
				}
				break;
			case Game::Scene::Pause:
				if (Input::Get().GetKey(KEY_INPUT_X) == 1)
				{
					Game::GetScene().Change(Game::Scene::Play);
					DOUT << "Play" << std::endl;
				}
				if (Input::Get().GetKey(KEY_INPUT_Z) == 1)
				{
					Game::GetScene().Change(Game::Scene::End);
					DOUT << "End" << std::endl;
				}
				break;
			case Game::Scene::End:
				if (Input::Get().GetKey(KEY_INPUT_X) == 1)
				{
					Game::GetScene().Change(Game::Scene::Title);
					DOUT << "Title" << std::endl;
				}
				break;
			}
		}
	};

}
