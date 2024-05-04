#pragma once
#include "Entity.h"
#include "Shot.h"


//#defines here

#define PHYSICAL_OFFSET			7

//Bubble speed
#define BUBBLE_SPEED			1

//Max Height & Top Offset
#define MAX_HEIGHT				40
#define TOP_OFFSET				20

//Movement advance
#define HORIZONTAL_ADVANCE_TOP	1
#define VERTICAL_ADVANCE		1

//Bubble control
#define RED_TIME				7		
#define BLINK_TIME				10
#define POP_TIME				12


enum class BubbleState { NORMAL, RED_END, BLINK_END};
enum class BubbleAnim {
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

	bool Update(const AABB& box) override;

private:

	//Bubble Mechanics
	void MoveX();
	void MoveY();
	void BubbleCounter(bool& flag);

	//BubbleTimer
	float bubbleTimer;

	BubbleState state;

	//Bubble shot variables
	int forceDelay;
	int forceMax;
};
