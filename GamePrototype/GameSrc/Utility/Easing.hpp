#pragma once
#include <cmath>
#include "Counter.hpp"
typedef float(*Ease)(float, float);

class Easing
{
private:
	Counter_f time;
	float vol;
	static constexpr float PI = 3.141592653589793f;
public:
	//コンストラクタ
	Easing() :
		time(1,1),
		vol(0)
	{}

	//イージングの実行
	//引数：イージング動作の関数ポインタ, 継続時間(float)
	void Run(const Ease em, const float durationTime)
	{
		time.SetEndTime(durationTime);
		time.Add();
		vol = em(time.GetCurrentCount(), durationTime);
	}

	//値を取得
	//引数：始点(float), 終点(float)-始点(float)
	const float GetVolume(const float startPoint, const float endPoint)
	{
		return startPoint + (vol * endPoint);
	}

	//イージングが終了したらtrueが返る
	const bool IsEaseEnd()
	{
		return time.IsMax();
	}

	//イージングをリセットする
	void Reset()
	{
		time.Reset();
	}

	static float LinearIn(float time, float duration)
	{
		return time / duration;
	}
	static float LinearOut(float time, float duration)
	{
		return time / duration;
	}
	static float LinearInOut(float time, float duration)
	{
		return time / duration;
	}

	static float BackIn(float time, float duration)
	{
		float s = 1.70158f;
		float postFix = time /= duration;
		return (postFix)*time*((s + 1)*time - s);
	}
	static float BackOut(float time, float duration)
	{
		float s = 1.70158f;
		return ((time = time / duration - 1)*time*((s + 1)*time + s) + 1);
	}
	static float BackInOut(float time, float duration)
	{
		float s = 1.70158f;
		if ((time /= duration / 2) < 1) return 1.f / 2.f * (time*time*(((s *= (1.525f)) + 1)*time - s));
		float postFix = time -= 2;
		return 1.f / 2.f * ((postFix)*time*(((s *= (1.525f)) + 1)*time + s) + 2);
	}

	static float BounceIn(float time, float duration)
	{
		return 1.f - BounceOut(duration - time, duration);
	}
	static float BounceOut(float time, float duration)
	{
		if ((time /= duration) < (1.f / 2.75f))
		{
			return 7.5625f*time*time;
		}
		else if (time < (2.f / 2.75f))
		{
			float postFix = time -= (1.5f / 2.75f);
			return 7.5625f*(postFix)*time + .75f;
		}
		else if (time < (2.5 / 2.75))
		{
			float postFix = time -= (2.25f / 2.75f);
			return 7.5625f*(postFix)*time + .9375f;
		}
		else
		{
			float postFix = time -= (2.625f / 2.75f);
			return 7.5625f*(postFix)*time + .984375f;
		}
	}
	static float BounceInOut(float time, float duration)
	{
		if (time < duration / 2) return BounceIn(time * 2, duration) * 0.5f;
		else return BounceOut(time * 2 - duration, duration) * 0.5f + 0.5f;
	}

	static float CircIn(float time, float duration)
	{
		return -1.f * static_cast<float>((sqrt(1 - (time /= duration)*time) - 1));
	}
	static float CircOut(float time, float duration)
	{
		return static_cast<float>(sqrt(1 - (time = time / duration - 1)*time));
	}
	static float CircInOut(float time, float duration)
	{
		if ((time /= duration / 2) < 1) return -1.f / 2.f * static_cast<float>((sqrt(1 - time * time) - 1));
		return 1.f / 2.f * static_cast<float>((sqrt(1 - time * (time -= 2)) + 1));
	}

	static float CubicIn(float time, float duration)
	{
		return (time /= duration)*time*time;
	}
	static float CubicOut(float time, float duration)
	{
		return (time = time / duration - 1)*time*time + 1;
	}
	static float CubicInOut(float time, float duration)
	{
		if ((time /= duration / 2) < 1) return 1.f / 2.f * time*time*time;
		return 1.f / 2.f * ((time -= 2)*time*time + 2);
	}

	static float ElasticIn(float time, float duration)
	{
		if (time == 0) return 0.f;  if ((time /= duration) == 1) return 1.f;
		float p = duration * 0.3f;
		float a = 1.f;
		float s = p / 4;
		float postFix = static_cast<float>(a*pow(2, 10 * (time -= 1)));
		return -static_cast<float>(postFix * sin((time*(duration)-s)*(2 * (PI)) / p));
	}
	static float ElasticOut(float time, float duration)
	{
		if (time == 0) return 0.f;  if ((time /= duration) == 1) return 1.f;
		float p = duration * 0.3f;
		float a = 1.f;
		float s = p / 4;
		return (static_cast<float>(a*pow(2, -10 * time) * sin((time*(duration)-s)*(2 * static_cast<float>(PI)) / p) + 1.f));
	}
	static float ElasticInOut(float time, float duration)
	{
		if (time == 0) return 0.f;  if ((time /= duration / 2) == 2) return 1.f;
		float p = duration * (0.3f*1.5f);
		float a = 1.f;
		float s = p / 4;

		if (time < 1) {
			float postFix = static_cast<float>(a*pow(2, 10 * (time -= 1)));
			return -0.5f*static_cast<float>((postFix* sin((time*(duration)-s)*(2 * (PI)) / p)));
		}
		float postFix = static_cast<float>(a*pow(2, -10 * (time -= 1)));
		return static_cast<float>(postFix * sin((time*(duration)-s)*(2 * static_cast<float>(PI)) / p)*.5f + 1.f);
	}

	static float ExpoIn(float time, float duration)
	{
		return (time == 0) ? 0.f : static_cast<float>(pow(2, 10 * (time / duration - 1)));
	}
	static float ExpoOut(float time, float duration)
	{
		return (time == duration) ? 1.f : static_cast<float>(-pow(2, -10 * time / duration) + 1);
	}
	static float ExpoInOut(float time, float duration)
	{
		if (time == 0) return 0.f;
		if (time == duration) return 1.f;
		if ((time /= duration / 2) < 1) return 1.f / 2.f * static_cast<float>(pow(2, 10 * (time - 1)));
		return 1.f / 2.f * (static_cast<float>(-pow(2, -10 * --time) + 2));
	}

	static float QuadIn(float time, float duration)
	{
		return (time /= duration)*time;
	}
	static float QuadOut(float time, float duration)
	{
		return -1.f * (time /= duration)*(time - 2);
	}
	static float QuadInOut(float time, float duration)
	{
		if ((time /= duration / 2) < 1) return ((1.f / 2.f)*(time*time));
		return -1.f / 2.f * (((time - 2)*(--time)) - 1);
	}

	static float QuartIn(float time, float duration)
	{
		return (time /= duration)*time*time*time;
	}
	static float QuartOut(float time, float duration)
	{
		return -1.f * ((time = time / duration - 1)*time*time*time - 1);
	}
	static float QuartInOut(float time, float duration)
	{
		if ((time /= duration / 2) < 1) return 1.f / 2.f * time*time*time*time;
		return -1.f / 2.f * ((time -= 2)*time*time*time - 2);
	}

	static float QuintIn(float time, float duration)
	{
		return (time /= duration)*time*time*time*time;
	}
	static float QuintOut(float time, float duration)
	{
		return (time = time / duration - 1)*time*time*time*time + 1;
	}
	static float QuintInOut(float time, float duration)
	{
		if ((time /= duration / 2) < 1) return 1.f / 2.f * time*time*time*time*time;
		return 1.f / 2.f * ((time -= 2)*time*time*time*time + 2);
	}

	static float SineIn(float time, float duration)
	{
		return -1.f * static_cast<float>(cos(time / duration * (PI / 2))) + 1.f;
	}
	static float SineOut(float time, float duration)
	{
		return static_cast<float>(sin(time / duration * (PI / 2)));
	}
	static float SineInOut(float time, float duration)
	{
		return -1.f / 2.f * static_cast<float>((cos(PI*time / duration) - 1));
	}
};

