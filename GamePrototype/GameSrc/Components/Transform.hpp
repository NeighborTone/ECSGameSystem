#pragma once
#include "../ECS/ECS.hpp"
#include "../Utility/Vec.hpp"
#include <DxLib.h>
namespace ECS
{
	struct Position : public ComponentData
	{
		Vec2 val;
		Position() = default;
		Position(const Vec2& v) :val(v){}
		Position(const float& x, const float& y) :val(x,y) {}
	};

	struct Rotation : public ComponentData
	{
		float val;
		Rotation() = default;
		Rotation(const float& r) : val(r) {}
	};

	struct Scale : public ComponentData
	{
		float val;
		Scale() = default;
		Scale(const float& scale) : val(scale) {}
	};

	struct Velocity : public ComponentData
	{
		Vec2 val;
		Velocity() = default;
		Velocity(const Vec2& v) :val(v) {}
		Velocity(const float& x, const float& y) : val(x,y){}
	};

	class Transform : public Component
	{
	private:
		Position * pos;
		Rotation* rota;
		Scale* scale;
		Velocity* velocity;
	public:
		Transform() :pos(nullptr), rota(nullptr) {}
		void Initialize() override
		{
			if (!white->HasComponent<Position>())
			{
				white->AddComponent<Position>(0.f, 0.f);
			}
			if (!white->HasComponent<Rotation>())
			{
				white->AddComponent<Rotation>(0.f);
			}
			if (!white->HasComponent<Scale>())
			{
				white->AddComponent<Scale>(1.f);
			}
			if (!white->HasComponent<Velocity>())
			{
				white->AddComponent<Velocity>();
			}
			pos = &white->GetComponent<Position>();
			rota = &white->GetComponent<Rotation>();
			scale = &white->GetComponent<Scale>();
			velocity = &white->GetComponent<Velocity>();
		}
		void SetPosition(const float& x, const float& y)
		{
			pos->val.x = x;
			pos->val.y = y;
		}
		void SetRotation(const float& r)
		{
			rota->val = r;
		}
		void SetScale(const float& scale_)
		{
			scale->val = scale_;
		}
		void SetVelocity(const float& x, const float& y)
		{
			velocity->val.x = x;
			velocity->val.y = y;
		}
	};

	//$Test$
	class IBoxColiider
	{
	public:
		virtual float w() const = 0;
		virtual float h() const = 0;
		virtual float x() const = 0;
		virtual float y() const = 0;
	};

	//矩形の定義
	class HitBase : public Component, public IBoxColiider
	{
	private:
		Position * pos = nullptr;
		float W, H;
		unsigned int color = 4294967295;
		bool isFill = false;
		bool isDraw = true;
	public:
		explicit HitBase(const float ww, const float hh)
		{
			W = ww;
			H = hh;
		}

		~HitBase()
		{
			pos = nullptr;
		}
		void Initialize() override
		{
			pos = &white->GetComponent<Position>();
		}
		void SetColor(const int r, const int g, const int b)
		{
			color = GetColor(r, g, b);
		}
		void FillEnable() { isFill = true; }
		void FillDisable() { isFill = false; }
		void DrawEnable() { isDraw = true; }
		void DrawDisable() { isDraw = false; }
		void Draw2D() override
		{
			if (!white->IsActive())
			{
				DrawDisable();
			}
			if (isDraw)
			{
				DrawBoxAA(
					pos->val.x,
					pos->val.y,
					pos->val.x + w(),
					pos->val.y + h(),
					color, isFill, 2);
			}

		}
		float w() const { return W; }
		float h() const { return H; }
		float x() const { return pos->val.x; }
		float y() const { return pos->val.y; }
	};
	//指定したフレーム後にEntityを殺す
	class KillEntity : public Component
	{
	private:
		int cnt;
	public:
		KillEntity(const int span) :cnt(span) {}

		void UpDate() override
		{
			--cnt;
			if (cnt <= 0)
			{
				white->Destroy();
			}
		}
		void Kill()
		{
			cnt = 0;
		}
	};
}
