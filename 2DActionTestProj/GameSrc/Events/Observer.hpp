#pragma once
#include <vector>
#include <string>
//https://qiita.com/a-beco/items/717d6dff5355776e3702 //�����Ⴛ�̂܂�܎ʌo����
//http://d.hatena.ne.jp/backpaper0/20111111/1321012118 //���Ƃ����킩��₷������
namespace Event
{
	template<class T>
	class ObserverBase
	{
	public:

		virtual ~ObserverBase() = default;
	private:
		//�󂯎�����L�[�����݂��邩���ׂ�
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
		//Observer�̒ǉ�
		void AddObserver(ObserverBase<T>* observer)
		{
			pObservers.emplace_back(observer);
		}
		//Observer�̍폜
		void RemoveObserver(ObserverBase<T>* observer)
		{
			auto it = std::find(std::begin(pObservers),
				std::end(pObservers), observer);
			if (it != pObservers.end())
			{
				pObservers.erase(it);
			}
		}
		//�C�x���g�ʒm���󂯎�菈�����Ăяo��
		//�I�u�U�[�o��Update�֐����Ă�
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