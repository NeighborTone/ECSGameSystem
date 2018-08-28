#pragma once
#pragma warning (disable : 4458)	//thisを使うため消す
template <typename T>
/*! @class Counter
*   @brief カウンターです.上限値に達したら下限値、下限値を超えたら上限値になります
*   @note コンストラクタで初期値、加算値、下限値、上限値を設定してください
*   上限値や下限値を設定していない場合、その型の最大値、最小値が設定されます
*   何も指定しない場合の加算値は1になります
*    int(Counter), float(CounterF), double(CounterD)型がそれぞれあります
*/
class TemplateCounter
{
#undef max
#undef min
private:
	T now;
	T add;
	T min;
	T max;
	bool isMax;
	bool isMin;
public:
	
	TemplateCounter() :
		isMax(false),
		isMin(false),
		now(0),
		add(1),
		min(std::numeric_limits<T>::min()),
		max(std::numeric_limits<T>::max())
	{};
	//!加算値のみ設定
	explicit TemplateCounter(T add) :
		isMax(false),
		isMin(false),
		now(0),
		add(add),
		min(std::numeric_limits<T>::min()),
		max(std::numeric_limits<T>::max())
	{};
	//!下限値と上限値設定
	TemplateCounter(T min, T max) :
		isMax(false),
		isMin(false),
		now(0),
		add(1),
		min(min),
		max(max)
	{};
	//!すべてのパラメータ設定
	TemplateCounter(T now, T add, T min, T max) :
		isMax(false),
		isMin(false)
	{
		this->now = now;
		this->add = add;
		this->min = min;
		this->max = max;
	}
	//!カウンターを初期化します
	void SetCounter(T now, T add, T min, T max) 
	{
		this->now = now;
		this->add = add;
		this->min = min;
		this->max = max;
	}
	/*!
	* @brief カウンターを加算値分足します
	* @return 現在のカウンターの値
	*/
	T operator++() 
	{
		if (now >= max)
		{
			now = min;
		}
		else
		{
			now += add;
		}
		return now;
	}

	/*!
	* @brief カウンターを加算値分引きます
	* @return 現在のカウンターの値
	*/
	T operator--()
	{
		if (now <= min)
		{
			now = max;
		}
		else
		{
			now -= add;
		}
		return now;
	}
	//!終了時間をまで加算値分増やします。上限以上になったら停止します
	void Add()
	{
		if (now >= max)
		{
			return;
		}
		now += add;
	}
	//!終了時間をまで加算値分引きます。下限以下になったら停止します
	void Sub()
	{
		if (now <= min)
		{
			return;
		}
		now -= add;
	}
	//!経過時間を0にし、フラグを再セットします
	void Reset()
	{
		isMax = false;
		isMin = false;
		now = 0;
	}

	//!現在のカウンターの値をミリ秒で返します
	T GetMilliSecond(T frameRate)
	{
		return (now / frameRate) * 1000;
	}

	//!終了時間を設定します
	void SetEndTime(T max, T min)
	{
		this->max = max;
		this->min = min;
	}
	//!終了時間を設定します
	void SetEndTime(T endTime)
	{
		this->max = endTime;
		this->min = -endTime;
	}
	//!終了時間を型の最大値にします
	void SetInfinity()
	{
		max = std::numeric_limits<T>::max();
		min = std::numeric_limits<T>::min();
	}

	//残り時間(終了時間-経過時間)を返す
	T GetRemainingTime() const
	{
		return max - now;
	}
	/*!
	* @brief 現在のカウンターの値を返します
	* @return 現在のカウンターの値
	*/
	const T& GetCurrentCount() const
	{
		return now;
	}

	/*!
	* @brief 現在のカウンターの値が1度でも上限値以上に到達したか返します
	* @return bool
	*/
	bool IsMax()
	{
		if (now >= max)
		{
			isMax = true;
		}
		return isMax;
	}

	/*!
	* @brief 現在のカウンターの値が1度でも下限値以下に到達したか返します
	* @return bool
	*/
	bool IsMin()
	{
		if (now <= min)
		{
			isMin = true;
		}
		return isMin;
	}
};

typedef TemplateCounter<int>Counter;
typedef TemplateCounter<long>Counter_l;
typedef TemplateCounter<long long>Counter_ll;
typedef TemplateCounter<float>Counter_f;
typedef TemplateCounter<double>Counter_d;