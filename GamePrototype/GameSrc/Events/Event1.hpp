#pragma once
#include "Observer.hpp"
#include "../ECS/ECS.hpp"
#include "EventList.hpp"
#include "../Components/Components.hpp"
#include "../Utility/Utility.hpp"
#include "../GameController/Test/Game.h"
#include "../Collision/Collision.hpp"
#include <Windows.h>
namespace Event
{
	class HitNotify : public Subject<HitNotify>
	{
	public:
		ECS::Entity* atkCollision;
		ECS::Entity* defenceCollision;
		//doSomething()で通知
		void doSomething() {
			//実際にしてほしい処理(Observer)に通知を送る
			const auto& b = ECS::EcsSystem().GetManager().GetEntitiesByGroup(ENTITY_GROUP::Enemy);
			const auto& c = ECS::EcsSystem().GetManager().GetEntitiesByGroup(ENTITY_GROUP::PlayerAttackCollision);
			for (const auto& col : c)
			{
				for (const auto& box : b)
				{
					if (Collision::BoxAndBox(*col, *box))
					{
						atkCollision = col;
						defenceCollision = box;
						Call(Message::Hit);
					}
				}
			}
		}
	};

	class HitEvent : public Observer<HitNotify, Message::Hit>
	{
	private:
		HitNotify * pTest;
	public:
		void operator()()
		{
			pTest = new HitNotify();
			pTest->AddObserver(this);
			pTest->doSomething();
		}
		~HitEvent()
		{
			Memory::SafeDelete(pTest);
		}
		void UpDate([[maybe_unused]]HitNotify* sender,
			[[maybe_unused]]const std::string& key_) override
		{
			DOUT << Message::Hit << std::endl;
			//まずコリジョンを消す
			pTest->atkCollision->Destroy();
			pTest->defenceCollision->DeleteComponent<ECS::HitBase>();
			PlaySoundMem(ResourceManager::GetSound().GetHandle("hitSE"), DX_PLAYTYPE_BACK);
			//何らかの死亡演出がある場合こんな感じにする
			pTest->defenceCollision->AddComponent<ECS::KillEntity>(10);  //10フレーム後に死ぬ
		}
	};
}
