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
		friend class Entity;	//Entity�ɂ���ĎE���ꂽ���̂ł����Ȃ���
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
		//���̃R���|�[�l���g�������Ă��邩�Ԃ��܂�
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
		//����Entity�ɂ��Ă���Component�̏������������s���܂�
		void Initialize()
		{
			for (auto& c : components) c->Initialize();
		}
		//����Entity�ɂ��Ă���Component�̍X�V�������s���܂�
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
		//����Entity�ɂ��Ă���Component��3D�`�揈�����s���܂�
		void Draw3D()
		{
			for (auto& c : components) c->Draw3D();
		}
		//����Entity�ɂ��Ă���Component��2D�`�揈�����s���܂�
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
		//Entity�̐�����Ԃ�Ԃ��܂�
		bool IsActive() const { return active; }
		//Entity���E���܂�
		void Destroy() 
		{
			active = false; 
		}
		//Entity���w�肵���O���[�v�ɓo�^����Ă��邩�Ԃ��܂�
		bool HasGroup(Group group) const noexcept
		{
			return groupBitSet[group];
		}
		//Entity���O���[�v�ɓo�^���܂�
		void AddGroup(Group group) noexcept;
		//Entity���O���[�v��������܂�
		void DeleteGroup(Group group) noexcept
		{
			groupBitSet[group] = false;
		}
		//Entity�Ɏw�肵��Component�����邩�Ԃ��܂�
		template <typename T> bool HasComponent() const 
		{
			return componentBitSet[GetComponentTypeID<T>()];
		}

		//�R���|�[�l���g�̒ǉ����\�b�h
		//�ǉ����ꂽ��R���|�[�l���g�̏��������\�b�h���Ă΂�܂�
		template <typename T, typename... TArgs> T& AddComponent(TArgs&&... args)
		{
			//�d���͋����Ȃ�
			if (HasComponent<T>())
			{
				return GetComponent<T>();
			}
			//Tips: std::forward
			//�֐��e���v���[�g�̈�����]������B
			//���̊֐��́A�n���ꂽ������T&&�^�ɃL���X�g���ĕԂ��B�i���FT�����Ӓl�Q�Ƃ̏ꍇ�ɂ�T&&�����Ӓl�Q�ƂɂȂ�A����ȊO�̏ꍇ��T&&�͉E�Ӓl�Q�ƂɂȂ�B�j
			//���̊֐��́A��ɓ]���֐��iforwarding function�j�̎�����P��������ړI�Ŏg����F
			T* c(new T(std::forward<TArgs>(args)...));
			c->entity = this;
			std::unique_ptr<Component> uPtr(c);
			components.emplace_back(std::move(uPtr));

			componentArray[GetComponentTypeID<T>()] = c;
			componentBitSet[GetComponentTypeID<T>()] = true;

			c->Initialize();
			return *c;
		}

		//�w�肵���R���|�[�l���g���폜���܂�
		template<typename T> void DeleteComponent() noexcept
		{
			if (HasComponent<T>())
			{
				GetComponent<T>().DeleteThis();
				componentBitSet[GetComponentTypeID<T>()] = false;
			}
		}

		//�o�^�����R���|�[�l���g���擾���܂�
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

	//Entity�����N���X
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
		//�O���[�v���Ƃ̕`���o�^���ɍs��
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
		//�A�N�e�B�u�łȂ����̂��폜���܂�
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
		//�w�肵���O���[�v�ɓo�^����Ă���Entity�B��Ԃ��܂�
		std::vector<Entity*>& GetEntitiesByGroup(Group group)
		{
			return groupedEntities[group];
		}
		//Entity���w�肵���O���[�v�ɓo�^���܂�
		void AddToGroup(Entity* pEntity, Group group)
		{
			groupedEntities[group].emplace_back(pEntity);
		}
		//Entity�𐶐������̃|�C���^��Ԃ�
		//�^�O��ݒ肵�Ă����ƃf�o�b�O����Ƃ��ɒǂ������₷��
		Entity& AddEntityAddTag(const std::string& tag)
		{
			Entity* e = new Entity(*this);
			std::unique_ptr<Entity> uPtr(e);
			entityes.emplace_back(std::move(uPtr));
			entityes.back()->tag = tag;
			return *e;
		}
		//Entity�𐶐������̃|�C���^��Ԃ�
		//��{�I�ɂ�������g��
		Entity& AddEntity()
		{
			Entity* e = new Entity(*this);
			std::unique_ptr<Entity> uPtr(e);
			entityes.emplace_back(std::move(uPtr));
			entityes.back()->tag = "";
			return *e;
		}
		//�^�O���w�肵����Entity���擾����
		//���s�����ꍇ������
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
			//�擾�Ɏ��s�����ꍇ�͂Ƃ肠�������Ƃ�
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

	//Entity�̌��^����邽�߂̃C���^�[�t�F�[�X
	template<class... Args>
	class IArcheType
	{
	private:
		virtual ECS::Entity* operator()(Args...) = 0;
	};
}
