#pragma once
#include "Observer.hpp"
#include "../ECS/ECS.hpp"
#include "EventList.hpp"
#include "../Components/Transform.hpp"
#include "../Utility/Utility.hpp"
#include <Windows.h>
namespace Event
{
	class RedNotify : public Subject<RedNotify>
	{
	public:
		ECS::Entity* me;
		explicit RedNotify(ECS::Entity& e)
		{
			me = &e;
		}
		//doSomething()�Œʒm
		void doSomething() {
			//���ۂɂ��Ăق�������(Observer)�ɒʒm�𑗂�
			if (GetAsyncKeyState(VK_RETURN) & 0x8001)
			{
				Call(Message::RedBox);
			}
		}
	};

	class AddRedBox : public Observer<RedNotify, Message::RedBox>
	{
	private:
		RedNotify* pTest;
	public:
		void operator()(ECS::Entity& e)
		{
			pTest = new RedNotify(e);
			pTest->AddObserver(this);
			pTest->doSomething();
		}
		~AddRedBox()
		{
			Memory::SafeDelete(pTest);

		}
		void UpDate([[maybe_unused]]RedNotify* sender,
			[[maybe_unused]]const std::string& key_) override
		{
			DOUT << Message::RedBox << std::endl;
			if (!pTest->me->HasComponent<ECS::HitBase>())
			{
				pTest->me->AddComponent<ECS::HitBase>(50.f, 50.f).SetColor(255,0,0);
			}
		}
	};

}
