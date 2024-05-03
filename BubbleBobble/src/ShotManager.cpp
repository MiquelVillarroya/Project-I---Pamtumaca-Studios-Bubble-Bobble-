#include "ShotManager.h"
#include "Bubble.h"

ShotManager::ShotManager()
{
}
ShotManager::~ShotManager()
{
	Release();
}
AppStatus ShotManager::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_BUBBLE, "images/bubbles.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	return AppStatus::OK;
}
void ShotManager::SetTileMap(TileMap* tilemap)
{
	for (Shot* shot : shots)
	{
		shot->SetTileMap(tilemap);
	}
}
void ShotManager::Add(const Point& pos, const Point& dir, ShotType type)
{
	Shot* shot;

	if (type == ShotType::BUBBLE)
	{
		shot = new Bubble(pos, dir, BUBBLE_PHYSICAL_WIDTH, BUBBLE_PHYSICAL_HEIGHT, BUBBLE_FRAME_SIZE, BUBBLE_FRAME_SIZE);
	}
	else if (type == ShotType::FIREBALL)
	{
		shot = new Bubble(pos, dir, BUBBLE_PHYSICAL_WIDTH, BUBBLE_PHYSICAL_HEIGHT, BUBBLE_FRAME_SIZE, BUBBLE_FRAME_SIZE);
	}
	else
	{
		LOG("Internal error; trying to add a new shot with invalid type");
		return;
	}

	shot->Initialise();
	shots.push_back(shot);
}
void ShotManager::Update(const AABB& temp_hitbox)
{
	AABB box;
	bool hit;

	for (Shot* shot : shots)
	{
		if (shot->IsAlive())
		{
			//Update position
 			shot->Update(temp_hitbox);
			shot->CheckEnemyCollision

			//Check player collision
			hit = false;
			if (!hit) hit = box.TestAABB(temp_hitbox);

			if (hit) shot->SetAlive(false);
		}
	}
}
void ShotManager::Draw() const
{
	//Iterate over the shots vector by reference instead of by value to avoid a copy
	//of each Entity object
	for (const Shot* shot : shots)
		if (shot->IsAlive())	shot->Draw();
}
void ShotManager::DrawDebug(const Color& col) const
{
	for (const Shot* shot : shots)
		if (shot->IsAlive()) shot->DrawHitbox(col);
}
void ShotManager::Release()
{
	for (Shot* shot : shots)
		delete shot;
	shots.clear();
}
