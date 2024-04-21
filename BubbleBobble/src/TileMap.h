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
	BLOCK_LVL1 = 1, BLOCK_LVL2 = 2, WALL_LVL2_1 = 3, WALL_LVL2_2 = 4, WALL_LVL2_3 = 5, WALL_LVL2_4 = 6, NUMBER_BLOCK_LVL1_1 = 7, NUMBER_BLOCK_LVL1_2 = 8, PLAT_LVL1 = 9,  PLAT_LVL2=10,

	// 50 <= id < 100: special tiles
	FOOD_MUSHROOM = 50, FOOD_BANANA, FOOD_CHERRY, FOOD_ICE_CREAM, FOOD_FLAM, FOOD_CAKE,


	// id >= 100: entities' initial locations
	PLAYER = 100, ZENCHAN_LEFT, ZENCHAN_RIGHT, HIDEGONS_LEFT, HIDEGONS_RIGHT,

	//Intervals
	STATIC_FIRST = BLOCK_LVL1,
	STATIC_LAST = NUMBER_BLOCK_LVL1_2,
	SOLID_FIRST = BLOCK_LVL1,
	SOLID_LAST = NUMBER_BLOCK_LVL1_2,
	PLAT_FIRST = PLAT_LVL1,
	PLAT_LAST = PLAT_LVL2,
	SPECIAL_FIRST = FOOD_MUSHROOM ,
	SPECIAL_LAST = FOOD_CAKE ,
	ENTITY_FIRST = PLAYER,
	ENTITY_LAST = ZENCHAN_RIGHT
};

class TileMap
{
public:
	TileMap();
	~TileMap();

	AppStatus Initialise();
	AppStatus Load(int data[], int w, int h);
	void Update();
	void Render();
	void Release();

	//Test for collisions with walls
	bool TestCollisionWallLeft(const AABB& box) const;
	bool TestCollisionWallRight(const AABB& box) const;
	
	//Test collision with the ground and update 'py' with the maximum y-position to prevent
	//penetration of the grounded tile, that is, the pixel y-position above the grounded tile.
	//Grounded tile = solid tile (blocks) or ladder tops.
	bool TestCollisionGround(const AABB& box, int *py) const;
	
	//Test if there is a ground tile one pixel below the given box
	bool TestFalling(const AABB& box) const;

private:
	void InitTileDictionary();

	Tile GetTileIndex(int x, int y) const;
	bool IsTileSolid(Tile tile) const;
	bool IsTilePlat(Tile tile) const;
	bool CollisionX(const Point& p, int distance) const;
	bool CollisionY(const Point& p, int distance) const;

	//Tile map
	Tile *map;

	//Size of the tile map
	int size, width, height;
	
	//Dictionary of tile frames
	std::unordered_map<int, Rectangle> dict_rect;

	Sprite* laser; //fire
	
	//Tile sheet
	const Texture2D *img_tiles;
};

