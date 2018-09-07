#pragma once
#include <atlbase.h>
/*! @class FPS
*   @brief 時間を計測するためのクラスです
*/
class FPS
{
private:
	float time;						//現在時刻
	float deltaTime;				//差分
	int frameRate;					//現在のフレームの値
	float second;					//秒数を測る
	float frameCount;				//フレームカウンタ
	float startTime;				//測定開始時刻
	int frameCont2;					//固定用カウンタ
	float fps;						//fps
	static constexpr int AVG = 60;	//60Hz
	static constexpr int Rate = 60;	//設定するFPS
	LARGE_INTEGER preCount;			//符号付数値型 long long
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
		int tookTime = static_cast<int>(GetTime() - startTime);	//かかった時間
		int waitTime = frameCont2 * 1000 / Rate - tookTime;	//待つべき時間
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
