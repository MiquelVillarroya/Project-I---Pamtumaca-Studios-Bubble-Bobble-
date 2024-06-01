#include "Scene.h"
#include <stdio.h>
#include "Globals.h"
#include "MemLeaks.h"

Scene::Scene()
{
	player = nullptr;
    level = nullptr;
	enemies = nullptr;
	shots = nullptr;
	particles = nullptr;
	font = nullptr;
	objects = nullptr;
	scoreParticles = nullptr;

	//Temporary for killing the scene
	stage = 1;
	alive = true;
	playerAlive = true;
	stage1 = nullptr;
	stage2 = nullptr;
	state = SceneState::NORMAL;
	timeSpent = 0;
	totalTime = 1.5;
	levelTimer = 0;

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
	if (objects != nullptr)
	{
		objects->Release();
		delete objects;
		objects = nullptr;
	}
	if (enemies != nullptr)
	{
		enemies->Release();
		delete enemies;
		enemies = nullptr;
	}
	if (shots != nullptr)
	{
		delete shots;
		shots = nullptr;
	}
	if (font != nullptr)
	{
		delete font;
		font = nullptr;
	}
	if (scoreParticles != nullptr)
	{
		delete scoreParticles;
		scoreParticles = nullptr;
	}
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
	//Create enemy manager
	enemies = new EnemyManager();
	if (enemies == nullptr)
	{
		LOG("Failed to allocate memory for Enemy Manager");
		return AppStatus::ERROR;
	}
	//Initialise enemy manager
	if (enemies->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Enemy Manager");
		return AppStatus::ERROR;
	}

	//Create shot manager 
	shots = new ShotManager();
	if (shots == nullptr)
	{
		LOG("Failed to allocate memory for Shot Manager");
		return AppStatus::ERROR;
	}
	//Initialise shot manager
	if (shots->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Shot Manager");
		return AppStatus::ERROR;
	}

	//Create particle manager 
	particles = new ParticleManager();
	if (particles == nullptr)
	{
		LOG("Failed to allocate memory for Particle Manager");
		return AppStatus::ERROR;
	}

	//Create object manager 
	objects = new ObjectManager();
	if (objects == nullptr)
	{
		LOG("Failed to allocate memory for Particle Manager");
		return AppStatus::ERROR;
	}
	//Initialise shot manager
	if (objects->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Object Manager");
		return AppStatus::ERROR;
	}

	//Create particle manager 
	scoreParticles = new ParticleScoreManager();
	if (scoreParticles == nullptr)
	{
		LOG("Failed to allocate memory for Particle Manager");
		return AppStatus::ERROR;
	}
	//Initialise shot manager
	if (scoreParticles->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Particle Score Manager");
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
	//Assign the bubble manager reference to the enemy manager so enemies can add bubbles
	player->SetShotManager(shots);
	//Assign the bubble manager reference to the enemy manager so enemies can add shots
	enemies->SetShotManager(shots);
	//Assign the object manager reference to the enemy manager so enemies can add shots
	shots->SetObjectManager(objects);
	//Assign the shot manager reference to the enemy manager to manage enemy collisions
	shots->SetEnemyManager(enemies);
	//Assign the particle manager reference to the shot manager to add particles when shots collide
	shots->SetParticleManager(particles);
	//Assign the enemy manager reference to the player for enemy AI and tracking

	//Assign the Score Particle Manager to the objects
	objects->SetParticleScoreManager(scoreParticles);

	//Create text font
	font = new Text();
	if (font == nullptr)
	{
		LOG("Failed to allocate memory for font 1");
		return AppStatus::ERROR;
	}
	//Initialise text font
	if (font->Initialise(Resource::IMG_FONT, "images/font.png", ' ', 8) != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}

    return AppStatus::OK;
}
AppStatus Scene::LoadLevel(int stage)
{

	int size;
	int x, y, i;
	Tile tile;
	Point pos;
	int *map = nullptr;
	//Object *obj;
	AABB hitbox, area;
	Look look;

	ClearLevel();

	size = LEVEL_WIDTH * LEVEL_HEIGHT;
	if (stage == 1)
	{
		map = new int[size] {
			 1,  16,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,	  1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
			 1,  17,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0, 101,   0,   0,  62,   0,   0,   0, 101,   0,   0,   0,   0,   0,   0,	 65,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,  46,  40,   0,   0,   0,  46,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,	 34,  34,  34,  40,   0,   0,   0,  46,  40,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,  61,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	 64,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,  46,  40,   0,   0,   0,  46,  34,  34,  34,  34,  34,   34,  34,  34,  34,  34,  34,  34,  34, 34,  34,  34,  40,   0,   0,   0,  46,  40,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,  60,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	 63,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,  46,  40,   0,   0,   0,  46,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,  34,	 34,  34,  34,  40,   0,   0,   0,  46,  40,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   0, 100,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 101,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,
			 1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1
		};
		player->InitScore();
	}
	else if (stage == 2)
	{
		map = new int[size] {
			18,  19,   2,   2,   2,   2,   2,   2,   2,   0,   0,   0,   0,   2,   2,   2,   2,   2,   2,   0,   0,	  0,   0,   2,   2,   2,   2,   2,   2,   2,   8,   9,
			20,  21,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,  10,  11,
			 8,   9,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   8,   9,
			10,  11,   0,   0,   0,   0,   0,   0,   0,   0,   2,   2,   2,   0,   0,   0,   0,   0,   0,   2,   2,	  2,   0,   0,   0,   0,   0,   0,   0,   0,  10,  11,
			 8,   9,   0,   0,   0,   0,   0,   0,   2,   2,   2,   0,   2,   2,   0,   0,   0,   0,   2,   2,   0,	  2,   2,   2,   0,   0,   0,   0,   0,   0,   8,   9,
			10,  11,   0,   0,   0,   0,   0,   2,   2,   0,   0,   0,   0,   2,   2,   0,   0,   2,   2,   0,   0,	  0,   0,   2,   2,   0,   0,   0,   0,   0,  10,  11,
			 8,   9,   0,   0,   0,   0,   0,   2,   0,   0,   0,   0,   0,   0,   2,   2,   0,   2,   0,   0,   0,	  0,   0,   0,   2,   0,   0,   0,   0,   0,   8,   9,
			10,  11,   0,   0,   0,   0,   2,   2,   0,   0,   0,   0,   0,   0,   0,   2,   0,   0,   0,   0,   0,	  0,   0,   0,   2,   2,   0,  103,   0,   0,  10,  11,
			 8,   9,   0,   0,   0,   0,   2,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 103,	  0,   0,   0,   0,   2,   0,   0,   0,   0,   8,   9,
			10,  11,   0,   0,   0,   0,   2,   0,   0,   0,   0, 103,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   2,   0,   0,   0,   0,  10,  11,
			 8,   9,   0,   0,   0,   0,   2,   2,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   2,   2,   0,   0,   0,   0,   8,   9,
			10,  11,   0,   0,   0,   0,   0,   2,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   2,   0,   0,   0,   0,   0,  10,  11,
			 8,   9,   0,   0,   0,   0,   0,   2,   2,   0,   0,   0,   0,   0,   0,   0,   0,   0, 103,   0,   0,	  0,   0,   2,   2,   0,   0,   0,   0,   0,   8,   9,
			10,  11,   0,   0,   0,   0,   0,   0,   2,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   2,   0,   0,   0,   0,   0,   0,  10,  11,
			 8,   9,   0, 103,   0,   0,   0,   0,   2,   2,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   2,   2,   0,   0,   0,   0,   0,   0,   8,   9,
			10,  11,   0,   0,   0,   0,   0,   0,   0,   2,   2,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  2,   2,   0,   0,   0,   0,   0,   0,   0,  10,  11,
			 8,   9,   0,   0,   0,   0,   0,   0,   0,   0,   2,   2,   0,   0,   0,   0,   0,   0,   0,   0,   2,	  2,   0,   0,   0,   0,   0,   0,   0,   0,   8,   9,
			10,  11,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,   2,   0,   0,   0,   0,   0,   0,   2,   2,	  0,   0,   0,   0,   0,   0,   0,   0,   0,  10,  11,
			 8,   9,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,   2,   0,   0,   0,   0,  35,   2,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   8,   9,
			10,  11,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,  35,   0,   0,   0,  35,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,  10,  11,
			 8,   9,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  35,  35,   0,  35,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   8,   9,
			10,  11,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  35,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,  10,  11,
			 8,   9,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   8,   9,
			10,  11,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,  10,  11,
			 8,   9,   0, 100,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   8,   9,
			10,  11,   2,   2,   2,   2,   2,   2,   2,   0,   0,   0,   0,   2,   2,   2,   2,   2,   2,   0,   0,   0,   0,   2,   2,   2,   2,   2,   2,   2,  10,  11
				


		};
	}
	else if (stage == 3)
	{
		map = new int[size] {
			22,  23,   3,   3,   3,   3,   3,   3,   3,   0,   0,   0,   0,   3,   3,   3,   3,   3,   3,   0,   0,	  0,   0,   3,   3,   3,   3,   3,   3,   3,   3,   3,
			24,  25,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   3,
			 3,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   3,
			 3,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   3,
			 3,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   3,
			 3,   3,   0,   0,  48,  36,  36,  36,  36,  36,  36,  36,  36,  36,  42,   0,   0,  48,  36,  36,  36,	 36,  36,  36,  36,  36,  36,  42,   0,   0,   3,   3,
			 3,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   3,
			 3,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   3,
			 3,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   3,
			 3,   3,  36,  36,  36,  36,  36,  36,  42,   0,   0,  48,  36,  36,  36,  36,  36,  36,  36,  36,  42,	  0,   0,  48,  36,  36,  36,  36,  36,  36,   3,   3,
			 3,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   3,
			 3,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0, 101,   0,   3,   3,
			 3,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   3,
			 3,   3,   0,   0,  48,  36,  36,  36,  36,  36,  36,  36,  36,  36,  42,   0,   0,  48,  36,  36,  36,	 36,  36,  36,  36,  36,  36,  42,   0,   0,   3,   3,
			 3,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   3,
			 3,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   3,
			 3,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   3,
			 3,   3,  36,  36,  36,  36,  36,  36,  42,   0,   0,  48,  36,  36,  36,  36,  36,  36,  36,  36,  42,	  0,   0,  48,  36,  36,  36,  36,  36,  36,   3,   3,
			 3,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   3,
			 3,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   3,
			 3,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   3,
			 3,   3,  36,  36,  36,  36,  36,  36,   3,  36,  36,  36,  36,  36,  42,   0,   0,  48,  36,  36,  36,	 36,  36,   3,  36,  36,  36,  36,  36,  36,   3,   3,
			 3,   3,   0,   0,   0,   0,   0,   0,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   3,   0,   0,   0,   0,   0,   0,   3,   3,
			 3,   3,   0,   0,   0,   0,   0,   0,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   3,   0,   0,   0,   0,   0,   0,   3,   3,
			 3,   3,   0, 100,   0,   0,   0,   0,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   3,   0,   0,   0,   0,   0,   0,   3,   3,
			 3,   3,   3,   3,   3,   3,   3,   3,   3,   0,   0,   0,   0,   3,   3,   3,   3,   3,   3,   0,   0,   0,   0,   3,   3,   3,   3,   3,   3,   3,   3,   3
		};
		
	}
	else if (stage == 4)
	{
		map = new int[size] {
			26,  27,   4,   4,   4,   4,   4,   4,   4,   0,   0,   0,   4,   4,   4,   4,   4,   4,   4,   4,   4,	  4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
			28,  29,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   4,   4,
			 4,   4,   0,   0,   4,   0,   4,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   4,   4,   0,   0,   0,   0,   0,   0,   4,   0,   4,   0,   0,   4,   4,
			 4,   4,   0,   4,   0,   4,   0,   4,   0,   0,   0,   0,   0,   0,   0,   0,   4,   4,   0,   4,   0,	  0,   0,   0,   4,   0,   4,   0,   4,   0,   4,   4,
			 4,   4,   0,   4,   0,   0,   0,   4,   0,   0,   0,   0,   4,   4,   4,   4,   4,   4,   0,   0,   0,	  0,   0,   0,   4,   0,   0,   0,   4,   0,   4,   4,
			 4,   4,   0,   0,   4,   0,   4,   0,   0,   0,   4,   4,   4,   4,   4,   4,   4,   4,   4,   0,   0,	  0,   0,   0,   0,   4,   0,   4,   0,   0,   4,   4,
			 4,   4,   0,   0,   0,   4,   0,   0,   0,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   0,	  0,   0,   0,   0,   0,   4,   0,   0,   0,   4,   4,
			 4,   4,   0,   0,   0,   0,   0,   0,   0,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   4,   4,
			 4,   4,   0,   0,   0,   0,   0,   0,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,	  0,   0,   0,   0,   0,   0,   0, 101,   0,   4,   4,
			 4,   4,   0,   0,   0,   0,   0,   0,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   0,   4,   4,	  4,   0,   0,   0,   0,   0,   0,   0,   0,   4,   4,
			 4,   4,   0,   0,   0,   0,   0,   0,   4,   4,   4,   4,   4,   4,   4,   4,   4,   0,   4,   4,   4,	  4,   0,   0,   0,   0,   0,   0,   0,   0,   4,   4,
			 4,   4,   0,   0,   0,   0,   0,   0,   4,   0,   4,   0,   4,   4,   4,   4,   4,   0,   4,   4,   4,	  4,   4,   0,   0,   0,   0,   0,   0,   0,   4,   4,
			 4,   4,   0,   0,   0,   0,   0,   0,   4,   0,   4,   0,   4,   4,   4,   4,   4,   0,   4,   4,   4,	  4,   4,   4,   0,   0,   0,   0,   0,   0,   4,   4,
			 4,   4,  43,   0,   0,   0,   0,   0,   4,   0,   4,   0,   4,   4,   4,   4,   4,   4,   0,   4,   4,	  4,   4,   0,   0,   0,   0,   0,   0,  49,   4,   4,
			 4,   4,   0,   0,   0,   0,   0,   0,   4,   0,   4,   0,   4,   4,   4,   4,   4,   4,   4,   0,   0,	  4,   4,   4,   0,   0,   0,   0,   0,   0,   4,   4,
			 4,   4,   0,   0,   0,   0,   0,   0,   0,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   4,   4,
			 4,   4,  37,  43,   0,   0,   0,   0,   0,   0,   0,   0,   4,   4,   4,   0,   0,   4,   4,   4,   0,	  0,   0,   0,   0,   0,   0,   0,  49,  37,   4,   4,
			 4,   4,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   4,   4,   4,   0,   0,   4,   4,   4,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   4,   4,
			 4,   4,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   4,   4,   4,   0,   0,   4,   4,   4,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   4,   4,
			 4,   4,  37,  37,  43,   0,   0,   0,   0,   0,   0,   0,   4,   4,   4,   0,   0,   4,   4,   4,   0,	  0,   0,   0,   0,   0,   0,  49,  37,  37,   4,   4,
			 4,   4,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   4,   4,   4,   0,   0,   4,   4,   4,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   4,   4,
			 4,   4,   0,   0,   0,   0,   0,   0,   0,   0,  49,  37,  37,  37,  43,   0,   0,  49,  37,  37,  37,	 43,   0,   0,   0,   0,   0,   0,   0,   0,   4,   4,
			 4,   4,  37,  37,  37,  43,   0,   0,  49,  37,  37,  37,  37,  37,  43,   0,   0,  49,  37,  37,  37,	 37,  37,  43,   0,   0,  49,  37,  37,  37,   4,   4,
			 4,   4,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   4,   4,
			 4,   4,   0, 100,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,   0,   0,   4,   4,
			 4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   0,   0,   0,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4
		};
		
	}
	else
	{
		//Error level doesn't exist or incorrect level number
		LOG("Failed to load level, stage %d doesn't exist", stage);
		return AppStatus::ERROR;	
	}

	//Tile map
	level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);

	//Set the tilemaps of the managers to further assign the reference to entities (enemies and shots)
	shots->SetTileMap(level);
	enemies->SetTileMap(level);
	objects->SetObjectsTileMap(level);

	//Entities and objects
	i = 0;
	for (y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for (x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile = (Tile)map[i];
			if (level->IsTileEntity(tile) || level->IsTileObject(tile))
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				if (x <= LEVEL_WIDTH / 2) look = Look::RIGHT;
				else if (x > LEVEL_WIDTH / 2) look = Look::LEFT;

				if (tile == Tile::PLAYER)
				{
					player->SetPos(pos);
				}
				else if (tile == Tile::ZENCHAN)
				{
					pos.x += (ENEMY_FRAME_SIZE - ENEMY_PHYSICAL_WIDTH) / 2;
					hitbox = enemies->GetEnemyHitBox(pos, EnemyType::ZENCHAN);
					area = level->GetSweptAreaX(hitbox);
					enemies->Add(pos, EnemyType::ZENCHAN, area, look);
				}
				else if (tile == Tile::HIDEGONS)
				{
					pos.x += (ENEMY_FRAME_SIZE - ENEMY_PHYSICAL_WIDTH) / 2;
					hitbox = enemies->GetEnemyHitBox(pos, EnemyType::HIDEGONS);
					area = level->GetSweptAreaX(hitbox);
					enemies->Add(pos, EnemyType::HIDEGONS, area, look);
				}
				else if (tile == Tile::MONSTA)
				{
					pos.x += (ENEMY_FRAME_SIZE - ENEMY_PHYSICAL_WIDTH) / 2;
					hitbox = enemies->GetEnemyHitBox(pos, EnemyType::MONSTA);
					area = level->GetSweptAreaX(hitbox);
					enemies->Add(pos, EnemyType::MONSTA, area, look);
				}
				{
					LOG("Internal error loading scene: invalid entity or object tile id")
				}
			}
			++i;
		}
	}
	//Remove initial positions of objects and entities from the map;
	level->ClearObjectEntityPositions();

	delete[] map;

	return AppStatus::OK;
}
void Scene::Update()
{
	bool hit = false;
	Point p1, p2;
	AABB hitbox;

	//Switch between the different debug modes: off, on (sprites & hitboxes), on (hitboxes) 
	if (IsKeyPressed(KEY_F1))
	{
		debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);
	}
	//Debug levels instantly
	if (IsKeyPressed(KEY_ONE))		{LoadLevel(1); stage = 1;}
	else if (IsKeyPressed(KEY_TWO))	{LoadLevel(2); stage = 2;}
	else if (IsKeyPressed(KEY_THREE)) { LoadLevel(3); stage = 3; }
	else if (IsKeyPressed(KEY_FOUR)) { LoadLevel(4); stage = 4; }


	if (enemies->IsEmpty() && shots->NoEnemies())
	{
		if (stage != MAX_STAGE)
		{
			levelTimer += GetFrameTime();
			if (levelTimer >= 1.5)
			{
				levelTimer = 0;
				stage++;
				LoadLevel(stage);
			}
		}
		//Need to rework scene kill
		else {
			alive = false;
		}
	}

	Point mousePos;
	mousePos.x = GetMouseX();
	mousePos.y = GetMouseY();

	//Debug enemies
	AABB area;
	Look look;
	if (IsKeyPressed(KEY_SIX)) {
		if (mousePos.x <= LEVEL_WIDTH / 2) look = Look::RIGHT;
		else if (mousePos.x > LEVEL_WIDTH / 2) look = Look::LEFT;
		hitbox = enemies->GetEnemyHitBox(mousePos, EnemyType::ZENCHAN);
		area = level->GetSweptAreaX(hitbox);
		enemies->Add(mousePos, EnemyType::ZENCHAN, area, look);
	}
	else if (IsKeyPressed(KEY_SEVEN))
	{
		if (mousePos.x <= LEVEL_WIDTH / 2) look = Look::RIGHT;
		else if (mousePos.x > LEVEL_WIDTH / 2) look = Look::LEFT;
		hitbox = enemies->GetEnemyHitBox(mousePos, EnemyType::MONSTA);
		area = level->GetSweptAreaX(hitbox);
		enemies->Add(mousePos, EnemyType::MONSTA, area, look);
	}

	//Debug items


	if (IsKeyPressed(KEY_Q)) 		objects->Add(mousePos, ObjectType::MUSHROOM);
	else if (IsKeyPressed(KEY_W))	objects->Add(mousePos, ObjectType::BANANA);
	else if (IsKeyPressed(KEY_E))	objects->Add(mousePos, ObjectType::CHERRY);
	else if (IsKeyPressed(KEY_R))	objects->Add(mousePos, ObjectType::ICE_CREAM);
	else if (IsKeyPressed(KEY_T))	objects->Add(mousePos, ObjectType::FLAM);
	else if (IsKeyPressed(KEY_Y))	objects->Add(mousePos, ObjectType::CAKE);
	else if (IsKeyPressed(KEY_U))	objects->Add(mousePos, ObjectType::YELLOW_CANDY);
	else if (IsKeyPressed(KEY_I))	objects->Add(mousePos, ObjectType::SHOES);

	//Level, player
	level->Update();
	player->Update();

	//Bubbles & Enemies & Objects
	hitbox = player->GetHitbox();
	enemies->Update(hitbox, hit);
	shots->Update(hitbox);
	int score = objects->Update(hitbox);
	particles->Update();
	scoreParticles->Update();

	//Update score
	if (score > 0)
	{
		player->IncrScore(score);
	}
	else if (score == -1) //Candy
	{
		player->Candy();
	}
	else if (score == -2) //Shoes
	{
		player->Shoes();
	}

	if (hit) player->MinusLife();
}
void Scene::Render()
{
	BeginMode2D(camera);

    level->Render();
	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES)
	{
		enemies->Draw();
		player->Draw();
		shots->Draw();
		objects->Draw();
	}
	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
	{
		enemies->DrawDebug();
		player->DrawDebug(GREEN);
		shots->DrawDebug(GRAY);
		objects->DrawDebug(ORANGE);
	}
	particles->Draw();
	scoreParticles->Draw();

	EndMode2D();

	RenderGUI();
}
void Scene::Release()
{
	level->Release();
	player->Release();
	ClearLevel();
}
void Scene::ClearLevel()
{
	objects->Release();
	enemies->Release();
	shots->Release();
}
void Scene::RenderGUI() const
{
	if (player->GetGod())
	{
		font->Draw(90, 0, TextFormat("GOD MODE"), YELLOW);
		font->Draw(110, 8, TextFormat("ON"), YELLOW);
	}
	font->Draw(20, 0, TextFormat("1UP"), GREEN);
	font->Draw(20, 8, TextFormat("%d", player->GetScore()), WHITE);
	font->Draw(180, 0, TextFormat("2UP"), BLUE);
	font->Draw(190, 8, TextFormat("%d", 0), WHITE);

	//int x_offset = 0;
	//for (int i = 0; i < player->GetLives(); ++i)
	//{		
	//	font->Draw(x_offset, 8, TextFormat("", 0), WHITE);
	//	x_offset += 8;
	//}

}
bool Scene::IsPlayerAlive()
{
	if (player->GetLives() > 0) return true;
	return false;
}
bool Scene::IsSceneAlive()
{
	return alive;
}
