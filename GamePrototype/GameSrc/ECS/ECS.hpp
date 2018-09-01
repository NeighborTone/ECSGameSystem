#pragma once
#include <bitset>
#include <array>
#include <memory>
#include <vector>
#include <assert.h>
namespace ECS
{
	class Entity;
	class Component;
	class EntityManager;

	using ComponentID = std::size_t;
	using Group = std::size_t;


	inline ComponentID GetNewComponentTypeID() noexcept
	{
		static ComponentID lastID = 0;
		return ++lastID;
	}

	template <typename T> inline ComponentID GetComponentTypeID() noexcept
	{
		static ComponentID typeID = GetNewComponentTypeID();
		return typeID;
	}

	constexpr std::size_t MaxComponents = 32;
	constexpr std::size_t MaxGroups = 32;

	using ComponentBitSet = std::bitset<MaxComponents>;
	using ComponentArray = std::array<Component*, MaxComponents>;
	using GroupBitSet = std::bitset<MaxGroups>;

	class Component
	{
	private:
		friend class Entity;	//Entityによって殺されたいのでこうなった
		bool active = true;
		void DeleteThis()
		{
			if (this != nullptr)
			{
				active = false;
			}

		}
	public:
		Entity * entity;
		virtual void Initialize() {};
		virtual void UpDate() {};
		virtual void Draw3D() {};
		virtual void Draw2D() {};
		virtual ~Component() {}
		//このコンポーネントが生きているか返します
		virtual bool IsActive() const final { return active; }

	};

	struct ComponentData : public Component
	{

	};

	class Entity
	{
	private:
		friend class EntityManager;
		std::string tag;
		EntityManager& manager_;
		bool active = true;
		std::vector<std::unique_ptr<Component>> components;
		ComponentArray  componentArray;
		ComponentBitSet componentBitSet;
		GroupBitSet groupBitSet;
		void RefreshComponent()
		{
			components.erase(std::remove_if(std::begin(components), std::end(components),
				[](const std::unique_ptr<Component> &pCom)
			{
				return !pCom->IsActive();
			}),
				std::end(components));
		}
		
	public:

		Entity(EntityManager& manager) : manager_(manager) {}
		//このEntityについているComponentの初期化処理を行います
		void Initialize()
		{
			for (auto& c : components) c->Initialize();
		}
		//このEntityについているComponentの更新処理を行います
		void UpDate()
		{
			RefreshComponent();
			for (auto& c : components)
			{
				if (c == nullptr)
				{
					continue;
				}
				c->UpDate();
			}
		}
		//このEntityについているComponentの3D描画処理を行います
		void Draw3D()
		{
			for (auto& c : components) c->Draw3D();
		}
		//このEntityについているComponentの2D描画処理を行います
		void Draw2D()
		{
			for (auto& c : components)
			{
				if (c == nullptr)
				{
					continue;
				}
				c->Draw2D();
			}
		}
		//の生存状態を返します
		bool IsActive() const { return active; }
		//Entityを殺します
		void Destroy() 
		{
			active = false; 
		}
		//Entityが指定したグループに登録されているか返します
		bool HasGroup(Group group) const noexcept
		{
			return groupBitSet[group];
		}
		//Entityをグループに登録します
		void AddGroup(Group group) noexcept;
		//Entityをグループから消します
		void DeleteGroup(Group group) noexcept
		{
			groupBitSet[group] = false;
		}
		//Entityに指定したComponentがあるか返します
		template <typename T> bool HasComponent() const 
		{
			return componentBitSet[GetComponentTypeID<T>()];
		}

		//コンポーネントの追加メソッド
		//追加されたらコンポーネントの初期化メソッドが呼ばれます
		template <typename T, typename... TArgs> T& AddComponent(TArgs&&... args)
		{
			//重複は許可しない
			if (HasComponent<T>())
			{
				return GetComponent<T>();
			}
			//Tips: std::forward
			//関数テンプレートの引数を転送する。
			//この関数は、渡された引数をT&&型にキャストして返す。（注：Tが左辺値参照の場合にはT&&も左辺値参照になり、それ以外の場合にT&&は右辺値参照になる。）
			//この関数は、主に転送関数（forwarding function）の実装を単純化する目的で使われる：
			T* c(new T(std::forward<TArgs>(args)...));
			c->entity = this;
			std::unique_ptr<Component> uPtr(c);
			components.emplace_back(std::move(uPtr));

			componentArray[GetComponentTypeID<T>()] = c;
			componentBitSet[GetComponentTypeID<T>()] = true;

			c->Initialize();
			return *c;
		}

		//指定したコンポーネントを削除します
		template<typename T> void DeleteComponent() noexcept
		{
			if (HasComponent<T>())
			{
				GetComponent<T>().DeleteThis();
				componentBitSet[GetComponentTypeID<T>()] = false;
			}
		}

		//登録したコンポーネントを取得します
		template<typename T> T& GetComponent() const
		{
			assert(HasComponent<T>());
			auto ptr(componentArray[GetComponentTypeID<T>()]);
			return *static_cast<T*>(ptr);
		}

		const std::string& GetTag() const
		{
			return tag;
		}
	};

	//Entity統括クラス
	class EntityManager
	{
	private:
		std::vector<std::unique_ptr<Entity>> entityes;
		std::array<std::vector<Entity*>, MaxGroups> groupedEntities;
	public:
		void Initialize()
		{
			for (auto& e : entityes) e->Initialize();
		}
		void UpDate()
		{
			for (auto& e : entityes)
			{
				if (e == nullptr)
				{
					continue;
				}
				e->UpDate();
			}
		}
		void Draw3D()
		{
			for (auto& e : entityes) e->Draw3D();
		}
		//グループごとの描画を登録順に行う
		void OrderByDraw(const size_t TheMaximumNumberOfRegistered)
		{
			for (auto i(0u); i < TheMaximumNumberOfRegistered; ++i)
			{
				const auto& entity = groupedEntities[i];
				for (const auto& e : entity)
				{
					e->Draw2D();
				}
			}
		}
		void Draw2D()
		{
			for (auto& e : entityes)
			{
				e->Draw2D();
			}
		}
		//アクティブでないものを削除します
		void Refresh()
		{
			for (auto i(0u); i < MaxGroups; ++i)
			{
				auto& v(groupedEntities[i]);

				v.erase(std::remove_if(std::begin(v), std::end(v),
					[i](Entity* pEntity)
				{
					return !pEntity->IsActive() ||
						!pEntity->HasGroup(i);
				}),
					std::end(v));
			}

			entityes.erase(std::remove_if(std::begin(entityes), std::end(entityes),
				[](const std::unique_ptr<Entity> &pEntity)
			{
				return !pEntity->IsActive();
			}),
				std::end(entityes));
		}
		//指定したグループに登録されているEntity達を返します
		std::vector<Entity*>& GetEntitiesByGroup(Group group)
		{
			return groupedEntities[group];
		}
		//Entityを指定したグループに登録します
		void AddToGroup(Entity* pEntity, Group group)
		{
			groupedEntities[group].emplace_back(pEntity);
		}
		//Entityを生成しそのポインタを返す
		//タグを設定しておくとデバッグするときに追いかけやすい
		Entity& AddEntityAddTag(const std::string& tag)
		{
			Entity* e = new Entity(*this);
			std::unique_ptr<Entity> uPtr(e);
			entityes.emplace_back(std::move(uPtr));
			entityes.back()->tag = tag;
			return *e;
		}
		//Entityを生成しそのポインタを返す
		//基本的にこちらを使う
		Entity& AddEntity()
		{
			Entity* e = new Entity(*this);
			std::unique_ptr<Entity> uPtr(e);
			entityes.emplace_back(std::move(uPtr));
			entityes.back()->tag = "";
			return *e;
		}
		//タグを指定しそのEntityを取得する
		//失敗した場合落ちる
		Entity& GetEntity(const std::string& tag)
		{
			constexpr bool  NOT_FOUND_TAG = false;
			assert(!(tag == ""));
			for (const auto& it : entityes)
			{
				if (it->tag == tag)
				{
					return *it;
				}
			}
			//取得に失敗した場合はとりあえず落とす
			assert(NOT_FOUND_TAG);
			return *entityes[0];
		}
	};

	class EcsSystem
	{
	public:
		static EntityManager& GetManager()
		{
			static std::unique_ptr<EntityManager> mana = std::make_unique<EntityManager>();
			return *mana;
		}
	};

	//Entityの原型を作るためのインターフェース
	template<class... Args>
	class IArcheType
	{
	private:
		virtual ECS::Entity* operator()(Args...) = 0;
	};
}
