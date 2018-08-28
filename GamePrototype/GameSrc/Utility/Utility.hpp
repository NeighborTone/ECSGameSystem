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
			TEXT("�t���X�N���[�����[�h�ŋN�����܂����H"),
			TEXT("�X�N���[���ݒ�"),
			MB_YESNO | MB_ICONQUESTION);
		if (flag == IDNO)
		{
			return true;
		}
		return false;
	}
};
//�f�o�b�O�r���h�̂ݗL���ɂ���
#ifdef _DEBUG
#define DOUT std::cout
#else 
#define DOUT 0 && std::cout
#endif