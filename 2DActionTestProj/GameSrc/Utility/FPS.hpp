#pragma once
#include <atlbase.h>
/*! @class FPS
*   @brief ���Ԃ��v�����邽�߂̃N���X�ł�
*/
class FPS
{
private:
	float time;						//���ݎ���
	float deltaTime;				//����
	int frameRate;					//���݂̃t���[���̒l
	float second;					//�b���𑪂�
	float frameCount;				//�t���[���J�E���^
	float startTime;				//����J�n����
	int frameCont2;					//�Œ�p�J�E���^
	float fps;						//fps
	static constexpr int AVG = 60;	//60Hz
	static constexpr int Rate = 60;	//�ݒ肷��FPS
	LARGE_INTEGER preCount;			//�����t���l�^ long long
	LARGE_INTEGER frequency;
	LARGE_INTEGER GetCounter() const
	{
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);
		return counter;
	}

	LARGE_INTEGER GetCountFrequency() const
	{
		LARGE_INTEGER frequency_;
		QueryPerformanceFrequency(&frequency_);
		return frequency_;
	}
public:


	FPS() :
		frameCount(0),
		frameRate(0),
		time(0),
		deltaTime(0),
		second(0)
	{

		preCount = GetCounter();
		frequency = GetCountFrequency();
	}

	~FPS()
	{

	}

	void UpDate()
	{
		if (frameCont2 == 0)
		{
			startTime = GetTime();
		}
		if (frameCont2 == AVG)
		{
			float t = GetTime();
			fps = 1000.f / ((t - startTime) / AVG);
			frameCont2 = 0;
			startTime = t;
		}
		++frameCont2;

		LARGE_INTEGER count = GetCounter();
		deltaTime = static_cast<float>(count.QuadPart - preCount.QuadPart) / frequency.QuadPart;
		preCount = GetCounter();

		time += deltaTime;

		++frameCount;
		second += deltaTime;
		if (second >= 1.0f)
		{
			frameRate = static_cast<int>(frameCount);
			frameCount = 0;
			second -= 1.0f;
		}



	}

	void Wait()
	{
		int tookTime = static_cast<int>(GetTime() - startTime);	//������������
		int waitTime = frameCont2 * 1000 / Rate - tookTime;	//�҂ׂ�����
		if (waitTime > 0)
		{
			Sleep(waitTime);
		}
	}

	float GetTime() const
	{
		return time;
	}

	float GetDeltaTime() const
	{
		return deltaTime;
	}

	int GetFrameRate() const
	{
		return frameRate;
	}

};
