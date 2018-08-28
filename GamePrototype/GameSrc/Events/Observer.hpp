#pragma once
#include <vector>
#include <string>
//https://qiita.com/a-beco/items/717d6dff5355776e3702 //実装例そのまんま写経した
//http://d.hatena.ne.jp/backpaper0/20111111/1321012118 //たとえがわかりやすかった
namespace Event
{
	template<class T>
	class ObserverBase
	{
	public:

		virtual ~ObserverBase() = default;
	private:
		//受け取ったキーが存在するか調べる
		virtual bool ShouldNotify(const std::string& key) = 0;
		virtual void UpDate(T* sender, const std::string& key) = 0;

		template<class F>
		friend class Subject;
	};

	//observer
	template<class T, const std::string&... keys>
	class Observer : public ObserverBase<T>
	{
	public:
		virtual ~Observer() = default;
		virtual void UpDate(T* sender, const std::string& key) = 0;
	private:
		std::vector<std::string> observerKeys = { keys... };
		bool ShouldNotify(const std::string& key_)
		{
			return (
				std::find(std::begin(observerKeys), 
				std::end(observerKeys), key_) != std::end(observerKeys));
		}
	};

	//subject
	template<class T>
	class Subject
	{
	private:
		std::vector<ObserverBase<T>*> pObservers;
	public:
		virtual ~Subject() = default;
		//Observerの追加
		void AddObserver(ObserverBase<T>* observer)
		{
			pObservers.emplace_back(observer);
		}
		//Observerの削除
		void RemoveObserver(ObserverBase<T>* observer)
		{
			auto it = std::find(std::begin(pObservers),
				std::end(pObservers), observer);
			if (it != pObservers.end())
			{
				pObservers.erase(it);
			}
		}
		//イベント通知を受け取り処理を呼び出す
		//オブザーバのUpdate関数を呼ぶ
		void Call(const std::string& key_)
		{
			for (const auto& o : pObservers)
			{
				if (o->ShouldNotify(key_))
				{
					o->UpDate(static_cast<T*>(this),key_);
				}
			}
		}
	};
}