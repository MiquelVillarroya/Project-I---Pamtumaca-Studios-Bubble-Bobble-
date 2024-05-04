#pragma once
#include "Entity.h"
#include "Shot.h"


//#defines here

//Bubble speed
#define BUBBLE_SPEED			1

//Initial bubble impulse
#define BUBBLE_MAX_FORCE		6

//Update bubble impulse delay
#define BUBBLE_FORCE_DELAY		2

//Movement advance
#define HORIZONTAL_ADVANCE		10

class PlayerBubble : public Shot {
public:
	PlayerBubble(const Point& p, const Point& d, int width, int heigth, int frame_width, int frame_heigth);
	~PlayerBubble();

	AppStatus Initialise() override;

	bool Update() override;

private:

	//Bubble shot variables
	int forceDelay;
	int forceMax;
};
