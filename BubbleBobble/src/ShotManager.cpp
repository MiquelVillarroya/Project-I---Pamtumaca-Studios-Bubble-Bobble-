#include "ShotManager.h"
#include "Bubble.h"
#include "PlayerBubble.h"

ShotManager::ShotManager()
{
	enemies = nullptr;
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
void ShotManager::SetEnemyManager(EnemyManager* enemies)
{
	this->enemies = enemies;
}
void ShotManager::SetTileMap(TileMap* tilemap)
{
	for (Shot* shot : shots)
	{
		shot->SetTileMap(tilemap);
	}
	for (Shot* shot : playerBubbles)
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
	else if (type == ShotType::PLAYER_BUBBLE)
	{
		shot = new PlayerBubble(pos, dir, BUBBLE_PHYSICAL_WIDTH, BUBBLE_PHYSICAL_HEIGHT, BUBBLE_FRAME_SIZE, BUBBLE_FRAME_SIZE);
	}
	else
	{
		LOG("Internal error; trying to add a new shot with invalid type");
		return;
	}

	shot->Initialise();
	if (type == ShotType::PLAYER_BUBBLE)
	{
		playerBubbles.push_back(shot);
	}
	else {
		shots.push_back(shot);
	}
}
void ShotManager::Update(const AABB& temp_hitbox)
{
	AABB box;
	bool playerHit, bubbleEnd, enemyHit;

	for (Shot* shot : playerBubbles)
	{
		if (shot->IsAlive())
		{
			bubbleEnd = false;
			//Update position
			bubbleEnd = shot->Update();

			//Check Enemy Collision
			enemyHit = false;
			box = shot->GetHitbox();
			if (!bubbleEnd) enemyHit = enemies->CheckBubbleCollisions(box);

			//Update according bubble logic (enemy/wall)
			if (bubbleEnd || enemyHit)
			{
				Point p, d;
				p = shot->GetPos();
				d = shot->GetDir();
				shot->SetAlive(false);
				if (bubbleEnd)		Add(p, d, ShotType::BUBBLE);
				else if (enemyHit)	Add(p, d, ShotType::BUBBLE);
			}
		}
	}

	for (Shot* shot : shots)
	{
		if (shot->IsAlive())
		{
			//Update position
 			shot->Update(temp_hitbox);

			//Check player collision
			playerHit = false;
			box = shot->GetHitbox();
			if (!playerHit) playerHit = box.TestAABB(temp_hitbox);

			//if (hit) shot->SetAlive(false);
		}
	}
}
void ShotManager::Draw() const
{
	//Iterate over the shots vector by reference instead of by value to avoid a copy
	//of each Entity object
	for (const Shot* shot : shots)
 		if (shot->IsAlive())	shot->Draw();
	for (const Shot* shot : playerBubbles)
 		if (shot->IsAlive())	shot->Draw();
}
void ShotManager::DrawDebug(const Color& col) const
{
	for (const Shot* shot : shots)
		if (shot->IsAlive()) shot->DrawHitbox(col);
	for (const Shot* shot : playerBubbles)
		if (shot->IsAlive()) shot->DrawHitbox(col);
}
void ShotManager::Release()
{
	for (Shot* shot : shots)
		delete shot;
	shots.clear();
	for (Shot* shot : playerBubbles)
		delete shot;
	playerBubbles.clear();
}
