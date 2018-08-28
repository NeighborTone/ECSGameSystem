#pragma once
#include "../GameSrc/ECS/ECS.hpp"
#include "../GameSrc/Components/Transform.hpp"
#include <functional>
#include <unordered_map>

class Game
{
private:
	ECS::EntityManager* pManager;
	ECS::Entity* white;
	ECS::Entity* red;
	ECS::Entity* green;
	ECS::Entity* hoge;
	ECS::Entity* blue[5];
public:
	enum GameGroup : ECS::Group
	{
		Box,
	};
	Game();
	//Entity�̏����������𖾎��I�ɌĂяo���܂�
	void Initialize();
	//Entity�����Event�̍X�V�������s���܂�
	void Update();
	//Entity�̕`����s���܂�
	void Draw();
};
//$Test$
//�ΐF�̔��̌��^(ArcheType)�����
class GreenBoxArcheType : public ECS::IArcheType<>
{
public:
	ECS::Entity* operator()() override
	{
		auto& entity(ECS::EcsSystem::GetManager().AddEntity());
		entity.AddComponent<ECS::Transform>().SetPosition(100.f, 100.f);
		entity.AddComponent<ECS::HitBase>(20.f, 20.f).SetColor(0, 255, 0);
		entity.AddGroup(Game::GameGroup::Box);
		return &entity;
	}
};

//$Test$
//�F�̔��̌��^(ArcheType)�����B�܂��A������float�ϐ���2�󂯎���
class BlueBoxArcheType : public ECS::IArcheType<float,float>
{
public:
	ECS::Entity* operator()(const float x, const float y) override
	{
		auto& entity(ECS::EcsSystem::GetManager().AddEntity());
		entity.AddComponent<ECS::Transform>().SetPosition(x, y);
		entity.AddComponent<ECS::HitBase>(20.f, 20.f).SetColor(0, 0, 255);
		entity.AddGroup(Game::GameGroup::Box);
		return &entity;
	}
};

//$Test$
//�����Ɏw�肵���F�̔��̌��^(ArcheType)�����B
class ColorBoxArcheType : public ECS::IArcheType<int,int,int>
{
public:
	ECS::Entity* operator()(const int r, const int g, int b) override
	{
		auto& entity(ECS::EcsSystem::GetManager().AddEntity());
		entity.AddComponent<ECS::Transform>().SetPosition(300.f, 300.f);
		entity.AddComponent<ECS::HitBase>(20.f, 20.f).SetColor(r, g, b);
		entity.AddGroup(Game::GameGroup::Box);
		return &entity;
	}
};

