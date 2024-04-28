#pragma once
#include "Enemy.h"
#include "ShotManager.h"

class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();

	AppStatus Initialise();

	//Set the ShotManager reference for managing enemy shots
	void SetShotManager(ShotManager* shots);
	//Set the tilemap for each enemy to manage collisions and logic
	void SetTileMap(TileMap* tilemap);

	//Add a new enemy with the given position, type, action area and looking direction
	void Add(const Point& pos, EnemyType type, const AABB& area, Look look = Look::RIGHT);

	//Retrieve the hitbox of an enemy based on the given position and type
	AABB GetEnemyHitBox(const Point& pos, EnemyType type) const;
	bool IsEmpty() const;

	//Update enemies according to their logic. If the given player hitbox is visible to them,
	//they will shoot by adding shots to the ShotManager
	void Update(const AABB& player_hitbox, bool& hit);

	//Draw all enemies
	void Draw() const;

	//Draw enemy hitboxes for debugging
	void DrawDebug() const;

	//Delete all enemies and clear the enemy vector
	void Release();

private:
	std::vector<Enemy*> enemies;

	//Reference to the ShotManager object
	//This class does not own the object, it only holds a reference to it
	ShotManager* shots;
};
