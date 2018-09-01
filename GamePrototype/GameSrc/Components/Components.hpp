#pragma once
#include "../Camera/Camera.hpp"
#include "../ECS/ECS.hpp"
#include "../Utility/Vec.hpp"
#include "../ResourceManager/ResourceManager.hpp"
#include "../GameSrc/Utility/Counter.hpp"
#include "../Input/Input.hpp"
#include "../Collision/Collision.hpp"
#include <DxLib.h>
namespace ECS
{
	struct Position : public ComponentData
	{
		Vec2 val;
		Position() = default;
		Position(const Vec2& v) :val(v) {}
		Position(const float& x, const float& y) :val(x, y) {}
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
		Velocity(const float& x, const float& y) : val(x, y) {}
	};

	struct Direction : public ComponentData
	{
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

	struct Gravity : public ComponentData
	{
		float val;
		Gravity() = default;
		Gravity(const float g) :val(g) {}
	};

	struct AnimationState : public ComponentData
	{
		enum State : short
		{
			Idol,
			Jump,
			Fall,
			Run,
			Attack,
		};
		State val;
		AnimationState() = default;
		AnimationState(const State& state) : val(state) {}
	};

	class Physics : public Component
	{
	private:
		Gravity * gravity;
		Velocity* velocity;
	public:
		void Initialize() override
		{
			if (!entity->HasComponent<Gravity>())
			{
				entity->AddComponent<Gravity>();
			}
			if (!entity->HasComponent<Velocity>())
			{
				entity->AddComponent<Velocity>();
			}
			velocity = &entity->GetComponent<Velocity>();
			gravity = &entity->GetComponent<Gravity>();
		}

		void SetVelocity(const float& x, const float& y)
		{
			velocity->val.x = x;
			velocity->val.y = y;
		}

		void SetGravity(const float& g)
		{
			gravity->val = g;
		}
	};

	class Transform : public Component
	{
	private:
		Position * pos;
		Rotation* rota;
		Scale* scale;

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
			pos = &entity->GetComponent<Position>();
			rota = &entity->GetComponent<Rotation>();
			scale = &entity->GetComponent<Scale>();
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
	};

	//$Test$
	class IBoxColiider
	{
	public:
		IBoxColiider() = default;
		virtual ~IBoxColiider() = default;
		virtual void SetColor(const int r, const int g, const int b) = 0;
		virtual void SetOffset(const float x, const float y) = 0;
		virtual void FillEnable() = 0;
		virtual void FillDisable() = 0;
		virtual void DrawEnable() = 0;
		virtual void DrawDisable() = 0;
		virtual float w() const = 0;
		virtual float h() const = 0;
		virtual float x() const = 0;
		virtual float y() const = 0;
	};

	//コリジョンの重複を何とか許可したい
	//矩形の定義
	class HitBase : public Component, public IBoxColiider
	{
	private:
		Position * pos = nullptr;
		Vec2 offSetPos;
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
		void Draw2D() override
		{
			if (!entity->IsActive())
			{
				DrawDisable();
			}
			if (isDraw)
			{
				auto convert = pos->val.OffSetCopy(offSetPos.x, offSetPos.y);
				auto convert2 = convert - Camera::Get().pos;
				DrawBoxAA(
					convert2.x,
					convert2.y,
					convert2.x + w(),
					convert2.y + h(),
					color, isFill, 2);
			}
		}
		void SetColor(const int r, const int g, const int b) override
		{
			color = GetColor(r, g, b);
		}
		void SetOffset(const float x, const float y) override
		{
			offSetPos.x = x;
			offSetPos.y = y;
		}
		void FillEnable() override { isFill = true; }
		void FillDisable() override { isFill = false; }
		void DrawEnable() override { isDraw = true; }
		void DrawDisable() override { isDraw = false; }
		float w() const override { return W; }
		float h() const override { return H; }
		float x() const override { return pos->val.x + offSetPos.x; }
		float y() const override { return pos->val.y + offSetPos.y; }
	};
	//足用矩形の定義
	class FootBase : public Component, public IBoxColiider
	{
	private:
		Position * pos = nullptr;
		Vec2 offSetPos;
		float W, H;
		unsigned int color = 4294967295;
		bool isFill = false;
		bool isDraw = true;
	public:
		explicit FootBase(const float ww, const float hh)
		{
			W = ww;
			H = hh;
		}
		~FootBase()
		{
			pos = nullptr;
		}
		void Initialize() override
		{
			pos = &entity->GetComponent<Position>();
		}
		void Draw2D() override
		{
			if (!entity->IsActive())
			{
				DrawDisable();
			}
			if (isDraw)
			{
				auto convert = pos->val.OffSetCopy(offSetPos.x, offSetPos.y);
				auto convert2 = convert - Camera::Get().pos;
				DrawBoxAA(
					convert2.x,
					convert2.y,
					convert2.x + w(),
					convert2.y + h(),
					color, isFill, 2);
			}
		}
		void SetColor(const int r, const int g, const int b) override
		{
			color = GetColor(r, g, b);
		}
		void SetOffset(const float x, const float y) override
		{
			offSetPos.x = x;
			offSetPos.y = y;
		}
		void FillEnable() override { isFill = true; }
		void FillDisable() override { isFill = false; }
		void DrawEnable() override { isDraw = true; }
		void DrawDisable() override { isDraw = false; }
		float w() const override { return W; }
		float h() const override { return H; }
		float x() const override { return pos->val.x + offSetPos.x; }
		float y() const override { return pos->val.y + offSetPos.y; }
	};

	//頭矩形の定義
	class HeadBase : public Component, public IBoxColiider
	{
	private:
		Position * pos = nullptr;
		Vec2 offSetPos;
		float W, H;
		unsigned int color = 4294967295;
		bool isFill = false;
		bool isDraw = true;
	public:
		explicit HeadBase(const float ww, const float hh)
		{
			W = ww;
			H = hh;
		}
		~HeadBase()
		{
			pos = nullptr;
		}
		void Initialize() override
		{
			pos = &entity->GetComponent<Position>();
		}
		void Draw2D() override
		{
			if (!entity->IsActive())
			{
				DrawDisable();
			}
			if (isDraw)
			{
				auto convert = pos->val.OffSetCopy(offSetPos.x, offSetPos.y);
				auto convert2 = convert - Camera::Get().pos;
				DrawBoxAA(
					convert2.x,
					convert2.y,
					convert2.x + w(),
					convert2.y + h(),
					color, isFill, 2);
			}
		}
		void SetColor(const int r, const int g, const int b) override
		{
			color = GetColor(r, g, b);
		}
		void SetOffset(const float x, const float y) override
		{
			offSetPos.x = x;
			offSetPos.y = y;
		}
		void FillEnable() override { isFill = true; }
		void FillDisable() override { isFill = false; }
		void DrawEnable() override { isDraw = true; }
		void DrawDisable() override { isDraw = false; }
		float w() const override { return W; }
		float h() const override { return H; }
		float x() const override { return pos->val.x + offSetPos.x; }
		float y() const override { return pos->val.y + offSetPos.y; }
	};

	//側面矩形の定義
	class SideBase : public Component, public IBoxColiider
	{
	private:
		Position * pos = nullptr;
		Direction* dir = nullptr;
		Vec2 offSetPos;
		float W, H;
		unsigned int color = 4294967295;
		bool isFill = false;
		bool isDraw = true;
	public:
		explicit SideBase(const float ww, const float hh)
		{
			W = ww;
			H = hh;
		}
		~SideBase()
		{
			pos = nullptr;
		}
		void Initialize() override
		{
			pos = &entity->GetComponent<Position>();
			dir = &entity->GetComponent<Direction>();
		}
		void Draw2D() override
		{
			if (!entity->IsActive())
			{
				DrawDisable();
			}
			if (isDraw)
			{
				auto convert = pos->val.OffSetCopy(offSetPos.x, offSetPos.y);
				auto convert2 = convert - Camera::Get().pos;
				if (dir->val == Direction::Dir::R)
				{
					DrawBoxAA(
						convert2.x + 50,
						convert2.y,
						convert2.x + 50 + w(),
						convert2.y + h(),
						color, isFill, 2);
				}
				else if (dir->val == Direction::Dir::L)
				{
				
					DrawBoxAA(
						convert2.x,
						convert2.y,
						convert2.x + w(),
						convert2.y + h(),
						color, isFill, 2);
				}
				
				
			}
		}
		void SetColor(const int r, const int g, const int b) override
		{
			color = GetColor(r, g, b);
		}
		void SetOffset(const float x, const float y) override
		{
			offSetPos.x = x;
			offSetPos.y = y;
		}
		void FillEnable() override { isFill = true; }
		void FillDisable() override { isFill = false; }
		void DrawEnable() override { isDraw = true; }
		void DrawDisable() override { isDraw = false; }
		float w() const override { return W; }
		float h() const override { return H; }
		float x() const override 
		{
			if (dir->val == Direction::Dir::R)
			{
				return pos->val.x + 50 + offSetPos.x;
			}
			return pos->val.x + offSetPos.x;
		}
		float y() const override { return pos->val.y + offSetPos.y; }
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
		Position * pos = nullptr;
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
				auto convert = pos->val - Camera::Get().pos;
				DrawGraphF(convert.x, convert.y, ResourceManager::GetGraph().GetHandle(name), true);
			}
		}
	};
	class RectDraw : public Component
	{
	private:
		Position * pos = nullptr;
		RECT rect;
		std::string name;
	public:
		RectDraw(const char* name_,const int srcX, const int srcY, const int w, const int h)
		{
			if (!ResourceManager::GetGraph().IsExistenceHandle(name_))
			{
				assert(false);
			}
			rect.left = srcX;
			rect.right = srcY;
			rect.bottom = w;
			rect.top = h;
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
				auto convert = pos->val - Camera::Get().pos;
				//スクリーン外は描画しない
				if (Collision::BoxAndBox(
					Vec2(0,0),
					Vec2(System::SCREEN_WIDIH,System::SCREEN_HEIGHT), 
					convert,
					Vec2(float(rect.bottom), float(rect.top))))
				{
					DrawRectGraphF(convert.x, convert.y, rect.left, rect.right, rect.bottom, rect.top, ResourceManager::GetGraph().GetHandle(name), true);
				}
			}
		}
	};
	//Animation
	class AnimationDraw : public Component
	{
	private:
		Position * pos = nullptr;
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
				auto convert = pos->val - Camera::Get().pos;
				if (!isTurn)
				{
					DrawGraphF(convert.x, convert.y, ResourceManager::GetGraph().GetDivHandle(name, index), true);
				}
				else
				{
					DrawTurnGraphF(convert.x, convert.y, ResourceManager::GetGraph().GetDivHandle(name, index), true);
				}
			}
		}
		//分割画像の要素番号を指定する
		void SetIndex(const int index_)
		{
			index = index_;
		}
	};

	class InputJump : public Component
	{
	private:
		Position * pos = nullptr;
		Velocity* vel = nullptr;
		Gravity* gravity;
		bool isStop = false;
		bool isLanding;
		bool isHeading;
		bool isFall;
		bool isJump;
		float jumpPow;
	public:
		InputJump(const float pow)
		{
			jumpPow = pow;
		}
		void Initialize() override
		{
			pos = &entity->GetComponent<Position>();
			gravity = &entity->GetComponent<Gravity>();
			vel = &entity->GetComponent<Velocity>();
		}
		void UpDate() override
		{
			//頭をぶつけた
			if (IsJumping())
			{
				if (isHeading)
				{
					vel->val.y = 0.0f;
				}
			}
			//着地中
			if (isLanding)
			{
				vel->val.y = 0.0f;
			}
			else
			{
				vel->val.y += gravity->val * 32 * 3;
			}
			if (!isStop)
			{
				if (isLanding && Input::Get().GetKey(KEY_INPUT_SPACE) == 1)
				{
					vel->val.y = jumpPow;
				}
				if (IsJumping() && Input::Get().GetKey(KEY_INPUT_SPACE) == 0)
				{
					vel->val.y *= 0.8f;
				}
			}
			//落下速度が負の値ならジャンプ中
			if (vel->val.y < 0)
			{
				isJump = true;
				isFall = false;
			}
			else
			{
				isJump = false;
				isFall = true;
			}
			pos->val.y += vel->val.y;
		}
		//ジャンプ中か
		const bool IsJumping() const { return isJump; }
		//落下中か
		const bool IsFalling() const { return isFall; }
		//着地中か
		const bool IsLanding() const { return isLanding; }
		//入力を無効にする
		void Stop() { isStop = true; }
		//入力を許可する
		void GoMove() { isStop = false;	}
		//足元判定をセット
		void Landing(const bool hit) { isLanding = hit; }
		//頭上判定をセット
		void Heading(const bool hit) { isHeading = hit; }
	};

	class InputMove : public Component
	{
	private:
		Position * pos = nullptr;
		Direction* pDir = nullptr;
		Velocity* vel = nullptr;
		Vec2 pre;
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
			pre = pos->val;
			isMove = false;
			if (!isStop)
			{
				if (Input::Get().GetKey(KEY_INPUT_RIGHT) >= 1)
				{
					pos->val.x += vel->val.x;
					pDir->val = Direction::Dir::R;
					isMove = true;
				}
				if (Input::Get().GetKey(KEY_INPUT_LEFT) >= 1)
				{
					pos->val.x -= vel->val.x;
					pDir->val = Direction::Dir::L;
					isMove = true;
				}
			}
		}
		//移動前の座標取得
		const Vec2& GetPrePos() const { return pre; }
		//動いたか
		const bool IsMoved() const { return isMove; }
		//入力を無効にする
		void Stop() { isStop = true; }
		//入力を許可する
		void GoMove() { isStop = false; }
	};

	class InputAttack : public Component
	{
	private:
		bool isAttack = false;
		bool isStop = false;
	public:
		void UpDate() override
		{
			if (!isStop && Input::Get().GetKey(KEY_INPUT_Z) == 1)
			{

				isAttack = true;
			}

		}
		void EndAttack()
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
			return Input::Get().GetKey(KEY_INPUT_Z) == 1 && !isAttack;
		}
		//入力を無効にする
		void Stop()
		{
			isStop = true;
		}
		//入力を許可する
		void GoMove()
		{
			isStop = false;
		}
		bool IsStop() const
		{
			return isStop;
		}
	};

	class PlayerAnimation : public Component
	{
	private:
		AnimationState* state;
		InputAttack* inputAttack;
		InputMove* inputMove;
		InputJump* inputJump;
		AnimationDraw* anim;

		//Animation用カウンタ
		Counter_f idolCnt;
		Counter_f runningCnt;
		Counter_f attackingCnt;
		Counter_f jumpCnt;
		Counter_f fallCnt;
		Counter_f jumpAttackingCnt;
		void AnimSelect()
		{

			if (inputJump->IsLanding() && inputMove->IsMoved())
			{
				state->val = AnimationState::State::Run;
				//DrawFormatString(300, 40, 0xffffffffu, "Run");
			}
			else if (!inputJump->IsLanding() &&
				inputJump->IsJumping() &&
				!inputAttack->IsAttacking())
			{
				state->val = AnimationState::State::Jump;
				//DrawFormatString(300, 60, 0xffffffffu, "Jump");
			}
			else if (!inputJump->IsLanding() && inputJump->IsFalling())
			{
				state->val = AnimationState::State::Fall;
				//DrawFormatString(300, 60, 0xffffffffu, "Fall");
			}
			else
			{
				state->val = AnimationState::State::Idol;
				//if (!inputAttack->IsAttacking())
				//{
				//	DrawFormatString(300, 80, 0xffffffffu, "Idol");
				//}
			}
			if (state->val != AnimationState::State::Jump &&
				state->val != AnimationState::State::Fall &&
				inputAttack->IsAttacking())
			{
				state->val = AnimationState::State::Attack;
				//DrawFormatString(300, 20, 0xffffffffu, "Attack");
			}
		}
	public:
		void Initialize() override
		{
			inputMove = &entity->GetComponent<InputMove>();
			inputJump = &entity->GetComponent<InputJump>();
			anim = &entity->GetComponent<AnimationDraw>();
			state = &entity->GetComponent<AnimationState>();
			inputAttack = &entity->GetComponent<InputAttack>();
			idolCnt.SetCounter(1.f, 0.15f, 1.f, 5.f);
			runningCnt.SetCounter(6.f, 0.15f, 6.f, 11.f);
			attackingCnt.SetCounter(24.f, 0.2f, 24.f, 29.f);
			jumpCnt.SetCounter(18.f, 0.1f, 18.f, 20.f);
			fallCnt.SetCounter(21.f, 0.05f, 21.f, 22.f);
			jumpAttackingCnt.SetCounter(36.f, 0.2f, 36.f, 41.f);
		}
		void UpDate() override
		{
			AnimSelect();
			switch (state->val)
			{
			case AnimationState::State::Idol:
				fallCnt.SetCounter(21.f, 0.05f, 21.f, 22.f);
				anim->SetIndex(static_cast<int>(++idolCnt));
				inputAttack->GoMove();
				break;
			case AnimationState::State::Jump:
				fallCnt.SetCounter(21.f, 0.05f, 21.f, 22.f);
				jumpCnt.Add();
				anim->SetIndex(static_cast<int>(jumpCnt.GetCurrentCount()));
				inputAttack->Stop();
				break;
			case AnimationState::State::Fall:
				jumpCnt.SetCounter(18.f, 0.1f, 18.f, 20.f);
				fallCnt.Add();
				anim->SetIndex(static_cast<int>(fallCnt.GetCurrentCount()));
				inputAttack->Stop();
				break;
			case AnimationState::State::Run:
				fallCnt.SetCounter(21.f, 0.05f, 21.f, 22.f);
				anim->SetIndex(static_cast<int>(++runningCnt));
				inputAttack->GoMove();
				break;
			case AnimationState::State::Attack:
				inputMove->Stop();
				inputJump->Stop();
				anim->SetIndex(static_cast<int>(++attackingCnt));
				if (attackingCnt.IsMax())
				{
					attackingCnt.Reset();
					attackingCnt.SetCounter(24.f, 0.2f, 24.f, 29.f);
					inputAttack->EndAttack();
					inputMove->GoMove();
					inputJump->GoMove();
				}
				break;
			}
		}
	};
}
