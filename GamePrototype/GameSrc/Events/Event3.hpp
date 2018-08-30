#pragma once
#include "Observer.hpp"
#include "../ECS/ECS.hpp"
#include "EventList.hpp"
#include "../Components/Components.hpp"
#include "../GameController/Test/Game.h"
#include "../ArcheType/ArcheType.hpp"
#include "../GameController/Test/Game.h"
namespace Event
{
	class AttackNotify : public Subject<AttackNotify>
	{
	public:
		//doSomething()Ç≈í ím
		void doSomething() {
			if (Game::GetScene().Current() == Game::Scene::Play)
			{
				const auto& player = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Player);
				for (const auto& it : player)
				{
					if (it->HasComponent<ECS::InputAttack>())
					{
						if (it->GetComponent<ECS::InputAttack>().IsAttacked() && !it->GetComponent<ECS::InputAttack>().IsStop())
						{
							//é¿ç€Ç…ÇµÇƒÇŸÇµÇ¢èàóù(Observer)Ç…í ímÇëóÇÈ
							Call(Message::PlayerAttack);
						}
					}
				}
			}
		}
	};

	class CreateAttackCollision : public Observer<AttackNotify, Message::PlayerAttack>
	{
	private:
		AttackNotify * pTest;
	public:
		CreateAttackCollision() = default;

		void operator()()
		{
			pTest = new AttackNotify();
			pTest->AddObserver(this);
			pTest->doSomething();
		}
		~CreateAttackCollision()
		{
			Memory::SafeDelete(pTest);
		}
		void UpDate([[maybe_unused]]AttackNotify* sender,
			[[maybe_unused]]const std::string& key_) override
		{
			const auto& player = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Player);
			for (const auto& it : player)
			{
				if (it->GetComponent<ECS::Direction>().val == ECS::Direction::Dir::R)
				{
					DOUT << Message::PlayerAttack << std::endl;
					ECS::AttackCollisionBoxArcheType()(it->GetComponent<ECS::Position>().val.OffSetCopy(64.f, 0));
				}
				if (it->GetComponent<ECS::Direction>().val == ECS::Direction::Dir::L)
				{
					DOUT << Message::PlayerAttack << std::endl;
					ECS::AttackCollisionBoxArcheType()(it->GetComponent<ECS::Position>().val.OffSetCopy(-20.f, 0));
				}
			}
		}
	};

}
