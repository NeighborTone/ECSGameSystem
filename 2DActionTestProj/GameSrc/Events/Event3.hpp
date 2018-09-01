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
		//doSomething()‚Å’Ê’m
		void doSomething() {
			if (Game::GetScene().Current() == Game::Scene::Play)
			{
				const auto& player = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Player);
				const auto& atk = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::PlayerAttackCollision);
				for (const auto& it : player)
				{
					if (it->HasComponent<ECS::InputAttack>())
					{
						if (it->GetComponent<ECS::InputAttack>().IsAttacked() && !it->GetComponent<ECS::InputAttack>().IsStop())
						{
							//ŽÀÛ‚É‚µ‚Ä‚Ù‚µ‚¢ˆ—(Observer)‚É’Ê’m‚ð‘—‚é
							Call(Message::PlayerAttack);
						}
					}
				}
				if (atk.empty())
				{
					return;
				}
				for (const auto& it_p : player)
				{
					for (auto& it_a : atk)
					{
						if (it_p->GetComponent<ECS::Direction>().val == ECS::Direction::Dir::R)
						{
							it_a->GetComponent<ECS::Position>().val =
								it_p->GetComponent<ECS::Position>().val.OffSetCopy(64.f, 0);

						}
						if (it_p->GetComponent<ECS::Direction>().val == ECS::Direction::Dir::L)
						{
							it_a->GetComponent<ECS::Position>().val = 
								it_p->GetComponent<ECS::Position>().val.OffSetCopy(-20.f, 0);

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
			DOUT << Message::PlayerAttack << std::endl;
			PlaySoundMem(ResourceManager::GetSound().GetHandle("atkSE"), DX_PLAYTYPE_BACK);
			ECS::AttackCollisionBoxArcheType()(Vec2(0,0));
		}
	};
}
