#pragma once
#include "Entity.h"
#include "Shot.h"


//#defines here

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
enum class BubbleAnim {
	INITIAL,
	IDLE,
	RED_START,
	RED_BLINK,
	POP,
	NUM_ANIMATIONS
};

class Bubble : public Shot {
public:
	Bubble(const Point& p, const Point& d, int width, int heigth, int frame_width, int frame_heigth);
	~Bubble();

	AppStatus Initialise() override;

	void Update(const AABB& box) override;

	//bool IsAlive() const;

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

	BubbleState state;

	//Bubble shot variables
	int forceDelay;
	int forceMax;
};
