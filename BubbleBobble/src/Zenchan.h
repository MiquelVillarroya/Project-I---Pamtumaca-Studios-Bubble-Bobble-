#pragma once
#include "Enemy.h"

//Representation model size: 16x16
#define ZENCHAN_SPEED			1
//Logical model size
#define	ZENCHAN_ANIM_DELAY		ANIM_DELAY

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

enum class ZenchanState { ROAMING, JUMPING, FALLING, BUBBLE, ANGRY, BLINK_END, RED_END };
enum class ZenchanAnim {
	WALKING_RIGHT, WALKING_LEFT,
	WALKING_ANGRY_LEFT, WALKING_ANGRY_RIGHT,
	DYING,
	NUM_ANIMATIONS
};

class Zenchan : public Enemy {
public:
	Zenchan(const Point& p, int width, int height, int frame_width, int frame_height);
	~Zenchan();

	//Initialize the enemy with the specified look and area
	AppStatus Initialise(Look look, const AABB& area) override;

	//Update the enemy according to its logic, return true if the enemy must shoot
	bool Update(const AABB& box) override;

private:

	//Movement
	void MoveX();
	void MoveY();

	ZenchanState state;

};