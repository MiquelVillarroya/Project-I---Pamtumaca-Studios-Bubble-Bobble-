#pragma once
#include "Entity.h"
#include "Tilemap.h"

//Representation model size 16x16
#define BUBBLE_FRAME_SIZE		16

//Logical model size 16x16
#define BUBBLE_PHYSICAL_WIDTH	14
#define BUBBLE_PHYSICAL_HEIGHT	14

enum class ShotType { BUBBLE, FIREBALL };

class Shot : public Entity
{
public:
	Shot(const Point& p, const Point& d, int width, int heigth, int frame_width, int frame_height);
	virtual ~Shot();

	virtual AppStatus Initialise() = 0;
	void SetTileMap(TileMap* tilemap);
	
	//Update the enemy according to its logic;
	virtual void Update(const AABB& box) = 0;
	bool IsMovingLeft() const;
	bool IsMovingRight() const;

protected:

	//Animation Management
	void SetAnimation(int id);

	TileMap* map;
};

