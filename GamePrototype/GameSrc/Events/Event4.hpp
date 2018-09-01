#pragma once
#include "Observer.hpp"
#include "../ECS/ECS.hpp"
#include "EventList.hpp"
#include "../Components/Components.hpp"
#include "../GameController/Test/Game.h"
#include "../ArcheType/ArcheType.hpp"
#include "../GameController/Test/Game.h"
#include "../Collision/Collision.hpp"
//主にプレーヤーとマップの衝突応答の処理
namespace Event
{
	class LandingNotify : public Subject<LandingNotify>
	{
	public:
		//doSomething()で通知
		void doSomething() {
			const auto& p = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Player);
			const auto& m = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Map);
			for (const auto& player : p)
			{
				for (const auto& map : m)
				{
					if (Collision::BoxAndBox<ECS::FootBase, ECS::HitBase>(*player, *map))
					{
						Call(Message::Landing);
						//いい感じにめり込まないよう強引に調整(-10は適当)
						player->GetComponent<ECS::Position>().val.y =
							map->GetComponent<ECS::Position>().val.y - player->GetComponent<ECS::HitBase>().h() - 10;
						break;
					}
					else
					{
						player->GetComponent<ECS::InputJump>().Landing(false);
					}
				}
			}
		}
	};

	class PlayerLandingCheke : public Observer<LandingNotify, Message::Landing>
	{
	private:
		LandingNotify * pTest;
	public:
		PlayerLandingCheke() = default;

		void operator()()
		{
			pTest = new LandingNotify();
			pTest->AddObserver(this);
			pTest->doSomething();
		}
		~PlayerLandingCheke()
		{
			Memory::SafeDelete(pTest);
		}
		void UpDate([[maybe_unused]]LandingNotify* sender,
			[[maybe_unused]]const std::string& key_) override
		{
			const auto& p = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Player);
			for (const auto& player : p)
			{
				//DOUT << Message::Landing << std::endl;
				player->GetComponent<ECS::InputJump>().Landing(true);
			}
		}
	};

	class HeadNotify : public Subject<HeadNotify>
	{
	public:
		//doSomething()で通知
		void doSomething() {
			const auto& p = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Player);
			const auto& m = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Map);
			for (const auto& player : p)
			{
				for (const auto& map : m)
				{
					if (Collision::BoxAndBox<ECS::HeadBase, ECS::HitBase>(*player, *map))
					{
						if (player->GetComponent<ECS::InputJump>().IsJumping())
						{
							Call(Message::Heading);
							break;
						}
					}
					else
					{
						player->GetComponent<ECS::InputJump>().Heading(false);
					}
				}
			}
		}
	};

	class PlayerHeadCheck : public Observer<HeadNotify, Message::Heading>
	{
	private:
		HeadNotify * pTest;
	public:
		PlayerHeadCheck() = default;

		void operator()()
		{
			pTest = new HeadNotify();
			pTest->AddObserver(this);
			pTest->doSomething();
		}
		~PlayerHeadCheck()
		{
			Memory::SafeDelete(pTest);
		}
		void UpDate([[maybe_unused]]HeadNotify* sender,
			[[maybe_unused]]const std::string& key_) override
		{
			const auto& p = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Player);
			for (const auto& player : p)
			{
				DOUT << Message::Heading << std::endl;
				player->GetComponent<ECS::InputJump>().Heading(true);
			}
		}
	};

	class SideNotify : public Subject<SideNotify>
	{
	public:
		//doSomething()で通知
		void doSomething() {
			const auto& p = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Player);
			const auto& m = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Map);
			for (const auto& player : p)
			{
				for (const auto& map : m)
				{
					if (Collision::BoxAndBox<ECS::SideBase, ECS::HitBase>(*player, *map))
					{
						Call(Message::SideHit);
						break;
					}
					else
					{
						player->GetComponent<ECS::Velocity>().val.x = 5.f;
					}
				}
			}
		}
	};

	class PlayerSideCheck : public Observer<SideNotify, Message::SideHit>
	{
	private:
		SideNotify * pTest;
	public:
		PlayerSideCheck() = default;

		void operator()()
		{
			pTest = new SideNotify();
			pTest->AddObserver(this);
			pTest->doSomething();
		}
		~PlayerSideCheck()
		{
			Memory::SafeDelete(pTest);
		}
		void UpDate([[maybe_unused]]SideNotify* sender,
			[[maybe_unused]]const std::string& key_) override
		{
			const auto& p = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Player);
			for (const auto& player : p)
			{
				DOUT << Message::SideHit << std::endl;

				player->GetComponent<ECS::Velocity>().val.x = 0;
			}
		}
	};
}