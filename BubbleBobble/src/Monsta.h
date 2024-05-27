#pragma once
#include "Enemy.h"

//Representation model size: 16x16
#define MONSTA_SPEED			1
//Logical model size
#define	MONSTA_ANIM_DELAY		ANIM_DELAY

enum class VertDir { UP, DOWN };
enum class MonstaAnim {
	WALKING_RIGHT, WALKING_LEFT,
	WALKING_ANGRY_LEFT, WALKING_ANGRY_RIGHT,
	DYING,
	NUM_ANIMATIONS
};

class Monsta : public Enemy {
public:
	Monsta(const Point& p, int width, int height, int frame_width, int frame_height);
	~Monsta();

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
	void MoveX();
	void MoveY();

	VertDir vert_dir;

};