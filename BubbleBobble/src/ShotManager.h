#pragma once
#include "Shot.h"

class ShotManager
{
public:
	ShotManager();
	~ShotManager();

	AppStatus Initialise();

	//Set the TileMap reference for each shot for managing shot collisions
	void SetTileMap(TileMap* tilemap);

	//Add a new shot with the given position and direction
	void Add(const Point& pos, const Point& dir, ShotType type);

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

	//Reference to the TileMap object
	//This class does not own the object, it only holds a reference to it
};

