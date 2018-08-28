#pragma once
#include "../GameSrc/ECS/ECS.hpp"
class Game
{
private:
	ECS::EntityManager* pManager;
	ECS::Entity* white;
	ECS::Entity* red;
public:
	Game();
	//Entity�̏����������𖾎��I�ɌĂяo���܂�
	void Initialize();
	//Entity�����Event�̍X�V�������s���܂�
	void Update();
	//Entity�̕`����s���܂�
	void Draw();
};