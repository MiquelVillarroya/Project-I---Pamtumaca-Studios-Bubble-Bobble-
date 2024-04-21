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

//Bubble logic parameters
#define PHYSICAL_OFFSET			7
#define MAX_HEIGHT				40
#define TOP_OFFSET				20
#define HORIZONTAL_ADVANCE_TOP	1
#define VERTICAL_ADVANCE		1

//Bubble render parameters
#define RED_TIME				7		
#define BLINK_TIME				10
#define POP_TIME				12

enum class EnemyState {NORMAL, BUBBLE, ANGRY, BLINK_END, RED_END, DEAD};
enum class EnemyLook { RIGHT, LEFT };
enum class ZenchanAnim {
	WALK_RIGHT, WALK_LEFT,
	ANGRY_LEFT, ANGRY_RIGHT,
	DEAD,
	BUBBLE, RED_START, RED_BLINK,
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

	EnemyState GetState();
	void SetState(const EnemyState& s);

private:

	//Movement
	void MoveX();
	void MoveY();
	void BubbleCounter();

	//Animations
	void SetAnimation(int id);
	ZenchanAnim GetAnimation();

	EnemyState state;
	EnemyLook look;
	float angryTimer;
	float bubbleTimer;
	bool alive;
	
	TileMap* map;
};