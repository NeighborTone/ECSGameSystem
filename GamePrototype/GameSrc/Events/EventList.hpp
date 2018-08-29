#pragma once
#include <string>
#include <iostream>
namespace Event
{
	struct Message
	{
		static const std::string WhiteBox;
		static const std::string RedBox;
		static const std::string PlayerAttack;
	};
	const std::string Message::WhiteBox = "white";
	const std::string Message::RedBox = "red";
	const std::string Message::PlayerAttack = "attack!";
}
