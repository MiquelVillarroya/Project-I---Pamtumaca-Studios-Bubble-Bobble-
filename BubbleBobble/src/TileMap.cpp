#include "TileMap.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <cstring>

TileMap::TileMap()
{
	map = nullptr;
	width = 0;
	height = 0;
	// Animated tiles (fire)
	laser = nullptr;

	img_tiles = nullptr;

	InitTileDictionary();
}
TileMap::~TileMap()
{
	if (map != nullptr)
	{
		delete[] map;
		map = nullptr;
	}
	if (laser != nullptr)
	{
		laser->Release();
		delete laser;
		laser = nullptr;
	}
}
void TileMap::InitTileDictionary()
{
	const int n = TILE_SIZE;
	const int sx = SHADOW_OFFSET_X;
	const int sy = SHADOW_OFFSET_Y;

	dict_rect[(int)Tile::BLOCK_LVL1] = { 0,  0, n + sx, n + sy};
	dict_rect[(int)Tile::PLAT_LVL1] = { 0,  0, n + sx, n + sy};
	dict_rect[(int)Tile::NUMBER_BLOCK_LVL1_1] = { 2*n,  0, n + sx, n + sy };
	dict_rect[(int)Tile::NUMBER_BLOCK_LVL1_2] = { 4 * n,  0, n + sx, n + sy };
	dict_rect[(int)Tile::PLAT_RIGHT_LVL1] = { 0,  0, n + sx, n + sy };
	dict_rect[(int)Tile::PLAT_LEFT_LVL1] = { 0,  0, n + sx, n + sy };

	dict_rect[(int)Tile::BLOCK_LVL2] = { 0,  2*n, n + sx, n + sy };
	dict_rect[(int)Tile::PLAT_LVL2] = { 0,  2*n, n + sx, n + sy };
	dict_rect[(int)Tile::WALL_LVL2_1] = { 2*n,  2*n, n + sx, n + sy };
	dict_rect[(int)Tile::WALL_LVL2_2] = {3*n,  2*n, n + sx, n + sy};
	dict_rect[(int)Tile::WALL_LVL2_3] = { 5 * n,  2 * n, n + sx, n + sy };
	dict_rect[(int)Tile::WALL_LVL2_4] = { 6 * n,  2 * n, n + sx, n + sy };
	dict_rect[(int)Tile::NUMBER_BLOCK_LVL2_1] = { 8 * n,  2 * n, n + sx, n + sy };
	dict_rect[(int)Tile::NUMBER_BLOCK_LVL2_2] = { 9 * n,  2 * n, n + sx, n + sy };
	dict_rect[(int)Tile::NUMBER_BLOCK_LVL2_3] = { 11 * n,  2 * n, n + sx, n + sy };
	dict_rect[(int)Tile::NUMBER_BLOCK_LVL2_4] = { 12 * n,  2 * n, n + sx, n + sy };
	dict_rect[(int)Tile::PLAT_RIGHT_LVL2] = { 0,  2 * n, n + sx, n + sy };
	dict_rect[(int)Tile::PLAT_LEFT_LVL2] = { 0,  2 * n, n + sx, n + sy };


	dict_rect[(int)Tile::BLOCK_LVL3] = { 0,  4 * n, n + sx, n + sy };
	dict_rect[(int)Tile::PLAT_LVL3] = { 0,  4 * n, n + sx, n + sy };
	dict_rect[(int)Tile::NUMBER_BLOC_LVL3_1] = { 2 * n,  4 * n, n + sx, n + sy };
	dict_rect[(int)Tile::NUMBER_BLOC_LVL3_2] = { 3 * n,  4 * n, n + sx, n + sy };
	dict_rect[(int)Tile::NUMBER_BLOC_LVL3_3] = { 5 * n,  4 * n, n + sx, n + sy };
	dict_rect[(int)Tile::NUMBER_BLOC_LVL3_4] = { 6 * n,  4 * n, n + sx, n + sy };
	dict_rect[(int)Tile::PLAT_RIGHT_LVL3] = { 0,  4 * n, n + sx, n + sy };
	dict_rect[(int)Tile::PLAT_LEFT_LVL3] = { 0,  4 * n, n + sx, n + sy };

	dict_rect[(int)Tile::BLOCK_LVL4] = { 0,  6 * n, n + sx, n + sy };
	dict_rect[(int)Tile::PLAT_LVL4] = { 0,  6 * n, n + sx, n + sy };
	dict_rect[(int)Tile::NUMBER_BLOC_LVL4_1] = { 2 * n,  6 * n, n + sx, n + sy };
	dict_rect[(int)Tile::NUMBER_BLOC_LVL4_2] = { 3 * n,  6 * n, n + sx, n + sy };
	dict_rect[(int)Tile::NUMBER_BLOC_LVL4_3] = { 5 * n,  6 * n, n + sx, n + sy };
	dict_rect[(int)Tile::NUMBER_BLOC_LVL4_4] = { 6 * n,  6 * n, n + sx, n + sy };
	dict_rect[(int)Tile::PLAT_RIGHT_LVL4] = { 0,  6 * n, n + sx, n + sy };
	dict_rect[(int)Tile::PLAT_LEFT_LVL4] = { 0,  6 * n, n + sx, n + sy };

	dict_rect[(int)Tile::BLOCK_LVL5] = { 0,  8 * n, n + sx, n + sy };
	dict_rect[(int)Tile::PLAT_LVL5] = { 0,  8 * n, n + sx, n + sy };
	dict_rect[(int)Tile::NUMBER_BLOC_LVL5_1] = { 2 * n,  8 * n, n + sx, n + sy };
	dict_rect[(int)Tile::NUMBER_BLOC_LVL5_2] = { 3 * n,  8 * n, n + sx, n + sy };
	dict_rect[(int)Tile::NUMBER_BLOC_LVL5_3] = { 5 * n,  8 * n, n + sx, n + sy };
	dict_rect[(int)Tile::NUMBER_BLOC_LVL5_4] = { 6 * n,  8 * n, n + sx, n + sy };
	dict_rect[(int)Tile::PLAT_RIGHT_LVL5] = { 0,  8 * n, n + sx, n + sy };
	dict_rect[(int)Tile::PLAT_LEFT_LVL5] = { 0,  8 * n, n + sx, n + sy };

	dict_rect[(int)Tile::BLOCK_SR] = { 0,  10 * n, n + sx, n + sy };
	dict_rect[(int)Tile::WALL_SR_1] = { 2*n,  10 * n, n + sx, n + sy };
	dict_rect[(int)Tile::WALL_SR_2] = { 3 * n,  10 * n, n + sx, n + sy };
	dict_rect[(int)Tile::WALL_SR_3] = { 2 * n,  11 * n, n + sx, n + sy };
	dict_rect[(int)Tile::WALL_SR_4] = { 3 * n,  11 * n, n + sx, n + sy };
	dict_rect[(int)Tile::PLAT_RIGHT_SR] = { 0,  10 * n, n + sx, n + sy };
	dict_rect[(int)Tile::PLAT_LEFT_SR] = { 0,  10 * n, n + sx, n + sy };

	dict_rect[(int)Tile::FOOD_MUSHROOM] = { 0,  0, n + sx, n + sy};
	dict_rect[(int)Tile::FOOD_BANANA] = { 0,  0, n + sx, n + sy};
	dict_rect[(int)Tile::FOOD_CHERRY] = { 0,  0, n + sx, n + sy};
	dict_rect[(int)Tile::FOOD_ICE_CREAM] = { 0,  0, n + sx, n + sy};
	dict_rect[(int)Tile::FOOD_FLAM] = { 0,  0, n + sx, n + sy};
	dict_rect[(int)Tile::FOOD_CAKE] = { 0,  0, n + sx, n + sy};




}
AppStatus TileMap::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();

	if (data.LoadTexture(Resource::IMG_TILES, "images/tileset.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_tiles = data.GetTexture(Resource::IMG_TILES);

	laser = new Sprite(img_tiles);
	if (laser == nullptr)
	{
		LOG("Failed to allocate memory for laser sprite");
		return AppStatus::ERROR;
	}
	//laser->SetNumberAnimations(1);
	//laser->SetAnimationDelay(0, ANIM_DELAY);
	//laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME0]);
	//laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME1]);
	//laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME2]);
	//laser->SetAnimation(0);

	return AppStatus::OK;
}
AppStatus TileMap::Load(int data[], int w, int h)
{
	size = w*h;
	width = w;
	height = h;

	if (map != nullptr)	delete[] map;

	map = new Tile[size];
	if (map == nullptr)	
	{
		LOG("Failed to allocate memory for tile map");
		return AppStatus::ERROR;
	}
	memcpy(map, data, size * sizeof(int));

	return AppStatus::OK;
}
void TileMap::ClearObjectEntityPositions()
{
	int i;
	Tile tile;
	for (i = 0; i < size; ++i)
	{
		tile = map[i];
		if (IsTileEntity(tile) || IsTileObject(tile) || tile == Tile::EMPTY)
			map[i] = Tile::AIR;
	}
}
void TileMap::Update()
{
	laser->Update();
}
Tile TileMap::GetTileIndex(int x, int y) const
{
	int idx = x + y*width;
	if(idx < 0 || idx >= size)
	{
		LOG("Error: Index out of bounds. Tile map dimensions: %dx%d. Given index: (%d, %d)", width, height, x, y)
		return Tile::AIR;
	}
	return map[x + y * width];
}
bool TileMap::IsTileObject(Tile tile) const
{
	return Tile::OBJECT_FIRST <= tile && tile <= Tile::OBJECT_LAST;
}
bool TileMap::IsTileEntity(Tile tile) const
{
	return Tile::ENTITY_FIRST <= tile && tile <= Tile::ENTITY_LAST;
}
bool TileMap::IsTileSolid(Tile tile) const
{
	return (Tile::SOLID_FIRST <= tile && tile <= Tile::SOLID_LAST);
}
bool TileMap::IsTilePlat(Tile tile) const
{
	return (Tile::PLAT_FIRST <= tile && tile <= Tile::PLAT_LAST);
}
bool TileMap::IsTilePlatSide(Tile tile) const
{
	return (Tile::PLAT_RIGHT_FIRST <= tile && tile <= Tile::PLAT_LEFT_LAST);
}
bool TileMap::IsTilePlatRight(Tile tile) const
{
	return (Tile::PLAT_RIGHT_FIRST <= tile && tile <= Tile::PLAT_RIGHT_LAST);
}
bool TileMap::IsTilePlatLeft(Tile tile) const
{
	return (Tile::PLAT_LEFT_FIRST <= tile && tile <= Tile::PLAT_LEFT_LAST);
}
bool TileMap::TestCollisionWallLeft(const AABB& box) const
{
	return CollisionX(box.pos, box.height);
}
bool TileMap::TestCollisionWallRight(const AABB& box) const
{
	return CollisionX(box.pos + Point(box.width - 1, 0), box.height);
}
bool TileMap::TestCollisionPlatLeft(const AABB& box) const
{
	return CollisionXPlat(box.pos, box.height);
}
bool TileMap::TestCollisionPlatRight(const AABB& box) const
{
	return CollisionXPlat(box.pos + Point(box.width - 1, 0), box.height);
}
bool TileMap::TestCollisionCeiling(const AABB& box, int* py) const
{
	Point p (box.pos.x, *py - box.height);	//control point
	int tile_y;

	if (CollisionYCeiling(p, box.width))
	{
		tile_y = p.y / TILE_SIZE;

		*py = tile_y * TILE_SIZE + 1;
		return true;
	}
	return false;
}
bool TileMap::TestCollisionGround(const AABB& box, int *py) const
{
	Point p(box.pos.x, *py);	//control point
	int tile_y;

	if (CollisionY(p, box.width))
	{
		tile_y = p.y / TILE_SIZE;

		*py = tile_y * TILE_SIZE - 1;
		return true;
	}
	return false;
}
bool TileMap::TestFalling(const AABB& box) const
{
	return !CollisionY(box.pos + Point(0, box.height), box.width);
}
bool TileMap::CollisionX(const Point& p, int distance) const
{
	int x, y, y0, y1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = p.x / TILE_SIZE;
	y0 = p.y / TILE_SIZE;
	y1 = (p.y + distance - 1) / TILE_SIZE;
	
	//Iterate over the tiles within the vertical range
	for (y = y0; y <= y1; ++y)
	{
		//One solid tile is sufficient
		if (IsTileSolid(GetTileIndex(x, y)))
			return true;
	}
	return false;
}
bool TileMap::CollisionXPlat(const Point& p, int distance) const
{
	int x, y, y0, y1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = p.x / TILE_SIZE;
	y0 = p.y / TILE_SIZE;
	y1 = (p.y + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the vertical range
	for (y = y0; y <= y1; ++y)
	{
		//One solid tile is sufficient
		if (IsTilePlatSide(GetTileIndex(x, y)))
			return true;
	}
	return false;
}
bool TileMap::CollisionY(const Point& p, int distance) const
{
	int x, y, x0, x1;
	Tile tile;

	//Calculate the tile coordinates and the range of tiles to check for collision
	y = p.y / TILE_SIZE;
	x0 = p.x / TILE_SIZE;
	x1 = (p.x + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the horizontal range
	for (x = x0; x <= x1; ++x)
	{
		tile = GetTileIndex(x, y);

		//One solid or laddertop tile is sufficient
		if (IsTileSolid(tile) || IsTilePlat(tile))
			return true;
	}
	return false;
}
bool TileMap::CollisionYCeiling(const Point& p, int distance) const
{
	int x, y, x0, x1;
	Tile tile;

	//Calculate the tile coordinates and the range of tiles to check for collision
	y = (p.y + 1)/ TILE_SIZE;
	x0 = p.x / TILE_SIZE;
	x1 = (p.x + distance - 1) / TILE_SIZE;

 	//Iterate over the tiles within the horizontal range
	for (x = x0; x <= x1; ++x)
	{
		tile = GetTileIndex(x, y);

		//One solid or laddertop tile is sufficient
		if (IsTileSolid(tile) || IsTilePlat(tile))
			return true;
	}
	return false;
}
AABB TileMap::GetSweptAreaX(const AABB& hitbox) const
{
	AABB box;
	int column, x, y, y0, y1;
	bool collision;

	box.pos.y = hitbox.pos.y;
	box.height = hitbox.height;

	column = hitbox.pos.x / TILE_SIZE;
	y0 = hitbox.pos.y / TILE_SIZE;
	y1 = (hitbox.pos.y + hitbox.height - 1) / TILE_SIZE;

	//Compute left tile index
	collision = false;
	x = column - 1;
	while (!collision && x >= 0)
	{
		//Iterate over the tiles within the vertical range
		for (y = y0; y <= y1; ++y)
		{
			//One solid tile is sufficient
			if (IsTileSolid(GetTileIndex(x, y)))
			{
				collision = true;
				break;
			}
		}
		if (!collision) x--;
	}
	box.pos.x = (x + 1) * TILE_SIZE;

	//Compute right tile index
	collision = false;
	x = column + 1;
	while (!collision && x < LEVEL_WIDTH)
	{
		//Iterate over the tiles within the vertical range
		for (y = y0; y <= y1; ++y)
		{
			//One solid tile is sufficient
			if (IsTileSolid(GetTileIndex(x, y)))
			{
				collision = true;
				break;
			}
		}
		if (!collision) x++;
	}
	box.width = x * TILE_SIZE - box.pos.x;

	return box;
}
void TileMap::Render()
{
	Tile tile;
	Rectangle rc;
	Vector2 pos;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			tile = map[i * width + j];
			if (tile != Tile::AIR)
			{
				pos.x = (float)j * TILE_SIZE;
				pos.y = (float)i * TILE_SIZE;

				//if (tile != Tile::LASER)
				//{
					rc = dict_rect[(int)tile];
					DrawTextureRec(*img_tiles, rc, pos, WHITE);
				//}
				//else
				//{
				//	laser->Draw((int)pos.x, (int)pos.y);
				//}
			}
		}
	}
}
void TileMap::Release()
{
	ResourceManager& data = ResourceManager::Instance(); 
	data.ReleaseTexture(Resource::IMG_TILES);
	
	laser->Release();

	dict_rect.clear();
}