#pragma once
#include "../Utility/Vec.hpp"
#include "../System/System.hpp"
#include <memory>
class Camera
{
private:
	class Singleton
	{
	private:
		float leftEnd = 0;
		float rightEnd;
		float topEnd = 0;
		float bottomEnd;
	public:
		Singleton(const Vec2& position, const Vec2& size_)
			:pos(position), rightEnd(size_.x),bottomEnd(size_.y) {}
	public:
		Vec2 pos;
		Vec2 size;
	
		void UpDate()
		{
			if (Camera::Get().pos.x < leftEnd) { Camera::Get().pos.x = leftEnd; }
			if (Camera::Get().pos.y < topEnd) { Camera::Get().pos.y = topEnd; }
			if (Camera::Get().pos.x > rightEnd) { Camera::Get().pos.x = rightEnd; }
			if (Camera::Get().pos.y > bottomEnd) { Camera::Get().pos.y = bottomEnd; }
		}
		void SetLeftEnd(const float val)
		{
			leftEnd = val;
		}
		void SetRightEnd(const float val)
		{
			rightEnd = val;
		}
		void SetTopEnd(const float val)
		{
			topEnd = val;
		}
		void SetBottomEnd(const float val)
		{
			bottomEnd = val;
		}
	};

public:
	inline static Singleton& Get()
	{
		static std::unique_ptr<Singleton> inst =
			std::make_unique<Singleton>(Vec2(0, 0), Vec2(System::SCREEN_WIDIH, System::SCREEN_HEIGHT));
		return *inst;
	}
};