#pragma once
#include "Observer.hpp"
#include "../ECS/ECS.hpp"
#include "EventList.hpp"
#include "../Components/Components.hpp"
#include "../GameController/Test/Game.h"
#include "../ArcheType/ArcheType.hpp"
#include "../GameController/Test/Game.h"
#include "../Collision/Collision.hpp"
namespace Event
{
	class LandingNotify : public Subject<LandingNotify>
	{
	public:
		//doSomething()�Œʒm
		void doSomething() {
			const auto& p = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Player);
			const auto& m = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Map);
			for (const auto& player : p)
			{
				for (const auto& map : m)
				{
					if (Collision::BoxAndBox<ECS::FootBase,ECS::HitBase>(*player, *map))
					{
						Call(Message::Landing);
					}
					else
					{
						player->GetComponent<ECS::InputJump>().Landing(false);
					}
				}
			}
		}
	};

	class PlayerLanding : public Observer<LandingNotify, Message::Landing>
	{
	private:
		LandingNotify * pTest;
	public:
		PlayerLanding() = default;

		void operator()()
		{
			pTest = new LandingNotify();
			pTest->AddObserver(this);
			pTest->doSomething();
		}
		~PlayerLanding()
		{
			Memory::SafeDelete(pTest);
		}
		void UpDate([[maybe_unused]]LandingNotify* sender,
			[[maybe_unused]]const std::string& key_) override
		{
			const auto& p = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Player);
			for (const auto& player : p)
			{
				//player->GetComponent<ECS::Position>().val = player->GetComponent<ECS::InputJump>().GetPrePos();
				player->GetComponent<ECS::InputJump>().Landing(true);
			}
		}
	};
}