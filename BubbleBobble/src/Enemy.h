#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 16x16
#define ENEMY_PLAYER_SIZE

//Logical model size
#define	ENEMY_PHYSICAL_WIDTH	14
#define ENEMY_PHYSICAL_HEIGTH	14

class Enemy : public Entity {
public:
	Enemy();
	~Enemy();

	AppStatus Initialise();
	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

private:

	//Movement
	void MoveX();
	void MoveY();

	//Animations
	void SetAnimation(int id);
	

};