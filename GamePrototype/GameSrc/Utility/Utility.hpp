#pragma once
#include <Windows.h>
#include <iostream>

namespace Memory
{
	template<class T>void SafeDelete(T& t)
	{
		if (t)
		{
			delete t;
			t = nullptr;
		}
	}
	template<class T>void SafeDeleteArray(T& t)
	{
		if (t)
		{
			delete[] t;
			t = nullptr;
		}
	}
	template<class T>void SafeRelease(T& t)
	{
		if (t)
		{
			t->Release();
			t = nullptr;
		}
	}
}

class WinMessageBox
{
public:
	static bool IsFullScreen()
	{
		int flag;
		flag = MessageBox(
			NULL,
			TEXT("フルスクリーンモードで起動しますか？"),
			TEXT("スクリーン設定"),
			MB_YESNO | MB_ICONQUESTION);
		if (flag == IDNO)
		{
			return true;
		}
		return false;
	}
};
//デバッグビルドのみ有効にする
#ifdef _DEBUG
#define DOUT std::cout
#else 
#define DOUT 0 && std::cout
#endif