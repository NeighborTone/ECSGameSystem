#pragma once
#include <string>
#include <iostream>
namespace Event
{
	struct Message
	{
		static const std::string Hit;
		static const std::string SceneChange;
		static const std::string PlayerAttack;
		static const std::string Landing;
	};
	const std::string Message::Hit = "hit!!";
	const std::string Message::SceneChange = "changeScene";
	const std::string Message::PlayerAttack = "attack!";
	const std::string Message::Landing = "landing!";
}
