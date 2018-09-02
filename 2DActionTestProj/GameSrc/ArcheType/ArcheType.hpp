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
			auto& entity(ECS::EcsSystem::GetManager().AddEntityAddTag("Player"));
			entity.AddComponent<ECS::Transform>().SetPosition(x, y);
			entity.AddComponent<ECS::Physics>().SetVelocity(5.f, 5.f);
			entity.GetComponent<ECS::Physics>().SetGravity(9.8f / 60 / 60);
			entity.AddComponent<ECS::Direction>();
			entity.AddComponent<ECS::InputMove>();
			entity.AddComponent<ECS::InputJump>(-10.f);
			entity.AddComponent<ECS::InputAttack>();
			entity.AddComponent<ECS::HitBase>(52.f, 50.f).SetColor(255, 255, 0);
			entity.GetComponent<ECS::HitBase>().SetOffset(7.f, 10.f);
			entity.GetComponent<ECS::HitBase>().DrawDisable();
			entity.AddComponent<ECS::FootBase>(32.f, 1.f).SetColor(255, 0, 255);
			entity.GetComponent<ECS::FootBase>().SetOffset(18.f, 63.f);
			entity.GetComponent<ECS::FootBase>().DrawDisable();
			entity.AddComponent<ECS::HeadBase>(32.f, 1.f).SetColor(255, 0, 255);
			entity.GetComponent<ECS::HeadBase>().SetOffset(18.f, 10.f);
			entity.GetComponent<ECS::HeadBase>().DrawDisable();
			entity.AddComponent<ECS::SideBase>(1.f, 40.f).SetColor(255, 0, 255);
			entity.GetComponent<ECS::SideBase>().SetOffset(7.f, 15.f);
			entity.GetComponent<ECS::SideBase>().DrawDisable();
			entity.AddComponent<ECS::AnimationState>(ECS::AnimationState::Idol);
			entity.AddComponent<ECS::AnimationDraw>(name);
			entity.AddComponent<ECS::PlayerAnimation>();
			entity.AddGroup(ENTITY_GROUP::Player);
			return &entity;
		}
	};
	//$Test$
	//óŒêFÇÃî†ÇÃå¥å^(ArcheType)ÇçÏÇÈ
	class GreenBoxArcheType : public ECS::IArcheType<float, float, float, float>
	{
	public:
		ECS::Entity* operator()(const float x, const float y, const float w, const float h) override
		{
			auto& entity(ECS::EcsSystem::GetManager().AddEntityAddTag("GreenMap"));
			entity.AddComponent<ECS::Transform>().SetPosition(x, y);
			entity.AddComponent<ECS::HitBase>(w, h).SetColor(10, 128, 20);
			entity.GetComponent<ECS::HitBase>().FillEnable();
			entity.AddGroup(ENTITY_GROUP::Map);
			return &entity;
		}
	};

	//$Test$
	//ê¬êFÇÃî†ÇÃå¥å^(ArcheType)ÇçÏÇÈÅB
	class BlueBoxArcheType : public ECS::IArcheType<float, float>
	{
	public:
		ECS::Entity* operator()(const float x, const float y) override
		{
			auto& entity(ECS::EcsSystem::GetManager().AddEntityAddTag("BlueEnemy"));
			entity.AddComponent<ECS::Transform>().SetPosition(x, y);
			entity.AddComponent<ECS::HitBase>(64.f, 64.f).SetColor(0, 0, 255);
			//entity.GetComponent<ECS::HitBase>().FillEnable();
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
	class BackArcheType : public ECS::IArcheType<const char*>
	{
	public:
		ECS::Entity* operator()(const char* name) override
		{
			auto& entity(ECS::EcsSystem::GetManager().AddEntityAddTag("back"));
			entity.AddComponent<ECS::Position>();
			entity.AddComponent<ECS::SimpleDraw>(name);
			entity.AddGroup(ENTITY_GROUP::Back);
			return &entity;
		}
	};
	class MapArcheType : public ECS::IArcheType<const char*,float,float,int,int,int,int>
	{
	public:
		ECS::Entity* operator()(const char* name,const float x, const float y, const int srcX, const int srcY,const int w, const int h) override
		{
			auto& entity(ECS::EcsSystem::GetManager().AddEntityAddTag("Map"));
			entity.AddComponent<ECS::Position>(x,y);
			entity.AddComponent<ECS::RectDraw>(name, srcX, srcY, w, h);
			entity.AddComponent<ECS::HitBase>(static_cast<float>(w), static_cast<float>(h)).DrawDisable();
			entity.AddGroup(ENTITY_GROUP::Map);
			return &entity;
		}
	};
	class TileMapArcheType : public ECS::IArcheType<const char*, float, float,int>
	{
	public:
		ECS::Entity* operator()(const char* name, const float x, const float y,const int num) override
		{
			ECS::Entity* entity = nullptr;
			switch (num)
			{
			case-1:

				break;
			case 54:
				entity = &ECS::EcsSystem::GetManager().AddEntityAddTag("Map");
				entity->AddComponent<ECS::Position>(x, y);
				entity->AddComponent<ECS::RectDraw>(name, 384, 384, 64, 64);
				entity->AddComponent<ECS::HitBase>(static_cast<float>(64), static_cast<float>(64)).DrawDisable();
				entity->AddGroup(ENTITY_GROUP::Map);
				break;
			}
			
			
			return entity;
		}
	};
	class MapBackArcheType : public ECS::IArcheType<const char*, float, float, int, int, int, int>
	{
	public:
		ECS::Entity* operator()(const char* name, const float x, const float y, const int srcX, const int srcY, const int w, const int h) override
		{
			auto& entity(ECS::EcsSystem::GetManager().AddEntityAddTag("Map"));
			entity.AddComponent<ECS::Position>(x, y);
			entity.AddComponent<ECS::RectDraw>(name, srcX, srcY, w, h);
			entity.AddGroup(ENTITY_GROUP::Map);
			return &entity;
		}
	};
}