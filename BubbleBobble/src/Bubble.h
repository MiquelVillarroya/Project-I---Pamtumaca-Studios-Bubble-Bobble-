#pragma once
#include "Entity.h"
#include "TileMap.h"


//#defines here
//Representation model size
#define BUBBLE_FRAME_SIZE		16

//Logical model size 16x16
#define BUBBLE_PHYSICAL_WIDTH	14
#define BUBBLE_PHYSICAL_HEIGHT	14

//Bubble speed
#define BUBBLE_SPEED			1

//Initial bubble impulse
#define BUBBLE_MAX_FORCE		4

//Update bubble impulse delay
#define BUBBLE_FORCE_DELAY		2

//Max Height
#define MAX_HEIGHT				40

//Movement advance
#define HORIZONTAL_ADVANCE		10
#define HORIZONTAL_ADVANCE_TOP	8
#define VERTICAL_ADVANCE		1


enum class BubbleState { SHOT, NORMAL, ENEMY};
enum class BubbleDirection {LEFT, RIGHT};
enum class BubbleAnim {
	INITIAL,
	IDLE,
	NUM_ANIMATIONS
};

class Bubble : public Entity {
public:
	Bubble(const Point& p, BubbleDirection);
	~Bubble();

	AppStatus Initialise();
	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

private:

	//Bubble Mechanics
	void MoveX();
	void MoveY();

	//Animations
	void SetAnimation(int id);
	BubbleAnim GetAnimation();

	//
	bool IsAlive() const;
	bool alive;

	BubbleState state;
	BubbleDirection direction;

	int forceDelay;
	int forceMax;
};
