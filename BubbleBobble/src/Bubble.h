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

	BubbleState state;
	BubbleDirection direction;

	//Animations
	void SetAnimation(int id);
	BubbleAnim GetAnimation();

	//
	bool IsAlive() const;
	bool alive;
};
