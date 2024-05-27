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
	BLOCK_LVL1 = 1, BLOCK_LVL2 = 2, BLOCK_LVL3 = 3, BLOCK_LVL4 = 4, BLOCK_SR = 5, BLOCK_LVL5 = 6,

	WALL_LVL2_1 = 7, WALL_LVL2_2 = 8, WALL_LVL2_3 = 9, WALL_LVL2_4 = 10, WALL_SR_1 = 11, WALL_SR_2 = 12, WALL_SR_3 = 13, WALL_SR_4 = 14,

	NUMBER_BLOCK_LVL1_1 = 15, NUMBER_BLOCK_LVL1_2 = 16, NUMBER_BLOCK_LVL2_1 = 17, NUMBER_BLOCK_LVL2_2 = 18, NUMBER_BLOCK_LVL2_3 = 19, NUMBER_BLOCK_LVL2_4 = 20, 
	NUMBER_BLOC_LVL3_1 = 21, NUMBER_BLOC_LVL3_2 = 22,NUMBER_BLOC_LVL3_3 = 23, NUMBER_BLOC_LVL3_4 = 24, NUMBER_BLOC_LVL4_1 = 25, NUMBER_BLOC_LVL4_2 = 26, NUMBER_BLOC_LVL4_3 = 27,
	NUMBER_BLOC_LVL4_4 = 28, NUMBER_BLOC_LVL5_1 = 29, NUMBER_BLOC_LVL5_2 = 30, NUMBER_BLOC_LVL5_3 = 31, NUMBER_BLOC_LVL5_4 = 32,

	PLAT_LVL1 = 33, PLAT_LVL2 = 34, PLAT_LVL3 = 35, PLAT_LVL4 = 36, PLAT_LVL5 = 37,PLAT_SR = 38,

	PLAT_RIGHT_LVL1 = 39,PLAT_RIGHT_LVL2 = 40, PLAT_RIGHT_LVL3 = 41, PLAT_RIGHT_LVL4 = 42, PLAT_RIGHT_LVL5 = 43, PLAT_RIGHT_SR = 44,

	PLAT_LEFT_LVL1=45, PLAT_LEFT_LVL2=46, PLAT_LEFT_LVL3 = 47, PLAT_LEFT_LVL4 = 48, PLAT_LEFT_LVL5 = 49, 

	// 50 <= id < 100: special tiles (mainly objects)
	FOOD_MUSHROOM = 50, FOOD_BANANA, FOOD_CHERRY, FOOD_ICE_CREAM, FOOD_FLAM, FOOD_CAKE,


	// id >= 100: entities' initial locations
	PLAYER = 100, ZENCHAN, HIDEGONS,

	//Intervals
	STATIC_FIRST = BLOCK_LVL1,
	STATIC_LAST = PLAT_LEFT_LVL5,
	SOLID_FIRST = BLOCK_LVL1,
	SOLID_LAST = NUMBER_BLOC_LVL5_4,
	PLAT_FIRST = PLAT_LVL1,
	PLAT_LAST = PLAT_SR,
	PLAT_RIGHT_FIRST=PLAT_RIGHT_LVL1,
	PLAT_RIGHT_LAST=PLAT_RIGHT_SR,
	PLAT_LEFT_FIRST=PLAT_LEFT_LVL1,
	PLAT_LEFT_LAST=PLAT_LEFT_LVL5,

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

