#pragma once
#include <DxLib.h>
class Input
{
public:
	inline static int key[256];	//キーの入力状態を格納する
	//キーの入力状態の更新
	static void Update_Key()
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
	static int GetKey(int keycode)
	{
		return key[keycode];	//keycodeの入力状態を返す
	}
};