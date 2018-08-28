#pragma once
#include <string>
#include <iostream>
namespace Event
{
	struct Message
	{
		static const std::string WhiteBox;
		static const std::string RedBox;
	};
	const std::string Message::WhiteBox = "white";
	const std::string Message::RedBox = "red";
}
