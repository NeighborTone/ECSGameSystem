#pragma once
#include "../GameSrc/ECS/ECS.hpp"
#include "../GameSrc/Components/Transform.hpp"
#include "../GameController/Test/Game.h"
namespace ECS
{
	class PlayerArcheType : public ECS::IArcheType<float, float, const char*>
	{
	public:
		ECS::Entity* operator()(const float x, const float y, const char* name) override
		{
			auto& entity(ECS::EcsSystem::GetManager().AddEntity());
			entity.AddComponent<ECS::Transform>().SetPosition(x, y);
			entity.GetComponent<ECS::Transform>().SetVelocity(5.f, 5.f);
			entity.AddComponent<ECS::Direction>();
			entity.AddComponent<ECS::InputMove>();
			entity.AddComponent<ECS::InputAttack>();
			entity.AddComponent<ECS::HitBase>(64.f, 64.f).SetColor(0, 255, 0);
			entity.AddComponent<ECS::AnimationDraw>(name);
			entity.AddComponent<ECS::PlayerAnimation>();
			entity.AddGroup(Game::GameGroup::Player);
			return &entity;
		}
	};
	//$Test$
	//緑色の箱の原型(ArcheType)を作る
	class GreenBoxArcheType : public ECS::IArcheType<>
	{
	public:
		ECS::Entity* operator()() override
		{
			auto& entity(ECS::EcsSystem::GetManager().AddEntity());
			entity.AddComponent<ECS::Transform>().SetPosition(100.f, 100.f);
			entity.AddComponent<ECS::HitBase>(20.f, 20.f).SetColor(0, 255, 0);
			entity.AddGroup(Game::GameGroup::Box);
			return &entity;
		}
	};

	//$Test$
	//青色の箱の原型(ArcheType)を作る。また、引数にfloat変数を2つ受け取れる
	class BlueBoxArcheType : public ECS::IArcheType<float, float>
	{
	public:
		ECS::Entity* operator()(const float x, const float y) override
		{
			auto& entity(ECS::EcsSystem::GetManager().AddEntity());
			entity.AddComponent<ECS::Transform>().SetPosition(x, y);
			entity.AddComponent<ECS::HitBase>(64.f, 64.f).SetColor(0, 0, 255);
			entity.GetComponent<ECS::HitBase>().FillEnable();
			entity.AddGroup(Game::GameGroup::Box);
			return &entity;
		}
	};

	//$Test$
	//引数に指定した色の箱の原型(ArcheType)を作る。
	class ColorBoxArcheType : public ECS::IArcheType<int, int, int>
	{
	public:
		ECS::Entity* operator()(const int r, const int g, int b) override
		{
			auto& entity(ECS::EcsSystem::GetManager().AddEntity());
			entity.AddComponent<ECS::Transform>().SetPosition(300.f, 300.f);
			entity.AddComponent<ECS::HitBase>(20.f, 20.f).SetColor(r, g, b);
			entity.AddGroup(Game::GameGroup::Box);
			return &entity;
		}
	};
	class AttackCollisionBoxArcheType : public ECS::IArcheType<const Vec2&>
	{
	public:
		ECS::Entity* operator()(const Vec2& v) override
		{
			auto& entity(ECS::EcsSystem::GetManager().AddEntityAddTag("PlayerAttack"));
			entity.AddComponent<ECS::Transform>().SetPosition(v.x, v.y);
			entity.AddComponent<ECS::HitBase>(20.f, 60.f).SetColor(255, 0, 0);
			entity.AddComponent<KillEntity>(25);
			entity.AddGroup(Game::GameGroup::PlayerAttackCollision);
			return &entity;
		}
	};

}