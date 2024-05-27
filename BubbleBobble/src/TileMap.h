#pragma once
#include <raylib.h>
#include "Sprite.h"
#include "Point.h"
#include "AABB.h"
#include "Globals.h"
#include <unordered_map>

enum class Tile {

	// -1: area covered by an entity
	EMPTY = -1,
	//  0: air tile
	AIR = 0,

	// 0 < id < 50: static tiles
	BLOCK_LVL1 = 1, BLOCK_LVL2 = 2,BLOCK_LVL3=3,BLOCK_LVL4=4,BLOCK_SR=5,BLOCK_LVL5=6,

	WALL_LVL2_1 = 3, WALL_LVL2_2 = 4, WALL_LVL2_3 = 5, WALL_LVL2_4 = 6,WALL_SR_1,WALL_SR_2,WALL_SR_3,WALL_SR_4,

	NUMBER_BLOCK_LVL1_1 = 7, NUMBER_BLOCK_LVL1_2 = 8, NUMER_BLOCK_LVL2_1,NUMBER_BLOCK_LVL2_2,NUMBER_BLOCK_LVL2_3,NUMBER_BLOCK_LVL2_4,NUMBER_BLOC_LVL3_1, NUMBER_BLOC_LVL3_2,
	NUMBER_BLOC_LVL3_3, NUMBER_BLOC_LVL3_4, NUMBER_BLOC_LVL4_1, NUMBER_BLOC_LVL4_2, NUMBER_BLOC_LVL4_3, NUMBER_BLOC_LVL4_4, NUMBER_BLOC_LVL5_1, NUMBER_BLOC_LVL5_2,
	NUMBER_BLOC_LVL5_3, NUMBER_BLOC_LVL5_4,

	PLAT_LVL1 = 9,  PLAT_LVL2=10, 

	PLAT_RIGHT_LVL1=11,PLAT_RIGHT_LVL2=12, 

	PLAT_LEFT_LVL1=13, PLAT_LEFT_LVL2=14,

	// 50 <= id < 100: special tiles (mainly objects)
	FOOD_MUSHROOM = 50, FOOD_BANANA, FOOD_CHERRY, FOOD_ICE_CREAM, FOOD_FLAM, FOOD_CAKE,


	// id >= 100: entities' initial locations
	PLAYER = 100, ZENCHAN, HIDEGONS,

	//Intervals
	STATIC_FIRST = BLOCK_LVL1,
	STATIC_LAST = PLAT_LVL2,
	SOLID_FIRST = BLOCK_LVL1,
	SOLID_LAST = NUMBER_BLOCK_LVL1_2,
	PLAT_FIRST = PLAT_LVL1,
	PLAT_LAST = PLAT_LVL2,
	PLAT_RIGHT_FIRST=PLAT_RIGHT_LVL1,
	PLAT_RIGHT_LAST=PLAT_RIGHT_LVL2,
	PLAT_LEFT_FIRST=PLAT_LEFT_LVL1,
	PLAT_LEFT_LAST=PLAT_LEFT_LVL2,

	OBJECT_FIRST = FOOD_MUSHROOM ,
	OBJECT_LAST = FOOD_CAKE ,
	ENTITY_FIRST = PLAYER,
	ENTITY_LAST = HIDEGONS
};

class TileMap
{
public:
	TileMap();
	~TileMap();

	AppStatus Initialise();
	AppStatus Load(int data[], int w, int h);
	void ClearObjectEntityPositions();

	void Update();
	void Render();
	void Release();

	bool IsTileObject(Tile tile) const;
	bool IsTileEntity(Tile tile) const;

	//Test for collisions with walls
	bool TestCollisionWallLeft(const AABB& box) const;
	bool TestCollisionWallRight(const AABB& box) const;
	
	//Test collision with the ground and update 'py' with the maximum y-position to prevent
	//penetration of the grounded tile, that is, the pixel y-position above the grounded tile.
	//Grounded tile = solid tile (blocks) or ladder tops.
	bool TestCollisionGround(const AABB& box, int *py) const;
	
	//Test if there is a ground tile one pixel below the given box
	bool TestFalling(const AABB& box) const;

	//Given a hitbox, computes the maximum swept box model along the X-axis without solid tiles
	AABB GetSweptAreaX(const AABB& hitboxbox) const;

private:
	void InitTileDictionary();

	Tile GetTileIndex(int x, int y) const;
	bool IsTileSolid(Tile tile) const;
	bool IsTilePlat(Tile tile) const;
	bool IsTilePlatRight(Tile  tile) const;
	bool IsTilePlatLeft(Tile  tile) const;
	bool CollisionX(const Point& p, int distance) const;
	bool CollisionY(const Point& p, int distance) const;

	//Tile map
	Tile *map;

	//Size of the tile map
	int size, width, height;
	
	//Dictionary of tile frames
	std::unordered_map<int, Rectangle> dict_rect;

	//Animated tile (fire)
	Sprite* laser;
	
	//Tile sheet
	const Texture2D *img_tiles;
};

