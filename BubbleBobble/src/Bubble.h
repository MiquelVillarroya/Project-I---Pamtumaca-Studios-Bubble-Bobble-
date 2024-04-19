#pragma once
#include "Entity.h"
#include "TileMap.h"

//#defines here
//Representation model size
#define BUBBLE_FRAME_SIZE		16

//Logical model size 16x16
#define BUBBLE_PHYSICAL_WIDTH	16
#define BUBBLE_PHYSICAL_HEIGHT	16

//Bubble speed
#define BUBBLE_SPEED			1


enum class State {

};
enum class BubbleAnim {
	NUM_ANIMATIONS
};

class Bubble : public Entity {
public:
	Bubble(const Point& p);
	~Bubble();

	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

private:
	bool IsAlive() const;
	bool alive;
	TileMap* map;
};
