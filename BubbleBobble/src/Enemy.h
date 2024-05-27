#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 32x32
#define ENEMY_FRAME_SIZE		16
//Logical model size: 24x30
#define ENEMY_PHYSICAL_WIDTH	16
#define ENEMY_PHYSICAL_HEIGHT	16

enum class EnemyType { NONE, ZENCHAN, HIDEGONS, MONSTA};

class Enemy : public Entity
{
public:
	Enemy(const Point& p, int width, int height, int frame_width, int frame_height);
	virtual ~Enemy();

	//Draw the maximum visibility area of the enemy
	void DrawVisibilityArea(const Color& col) const;

	//Initialize enemy tilemap for collision checks
	void SetTileMap(TileMap* tilemap);

	//Pure virtual functions, any class inheriting from this class must provide its own implementations
	//Initialize the enemy with the specified look and area
	virtual AppStatus Initialise(Look look, const AABB& area) = 0;

	//Update the enemy according to its logic, return true if the enemy must shoot
	virtual bool Update(const AABB& box) = 0;

	//Retrieve the position and direction of the shot to be thrown
	virtual void GetShootingPosDir(Point* pos, Point* dir) const = 0;

	//Get the enemy type
	virtual EnemyType GetEnemyType() const = 0;

protected:
	//Return true if the given hitbox is within the visibility area and the enemy is facing it
	bool IsVisible(const AABB& hitbox);

	//Animation Management
	void SetAnimation(int id);

	Look look;
	AABB visibility_area;
	TileMap* map;
};

