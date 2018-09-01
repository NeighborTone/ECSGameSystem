#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/Components.hpp"
struct Collision
{
	template<class T = ECS::HitBase, class T2 = ECS::HitBase>
	static bool BoxAndBox(const ECS::Entity& b1, const ECS::Entity& b2)
	{
		if (!b1.HasComponent<T>() || !b2.HasComponent<T2>())
		{
			return false;
		}
		const auto& box1 = b1.GetComponent<T>();
		const auto& box2 = b2.GetComponent<T2>();
		if (box1.x() < box2.x() + box2.w() &&
			box2.x() < box1.x() + box1.w() &&
			box1.y() < box2.y() + box2.h() &&
			box2.y() < box1.y() + box1.h())
		{
			return true;
		}
		return false;
	}
	static bool BoxAndBox(const Vec2& b1Pos, const Vec2& b1Size, const Vec2& b2Pos, const Vec2& b2Size)
	{
		if (b1Pos.x < b2Pos.x + b2Size.x &&
			b2Pos.x < b1Pos.x + b1Size.x &&
			b1Pos.y < b2Pos.y + b2Size.y &&
			b2Pos.y < b1Pos.y + b1Size.y)
		{
			return true;
		}
		return false;
	}
};
