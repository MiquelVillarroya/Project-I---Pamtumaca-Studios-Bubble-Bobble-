#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 16x16
#define ENEMY_FRAME_SIZE		16

//Logical model size
#define	ENEMY_PHYSICAL_WIDTH	14
#define ENEMY_PHYSICAL_HEIGTH	14

//Enemy speed
#define ENEMY_SPEED				1

enum class EnemyState {NORMAL, ANGRY};
enum class EnemyLook { RIGHT, LEFT };
enum class ZenchanAnim {
	WALK_RIGHT, WALK_LEFT,
	ANGRY_LEFT, ANGRY_RIGHT,
	DEAD,
	BUBBLE,
	NUM_ANIMATIONS
};

class Enemy : public Entity {
public:
	Enemy(const Point& p, EnemyState s, EnemyLook l);
	~Enemy();

	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

private:

	//Movement
	void MoveX();
	void MoveY();

	//Animations
	void SetAnimation(int id);
	ZenchanAnim GetAnimation();

	EnemyState state;
	EnemyLook look;
	float angryTimer;
	
	TileMap* map;
};