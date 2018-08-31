#pragma once
#include "../Utility/Vec.hpp"
#include "../System/System.hpp"
#include <memory>
class Camera
{
private:
	class Singleton
	{
	public:
		Singleton(const Vec2&& position, const Vec2&& size_)
			:pos(position), size(size_) {}
	public:
		Vec2 pos;
		Vec2 size;
		void UpDate()
		{
			if (Camera::Get().pos.x < 0) { Camera::Get().pos.x = 0; }
			if (Camera::Get().pos.y < -500) { Camera::Get().pos.y = -500; }
			if (Camera::Get().pos.x > Camera::Get().size.x) { Camera::Get().pos.x = Camera::Get().size.x; }
			if (Camera::Get().pos.y > Camera::Get().size.y) { Camera::Get().pos.y = Camera::Get().size.y; }
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