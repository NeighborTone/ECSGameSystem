#pragma once
#include "../ECS/ECS.hpp"
#include "../Utility/Vec.hpp"
#include "../ResourceManager/ResourceManager.hpp"
#include "../GameSrc/Utility/Counter.hpp"
#include "../Input/Input.hpp"
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


	class Direction : public ComponentData
	{
	public:
		enum class Dir : short
		{
			R,
			L,
			U,
			D
		};
		Dir val;
		Direction() : val(Dir::R) {};
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
			if (!entity->HasComponent<Position>())
			{
				entity->AddComponent<Position>(0.f, 0.f);
			}
			if (!entity->HasComponent<Rotation>())
			{
				entity->AddComponent<Rotation>(0.f);
			}
			if (!entity->HasComponent<Scale>())
			{
				entity->AddComponent<Scale>(1.f);
			}
			if (!entity->HasComponent<Velocity>())
			{
				entity->AddComponent<Velocity>();
			}
			pos = &entity->GetComponent<Position>();
			rota = &entity->GetComponent<Rotation>();
			scale = &entity->GetComponent<Scale>();
			velocity = &entity->GetComponent<Velocity>();
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
			pos = &entity->GetComponent<Position>();
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
			if (!entity->IsActive())
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
				entity->Destroy();
			}
		}
		void Kill()
		{
			cnt = 0;
		}
		void SetKillLimit(const int limit)
		{
			cnt = limit;
		}
	};
	//簡易画像描画
	class SimpleDraw : public Component
	{
	private:
		Position* pos = nullptr;
		std::string name;
	public:
		SimpleDraw(const char* name_)
		{
			if (!ResourceManager::GetGraph().IsExistenceHandle(name_))
			{
				assert(false);
			}
			name = name_;
		}
		void Initialize() override
		{
			pos = &entity->GetComponent<Position>();
		}
		void Draw2D() override
		{
			if (ResourceManager::GetGraph().IsExistenceHandle(name))
			{
				DrawGraphF(pos->val.x, pos->val.y, ResourceManager::GetGraph().GetHandle(name), true);
			}
			
		}
	};
	//Animation
	class AnimationDraw : public Component
	{
	private:
		Position* pos = nullptr;
		Direction* dir = nullptr;
		std::string name;
		int index = 0;
		bool isTurn = false;
	public:
		AnimationDraw(const char* name_)
		{
			if (!ResourceManager::GetGraph().IsExistenceDivHandle(name_))
			{
				assert(false);
			}
			name = name_;
		}
		void Initialize() override
		{
			if (entity->HasComponent<Direction>())
			{
				dir = &entity->GetComponent<Direction>();
			}
			pos = &entity->GetComponent<Position>();
		}
		void UpDate() override
		{
			if (dir->val == Direction::Dir::R)
			{
				isTurn = false;
			}
			if (dir->val == Direction::Dir::L)
			{
				isTurn = true;
			}
		}
		void Draw2D() override
		{
			if (ResourceManager::GetGraph().IsExistenceDivHandle(name))
			{
				if (!isTurn)
				{
					DrawGraphF(pos->val.x, pos->val.y, ResourceManager::GetGraph().GetDivHandle(name, index), true);
				}
				else
				{
					DrawTurnGraphF(pos->val.x, pos->val.y, ResourceManager::GetGraph().GetDivHandle(name, index), true);
				}
			}
		}
		void SetIndex(const int index_)
		{
			index = index_;
		}
		
	};

	class InputMove : public Component
	{
	private:
		Position * pos = nullptr;
		Direction* pDir = nullptr;
		Velocity* vel = nullptr;
		Position pre;
		bool isMove = false;
		bool isStop = false;
	public:
		InputMove() = default;
		void Initialize() override
		{
			pos = &entity->GetComponent<Position>();
			pDir = &entity->GetComponent<Direction>();
			vel = &entity->GetComponent<Velocity>();
		}
		void UpDate() override
		{
			pre.val.x = pos->val.x;
			pre.val.y = pos->val.y;
			isMove = false;
			if (!isStop)
			{
				if (Input::GetKey(KEY_INPUT_RIGHT) >= 1)
				{
					pos->val.x += vel->val.x;
					pDir->val = Direction::Dir::R;
					isMove = true;
				}
				if (Input::GetKey(KEY_INPUT_LEFT) >= 1)
				{
					pos->val.x -= vel->val.x;
					pDir->val = Direction::Dir::L;
					isMove = true;
				}
				/*if (Input::GetKey(KEY_INPUT_UP) >= 1)
				{
					pos->val.y -= vel->val.y;
					pDir->val = Direction::Dir::U;
					isMove = true;
				}
				if (Input::GetKey(KEY_INPUT_DOWN) >= 1)
				{
					pos->val.y += vel->val.y;
					pDir->val = Direction::Dir::D;
					isMove = true;
				}*/
			}
		}
		//移動前の座標取得
		const Position& GetPrePos() const
		{
			return pre;
		}
		const bool IsMove() const
		{
			return isMove;
		}
		void Stop()
		{
			isStop = true;
		}
		void GoMove()
		{
			isStop = false;
		}
	};

	class InputAttack : public Component
	{
	private:
		bool isAttack = false;
	public:
		void UpDate() override 
		{
			if (Input::GetKey(KEY_INPUT_Z) == 1)
			{

				isAttack = true;
			}
			
		}
		void ResetAttack()
		{
			isAttack = false;
		}

		//攻撃モーション中
		const bool IsAttacking() const
		{
			return isAttack;
		}
		//攻撃した瞬間だけtrue
		const bool IsAttacked() const
		{
			return Input::GetKey(KEY_INPUT_Z) == 1 && !isAttack;
		}
	};

	class PlayerAnimation : public Component
	{
	private:
		InputAttack* attack;
		InputMove* input;
		AnimationDraw* anim;
		Counter_f idol;
		Counter_f running;
		Counter_f attacking;
	public:
		void Initialize() override
		{
			input = &entity->GetComponent<InputMove>();
			anim = &entity->GetComponent<AnimationDraw>();
			attack = &entity->GetComponent<InputAttack>();
			idol.SetCounter(1.f, 0.15f, 1.f, 5.f);
			running.SetCounter(6.f, 0.15f, 6.f, 11.f);
			attacking.SetCounter(24.f,0.2f,24.f,29.f);
		}
		void UpDate() override
		{
			if (attack->IsAttacking())
			{
				input->Stop();
				anim->SetIndex(static_cast<int>(++attacking));
				if (attacking.IsMax())
				{
					attacking.Reset();
					attacking.SetCounter(24.f,0.2f,24.f,29.f);
					attack->ResetAttack();
					input->GoMove();
				}
			}
			else if (input->IsMove())
			{
				anim->SetIndex(static_cast<int>(++running));
			}
			else
			{
				anim->SetIndex(static_cast<int>(++idol));
			}
		}
	};
}
