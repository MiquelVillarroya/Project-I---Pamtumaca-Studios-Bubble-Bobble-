#pragma once
#include "Shot.h"
#include "EnemyManager.h"
#include "ParticleManager.h"
#include "ObjectManager.h"
#include <array>

class EnemyManager;

class ShotManager
{
public:
	ShotManager();
	~ShotManager();

	AppStatus Initialise();

	//Set the EnemyManager reference for managing enemy shots
	void SetEnemyManager(EnemyManager* enemies);

	//Set the TileMap reference for each shot for managing shot collisions
	void SetTileMap(TileMap* tilemap);
	//Set the TileMap reference for the Particle Manager
	void SetParticleManager(ParticleManager* particles);
	//Set the objects for each enemy to manage collisions and logic
	void SetObjectManager(ObjectManager* objects);

	//Add a new shot with the given position and direction
	void Add(const Point& pos, const Point& dir, ShotType type, EnemyType enemyType = EnemyType::NONE);

	//Update shot positions and check for collisions with the level and player
	void Update(const AABB& player_hitbox);

	//Draw all active shots
	void Draw() const;

	//Draw hitboxes of shots for debugging
	void DrawDebug(const Color& col) const;

	//Delete all shots and clear the shots vector
	void Release();

private:
	std::vector<Shot*> shots;
	std::vector<Shot*> playerBubbles;

	//Reference to the EnemyManager object
	//This class does not own the object, it only holds a reference to it
	EnemyManager* enemies;
	TileMap* map;
	ParticleManager* particles;
	ObjectManager* objects;

};


