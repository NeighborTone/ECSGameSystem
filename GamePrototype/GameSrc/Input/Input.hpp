#pragma once
#include "../GameSrc/Utility/Utility.hpp"
#include <DxLib.h>
class Input
{
private:
	class Singleton
	{
	public:
		Singleton() = default;
		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;
	public:
		int key[256];	//�L�[�̓��͏�Ԃ��i�[����
		//�L�[�̓��͏�Ԃ̍X�V
		void Update_Key()
		{
			char tmpKey[256];
			GetHitKeyStateAll(tmpKey);	//���݂̃L�[�̏�Ԃ��i�[
			for (int i = 0; i < 256; ++i)
			{
				if (tmpKey[i] != 0)
				{
					++key[i];
				}
				else  //������Ă��Ȃ����
				{
					key[i] = 0;
				}
			}
		}
		//keycode�̃L�[�̓��͏�Ԃ��擾����
		int GetKey(int keycode)
		{
			return key[keycode];	//keycode�̓��͏�Ԃ�Ԃ�
		}
	};
public:
	inline static Singleton& Get()
	{
		static std::unique_ptr<Singleton> inst =
			std::make_unique<Singleton>();
		return *inst;
	}
};
