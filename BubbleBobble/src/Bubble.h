#pragma once
#include "Entity.h"
#include "TileMap.h"


//#defines here
//Representation model size
#define BUBBLE_FRAME_SIZE		16

//Logical model size 16x16
#define BUBBLE_PHYSICAL_WIDTH	14
#define BUBBLE_PHYSICAL_HEIGHT	14
#define PHYSICAL_OFFSET			7

//Bubble speed
#define BUBBLE_SPEED			1

//Initial bubble impulse
#define BUBBLE_MAX_FORCE		4

//Update bubble impulse delay
#define BUBBLE_FORCE_DELAY		2

//Max Height & Top Offset
#define MAX_HEIGHT				40
#define TOP_OFFSET				20

//Movement advance
#define HORIZONTAL_ADVANCE		10
#define HORIZONTAL_ADVANCE_TOP	1
#define VERTICAL_ADVANCE		1

//Bubble control
#define RED_TIME				7		
#define BLINK_TIME				10
#define POP_TIME				12


enum class BubbleState { SHOT, NORMAL, RED_END, BLINK_END, ENEMY};
enum class BubbleDirection {LEFT, RIGHT};
enum class BubbleAnim {
	INITIAL,
	IDLE,
	RED_START,
	RED_BLINK,
	POP,
	NUM_ANIMATIONS
};

class Bubble : public Entity {
public:
	Bubble(const Point& p, BubbleDirection);
	~Bubble();

	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

	bool IsAlive() const;

private:

	//Bubble Mechanics
	void MoveX();
	void MoveY();
	void BubbleCounter();

	//Bubble destruction;
	bool alive;
	bool move;

	//BubbleTimer
	float bubbleTimer;

	//Animations
	void SetAnimation(int id);
	BubbleAnim GetAnimation();


	BubbleState state;
	BubbleDirection direction;
	TileMap* map;

	//Bubble shot variables
	int forceDelay;
	int forceMax;
};
