#pragma once
#include <Windows.h>
#include <iostream>
#include <chrono>

//�f�o�b�O�r���h�̂ݗL���ɂ���
#ifdef _DEBUG
#define DOUT std::cout
#else 
#define DOUT 0 && std::cout
#endif
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
	WinMessageBox(const std::string& text, const char* category)
	{
		MessageBox(NULL, text.c_str(), category, MB_OK);
	}
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

template<class T = std::chrono::milliseconds>
class ProcessingTime
{
private:

	std::string name;
	std::chrono::system_clock::time_point start;
	bool isActive;

public:
	ProcessingTime(const std::string& name = "Process", bool start = true) :
		name(name),
		isActive(start)
	{
		if (start)
		{
			this->Restart();
		}
	}
	~ProcessingTime()
	{
		this->Stop();
	}

	//�v���̃��X�^�[�g
	void Restart()&
	{
		start = std::chrono::system_clock::now();
		isActive = true;
	}
	//�v�����I�����o��
	void Stop()&
	{
		if (!isActive)
			return;
		const auto end = std::chrono::system_clock::now();
		const auto elapsed = std::chrono::duration_cast<T>(end - start).count();
		if (typeid(T) == typeid(std::chrono::nanoseconds))
		{
			DOUT << elapsed << " [nanoseconds]" << std::endl;
			return;
		}
		if (typeid(T) == typeid(std::chrono::microseconds))
		{
			DOUT << elapsed << " [microseconds]" << std::endl;
			return;
		}
		if (typeid(T) == typeid(std::chrono::milliseconds))
		{
			DOUT << elapsed << " [milliseconds]" << std::endl;
			return;
		}
		if (typeid(T) == typeid(std::chrono::seconds))
		{
			DOUT << elapsed << " [seconds]" << std::endl;
			return;
		}
		if (typeid(T) == typeid(std::chrono::minutes))
		{
			DOUT << elapsed << " [minutes]" << std::endl;
			return;
		}
		if (typeid(T) == typeid(std::chrono::hours))
		{
			DOUT << elapsed << " [hours]" << std::endl;
			return;
		}
		isActive = false;
	}

};
