#pragma once
#include "Observer.hpp"
#include "../ECS/ECS.hpp"
#include "EventList.hpp"
#include "../Components/Transform.hpp"
#include "../Utility/Utility.hpp"
#include <Windows.h>
namespace Event
{
	class WhiteNotify : public Subject<WhiteNotify>
	{
	public:
		ECS::Entity* me;
		explicit WhiteNotify(ECS::Entity& e)
		{
			me = &e;
		}
		//doSomething()Ç≈í ím
		void doSomething() {
			//é¿ç€Ç…ÇµÇƒÇŸÇµÇ¢èàóù(Observer)Ç…í ímÇëóÇÈ
			if (GetAsyncKeyState(VK_SPACE) & 0x8001)
			{
				Call(Message::WhiteBox);
			}
		}
	};

	class AddWhiteBox : public Observer<WhiteNotify, Message::WhiteBox>
	{
	private:
		WhiteNotify * pTest;
	public:
		AddWhiteBox() = default;
		AddWhiteBox(const AddWhiteBox*) = delete;
		AddWhiteBox(AddWhiteBox*) = delete;
		AddWhiteBox(const AddWhiteBox&) = delete;
		AddWhiteBox(const AddWhiteBox&&) = delete;
		void operator()(ECS::Entity& e)
		{
			pTest = new WhiteNotify(e);
			pTest->AddObserver(this);
			pTest->doSomething();
		}
		~AddWhiteBox()
		{
			Memory::SafeDelete(pTest);
		}
		void UpDate([[maybe_unused]]WhiteNotify* sender,
			[[maybe_unused]]const std::string& key_) override
		{
			DOUT << Message::WhiteBox << std::endl;
			if (!pTest->me->HasComponent<ECS::HitBase>())
			{
				pTest->me->AddComponent<ECS::HitBase>(50.f, 50.f);
			}
		}
	};

}
