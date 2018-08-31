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
		int key[256];	//キーの入力状態を格納する
		//キーの入力状態の更新
		void Update_Key()
		{
			char tmpKey[256];
			GetHitKeyStateAll(tmpKey);	//現在のキーの状態を格納
			for (int i = 0; i < 256; ++i)
			{
				if (tmpKey[i] != 0)
				{
					++key[i];
				}
				else  //押されていなければ
				{
					key[i] = 0;
				}
			}
		}
		//keycodeのキーの入力状態を取得する
		int GetKey(int keycode)
		{
			return key[keycode];	//keycodeの入力状態を返す
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
