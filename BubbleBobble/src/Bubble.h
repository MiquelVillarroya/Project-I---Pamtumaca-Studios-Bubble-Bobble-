#pragma once
#include "Entity.h"
#include "Shot.h"
#include "Enemy.h"
#include "ParticleManager.h"

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
	DARKER_RED,
	RED_BLINK,
	POP,
	NUM_ANIMATIONS
};

class Bubble : public Shot {
public:
	Bubble(const Point& p, const Point& d, int width, int heigth, int frame_width, int frame_heigth, EnemyType type);
	~Bubble();

	AppStatus Initialise() override;

	EnemyType Update(const AABB& box) override;

	EnemyType GetEnemy() const override;

private:

	//Bubble Mechanics
	void MoveX();
	void MoveY();
	void BubbleCounter(EnemyType& flag);

	//Animation Management
	BubbleAnim GetAnimation();
	void SetAnimations();

	//BubbleTimer
	float bubbleTimer;

	BubbleState state;

	//Enemy Management
	EnemyType enemyType;

	//Particle Manager reference
	ParticleManager* particles;
};
