#pragma once
#include <atlbase.h>
#include <memory>
/*! @class FPS
*   @brief FPSを固定するためのクラスです
*/
class FPS
{
private:
	float time;
	float deltaTime;
	int frameRate;
	float second;
	int frameCount;
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

	void UpDate()
	{
		LARGE_INTEGER count = GetCounter();
		deltaTime = static_cast<float>(count.QuadPart - preCount.QuadPart) / frequency.QuadPart;
		preCount = GetCounter();

		time += deltaTime;

		frameCount++;
		second += deltaTime;
		if (second >= 1.0f)
		{
			frameRate = frameCount;
			frameCount = 0;
			second -= 1.0f;
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

