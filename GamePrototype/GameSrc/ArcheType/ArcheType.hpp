#pragma once
#include "../GameSrc/ECS/ECS.hpp"
#include "../GameSrc/Components/Components.hpp"
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
			entity.AddComponent<ECS::Physics>().SetVelocity(5.f, 5.f);
			entity.GetComponent<ECS::Gravity>().val = 9.8f / 60 / 60;
			entity.AddComponent<ECS::Direction>();
			entity.AddComponent<ECS::InputMove>();
			entity.AddComponent<ECS::InputJump>(-10.f, -3.f);
			entity.AddComponent<ECS::InputAttack>();
			entity.AddComponent<ECS::HitBase>(52.f, 50.f).SetColor(255, 255, 0);
			entity.GetComponent<ECS::HitBase>().SetOffset(7.f, 10.f);
			//entity.GetComponent<ECS::HitBase>().DrawDisable();
			entity.AddComponent<ECS::FootBase>(32.f, 1.f).SetColor(255, 0, 255);
			entity.GetComponent<ECS::FootBase>().SetOffset(18.f, 63.f);
			entity.AddComponent<ECS::HeadBase>(32.f, 1.f).SetColor(255, 0, 255);
			entity.GetComponent<ECS::HeadBase>().SetOffset(18.f, 10.f);
			//entity.GetComponent<ECS::FootBase>().DrawDisable();
			entity.AddComponent<ECS::AnimationDraw>(name);
			entity.AddComponent<ECS::PlayerAnimation>();
			entity.AddGroup(ENTITY_GROUP::Player);
			return &entity;
		}
	};
	//$Test$
	//緑色の箱の原型(ArcheType)を作る
	class GreenBoxArcheType : public ECS::IArcheType<float, float, float, float>
	{
	public:
		ECS::Entity* operator()(const float x, const float y, const float w, const float h) override
		{
			auto& entity(ECS::EcsSystem::GetManager().AddEntity());
			entity.AddComponent<ECS::Transform>().SetPosition(x, y);
			entity.AddComponent<ECS::HitBase>(w, h).SetColor(0, 255, 0);
			entity.AddGroup(ENTITY_GROUP::Map);
			return &entity;
		}
	};

	//$Test$
	//青色の箱の原型(ArcheType)を作る。
	class BlueBoxArcheType : public ECS::IArcheType<float, float>
	{
	public:
		ECS::Entity* operator()(const float x, const float y) override
		{
			auto& entity(ECS::EcsSystem::GetManager().AddEntity());
			entity.AddComponent<ECS::Transform>().SetPosition(x, y);
			entity.AddComponent<ECS::HitBase>(64.f, 64.f).SetColor(0, 0, 255);
			entity.GetComponent<ECS::HitBase>().FillEnable();
			entity.AddGroup(ENTITY_GROUP::Enemy);
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
			entity.AddGroup(ENTITY_GROUP::Enemy);
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
			entity.AddComponent<ECS::HitBase>(25.f, 60.f).SetColor(255, 0, 0);
			entity.AddComponent<KillEntity>(25);
			entity.AddGroup(ENTITY_GROUP::PlayerAttackCollision);
			return &entity;
		}
	};

}