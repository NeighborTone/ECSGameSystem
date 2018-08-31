#pragma once
#include <string>
#include <iostream>
namespace Event
{
	struct Message
	{
		inline static const std::string Hit = "hit!!";
		inline static const std::string SceneChange = "changeScene";
		inline static const std::string PlayerAttack = "attack!";
		inline static const std::string Landing = "landing!";
		inline static const std::string Heading = "heading!";
	};
}
