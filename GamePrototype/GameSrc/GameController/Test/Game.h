#pragma once
#include "../GameSrc/ECS/ECS.hpp"
#include "../GameSrc/Components/Transform.hpp"
#include "../GameSrc/System/System.hpp"
class Game
{
private:
	ECS::EntityManager* pManager;
	ECS::Entity* player;
	ECS::Entity* hitBox;
	void EventUpDate();
public:
	enum GameGroup : ECS::Group
	{
		Player,
		Box,
		PlayerAttackCollision,
	};
	Game();
	//Entity�̏����������𖾎��I�ɌĂяo���܂�
	void Initialize();
	//Entity�����Event�̍X�V�������s���܂�
	void Update();
	//Entity�̕`����s���܂�
	void Draw();
};



