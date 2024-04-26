#pragma once
#include "Enemy.h"

//Representation model size: 16x16
#define HIDEGONS_SPEED			1
//Logical model size
#define	HIDEGONS_ANIM_DELAY		ANIM_DELAY

enum class HidegonsState { ROAMING, JUMPING, FALLING, SHOOTING};
enum class HidegonsAnim {
	WALKING_RIGHT, WALKING_LEFT,
	WALKING_ANGRY_LEFT, WALKING_ANGRY_RIGHT,
	DYING,
	NUM_ANIMATIONS
};

class Hidegons : public Enemy {
public:
	Hidegons(const Point& p, int width, int height, int frame_width, int frame_height);
	~Hidegons();

	//Initialize the enemy with the specified look and area
	AppStatus Initialise(Look look, const AABB& area) override;

	//Update the enemy according to its logic, return true if the enemy must shoot
	bool Update(const AABB& box) override;

private:

	//Movement
	void MoveX();
	void MoveY();

	int attack_delay;
	HidegonsState state;

};