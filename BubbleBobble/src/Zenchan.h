#pragma once
#include "Enemy.h"

//Representation model size: 16x16
#define ZENCHAN_SPEED			1
//Logical model size
#define	ZENCHAN_ANIM_DELAY		ANIM_DELAY

enum class ZenchanState { ROAMING, JUMPING, FALLING };
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


	//To avoid errors
	void GetShootingPosDir(Point* pos, Point* dir) const {};

	//Get enemy type
	virtual EnemyType GetEnemyType() const override;


private:

	//Movement
	void MoveX(const AABB& box);
	void MoveY(const AABB& box);
	void StartFalling();

	ZenchanState state;

};