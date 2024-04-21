#include "Scene.h"
#include <stdio.h>
#include "Globals.h"

Scene::Scene()
{
	player = nullptr;
    level = nullptr;
	
	camera.target = { 0, 0 };				//Center of the screen
	camera.offset = { 0, MARGIN_GUI_Y };	//Offset from the target (center of the screen)
	camera.rotation = 0.0f;					//No rotation
	camera.zoom = 1.0f;						//Default zoom

	debug = DebugMode::OFF;
}
Scene::~Scene()
{
	if (player != nullptr)
	{
		player->Release();
		delete player;
		player = nullptr;
	}
    if (level != nullptr)
    {
		level->Release();
        delete level;
        level = nullptr;
    }
	for (Entity* obj : objects)
	{
		delete obj;
	}
	objects.clear();
	for (Enemy* enemy : enemies)
	{
		delete enemy;
	}
	enemies.clear();
}
AppStatus Scene::Init()
{
	//Create player
	player = new Player({ 0,0 }, State::IDLE, Look::RIGHT);
	if (player == nullptr)
	{
		LOG("Failed to allocate memory for Player");
		return AppStatus::ERROR;
	}
	//Initialise player
	if (player->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Player");
		return AppStatus::ERROR;
	}

	//Create level 
    level = new TileMap();
    if (level == nullptr)
    {
        LOG("Failed to allocate memory for Level");
        return AppStatus::ERROR;
    }
	//Initialise level
	if (level->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}
	//Load level
	if (LoadLevel(1) != AppStatus::OK)
	{
		LOG("Failed to load Level 1");
		return AppStatus::ERROR;
	}
	//Assign the tile map reference to the player to check collisions while navigating
	player->SetTileMap(level);

    return AppStatus::OK;
}
AppStatus Scene::LoadLevel(int stage)
{
	int size;
	int x, y, i;
	Tile tile;
	Point pos;
	int *map = nullptr;
	Object *obj;
	Enemy *enemy;
	
	ClearLevel();

	size = LEVEL_WIDTH * LEVEL_HEIGHT;
	if (stage == 1)
	{
		map = new int[size] {
			 1,   7,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
			 1,   8,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 101,   0,   0, 102,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   9,   9,   0,   0,   0,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,	  9,   9,   9,   9,   0,   0,   0,   9,   9,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   9,   9,   0,   0,   0,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,	  9,   9,   9,   9,   0,   0,   0,   9,   9,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   9,   9,   0,   0,   0,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,   9,	  9,   9,   9,   9,   0,   0,   0,   9,   9,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0, 100,   0,   0,   0,  50,   0,  51,   0,  52,   0,  53,   0,  54,   0,  55,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1
		};
		player->InitScore();
	}
	else if (stage == 2)
	{
		map = new int[size] {
			 3,   4,   2,   2,   2,   2,   2,   2,   2,   0,   0,   0,   0,   2,   2,   2,   2,   2,   2,   0,   0,	  0,   0,   2,   2,   2,   2,   2,   2,   2,   3,   4,
			 5,   6,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   5,   6,
			 3,   4,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   4,
			 5,   6,   0,   0,   0,   0,   0,   0,   0,   0,  10,  10,  10,   0,   0,   0,   0,   0,   0,  10,  10,	 10,   0,   0,   0,   0,   0,   0,   0,   0,   5,   6,
			 3,   4,   0,   0,   0,   0,   0,   0,  10,  10,  10,   0,  10,  10,   0,   0,   0,   0,  10,  10,   0,	 10,  10,  10,   0,   0,   0,   0,   0,   0,   3,   4,
			 5,   6,   0,   0,   0,   0,   0,  10,  10,   0,   0,   0,   0,  10,  10,   0,   0,  10,  10,   0,   0,	  0,   0,  10,  10,   0,   0,   0,   0,   0,   5,   6,
			 3,   4,   0,   0,   0,   0,   0,  10,   0,   0,   0,   0,   0,   0,  10,  10,   0,  10,   0,   0,   0,	  0,   0,   0,  10,   0,   0,   0,   0,   0,   3,   4,
			 5,   6,   0,   0,   0,   0,  10,  10,   0,   0,   0,   0,   0,   0,   0,  10,   0,   0,   0,   0,   0,	  0,   0,   0,  10,  10,   0,   0,   0,   0,   5,   6,
			 3,   4,   0,   0,   0,   0,  10,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,  10,   0,   0,   0,   0,   3,   4,
			 5,   6,   0,   0,   0,   0,  10,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,  10,   0,   0,   0,   0,   5,   6,
			 3,   4,   0,   0,   0,   0,  10,  10,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,  10,  10,   0,   0,   0,   0,   3,   4,
			 5,   6,   0,   0,   0,   0,   0,  10,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,  10,   0,   0,   0,   0,   0,   5,   6,
			 3,   4,   0,   0,   0,   0,   0,  10,  10,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,  10,  10,   0,   0,   0,   0,   0,   3,   4,
			 5,   6,   0,   0,   0,   0,   0,   0,  10,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,  10,   0,   0,   0,   0,   0,   0,   5,   6,
			 3,   4,   0,   0,   0,   0,   0,   0,  10,  10,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,  10,  10,   0,   0,   0,   0,   0,   0,   3,   4,
			 5,   6,   0,   0,   0,   0,   0,   0,   0,  10,  10,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	 10,  10,   0,   0,   0,   0,   0,   0,   0,   5,   6,
			 3,   4,   0,   0,   0,   0,   0,   0,   0,   0,  10,  10,   0,   0,   0,   0,   0,   0,   0,   0,  10,	 10,   0,   0,   0,   0,   0,   0,   0,   0,   3,   4,
			 5,   6,   0,   0,   0,   0,   0,   0,   0,   0,   0,  10,  10,   0,   0,   0,   0,   0,   0,  10,  10,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   5,   6,
			 3,   4,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  10,  10,   0,   0,   0,   0,  10,  10,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   4,
			 5,   6,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  10,  10,   0,   0,   0,  10,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   5,   6,
			 3,   4,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  10,  10,   0,  10,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   4,
			 5,   6,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  10,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   5,   6,
			 3,   4,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   4,
			 5,   6,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   5,   6,
			 3,   4,   0, 100,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   4,
			 5,   6,   2,   2,   2,   2,   2,   2,   2,   0,   0,   0,   0,   2,   2,   2,   2,   2,   2,   0,   0,   0,   0,   2,   2,   2,   2,   2,   2,   2,   5,   6






		};
	}
	else
	{
		//Error level doesn't exist or incorrect level number
		LOG("Failed to load level, stage %d doesn't exist", stage);
		return AppStatus::ERROR;	
	}

	//Entities and objects
	i = 0;
	for (y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for (x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile = (Tile)map[i];
			if (tile == Tile::EMPTY)
			{
				map[i] = 0;
			}
			else if (tile == Tile::PLAYER)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				player->SetPos(pos);
				map[i] = 0;
			}
			else if (tile == Tile::FOOD_MUSHROOM)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::MUSHROOM);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::FOOD_BANANA)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::BANANA);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::FOOD_CHERRY)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::CHERRY);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::FOOD_ICE_CREAM)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::ICE_CREAM);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::FOOD_FLAM)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::FLAM);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::FOOD_CAKE)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::CAKE);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::ZENCHAN_LEFT)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				enemy = new Enemy(pos, EnemyState::ANGRY, EnemyLook::LEFT);
				enemy->Initialise();
				enemy->SetTileMap(level);
				enemies.push_back(enemy);
				map[i] = 0;
			}
			else if (tile == Tile::ZENCHAN_RIGHT)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				enemy = new Enemy(pos, EnemyState::ANGRY, EnemyLook::RIGHT);
				enemy->Initialise();
				enemy->SetTileMap(level);
				enemies.push_back(enemy);
				map[i] = 0;
			}
			++i;
		}
	}
	//Tile map
	level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);
	delete[] map;

	return AppStatus::OK;
}
void Scene::Update()
{
	Point p1, p2;
	AABB box;

	//Switch between the different debug modes: off, on (sprites & hitboxes), on (hitboxes) 
	if (IsKeyPressed(KEY_F1))
	{
		debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);
	}
	//Debug levels instantly
	if (IsKeyPressed(KEY_ONE))		LoadLevel(1);
	else if (IsKeyPressed(KEY_TWO))	LoadLevel(2);

	level->Update();
	player->Update();
	auto it = enemies.begin();
	while (it != enemies.end())
	{
		(*it)->Update();
		++it;
	}
	CheckCollisions();
}
void Scene::Render()
{
	BeginMode2D(camera);

    level->Render();
	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES)
	{
		RenderObjects();
		player->Draw();
		player->DrawBubbles();
		if (player->GetGod()) player->DrawGod(GREEN);
		RenderEnemies();
	}
	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
	{
		RenderObjectsDebug(YELLOW);
		player->DrawDebug(GREEN);
		player->DrawBubblesDebug(PURPLE);
		if (player->GetGod()) player->DrawGod(GREEN);
		RenderEnemiesDebug(RED);
	}

	EndMode2D();

	RenderGUI();
}
void Scene::Release()
{
    level->Release();
	player->Release();
	ClearLevel();
}
void Scene::CheckCollisions()
{
	AABB player_box, obj_box, enemy_box, bubl_box;
	
	player_box = player->GetHitbox();
	auto itObj = objects.begin();
	while (itObj != objects.end())
	{
		obj_box = (*itObj)->GetHitbox();
		if(player_box.TestAABB(obj_box))
		{
			player->IncrScore((*itObj)->Points());
			
			//Delete the object
			delete* itObj; 
			//Erase the object from the vector and get the itObjerator to the next valid element
			itObj = objects.erase(itObj); 
		}
		else
		{
			//Move to the next object
			++itObj; 
		}
	}
	auto itEnem = enemies.begin();
	while ( itEnem != enemies.end())
	{
		enemy_box = (*itEnem)->GetHitbox();
		if (player_box.TestAABB(enemy_box) && player->GetState() != State::DEAD)  
		{
			player->MinusLife();
			itEnem++;
		}
		else if (player->CheckBubbleCollision(enemy_box))
		{
			delete* itEnem;
			itEnem = enemies.erase(itEnem);
		}
		else {
			itEnem++;
		}
	}
}
void Scene::ClearLevel()
{
	for (Object* obj : objects)
	{
		delete obj;
	}
	objects.clear();
	for (Enemy* enemy : enemies)
	{
		delete enemy;
	}
	enemies.clear();
	player->ClearBubbles();
}
void Scene::RenderObjects() const
{
	for (Object* obj : objects)
	{
		obj->Draw();
	}
}
void Scene::RenderObjectsDebug(const Color& col) const
{
	for (Object* obj : objects)
	{
		obj->DrawDebug(col);
	}
}
void Scene::RenderEnemies()
{
	for (Enemy* enemy : enemies)
	{
		enemy->Draw();
	}
}
void Scene::RenderEnemiesDebug(const Color& col) const
{
	for (Enemy* enemy : enemies)
	{
		enemy->DrawDebug(col);
	}
}
void Scene::RenderGUI() const
{
	//Temporal approach
	DrawText(TextFormat("SCORE : %d", player->GetScore()), 10, 5, 8, LIGHTGRAY);
	DrawText(TextFormat("LIVES : %d", player->GetLives()), 200, 5, 8, LIGHTGRAY);
}