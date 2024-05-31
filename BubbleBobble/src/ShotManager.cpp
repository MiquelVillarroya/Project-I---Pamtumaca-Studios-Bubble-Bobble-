#include "ShotManager.h"
#include "Bubble.h"
#include "PlayerBubble.h"

ShotManager::ShotManager()
{
	enemies = nullptr;
	map = nullptr;
	particles = nullptr;
	objects = nullptr;
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
void ShotManager::SetObjectManager(ObjectManager* objects)
{
	this->objects = objects;
}
void ShotManager::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
void ShotManager::SetParticleManager(ParticleManager* particles)
{
	this->particles = particles;
}
void ShotManager::Add(const Point& pos, const Point& dir, ShotType type, EnemyType enemyType)
{
	Shot* shot;

	if (type == ShotType::BUBBLE)
	{
		shot = new Bubble(pos, dir, BUBBLE_PHYSICAL_WIDTH, BUBBLE_PHYSICAL_HEIGHT, BUBBLE_FRAME_SIZE, BUBBLE_FRAME_SIZE, enemyType);
	}
	else if (type == ShotType::FIREBALL)
	{
		shot = new Bubble(pos, dir, BUBBLE_PHYSICAL_WIDTH, BUBBLE_PHYSICAL_HEIGHT, BUBBLE_FRAME_SIZE, BUBBLE_FRAME_SIZE, enemyType);
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
	shot->SetTileMap(map);
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
	EnemyType enemyHit;
	bool playerHit, bubbleHit;

	for (Shot* shot : playerBubbles)
	{
		if (shot->IsAlive())
		{
			bubbleHit = false;
			//Update position, return true if bubble ends shot or finds collision
			bubbleHit = shot->Update();

			//Check Enemy Collision
			enemyHit = EnemyType::NONE;
			box = shot->GetHitbox();
			if (!bubbleHit) enemyHit = enemies->CheckBubbleCollisions(box);

			//Update according bubble logic (enemy/wall)
			if (bubbleHit || enemyHit != EnemyType::NONE)
			{
				Point p, d;
				p = shot->GetPos();
				d = shot->GetDir();
				shot->SetAlive(false);
				if (bubbleHit)		Add(p, d, ShotType::BUBBLE);
				else if (enemyHit != EnemyType::NONE)
				{
					Add(p, d, ShotType::BUBBLE, enemyHit);
				}
			}
		}
	}

	for (Shot* shot : shots)
	{
		if (shot->IsAlive())
		{
			enemyHit = EnemyType::NONE;

			//Update position
 			enemyHit = shot->Update(temp_hitbox);

			//Check player collision
			playerHit = false;
			box = shot->GetHitbox();
			playerHit = box.TestAABB(temp_hitbox);

			//Add enemy
			if (enemyHit != EnemyType::_NULL)
			{
				Point p;

				if (enemyHit != EnemyType::NONE)
				{
					Look look = Look::LEFT;
					AABB enemy_box, area;

					p = shot->GetPos();
					enemy_box = enemies->GetEnemyHitBox(p, enemyHit);
					area = map->GetSweptAreaX(enemy_box);
					if (p.x <= LEVEL_WIDTH / 2) look = Look::RIGHT;
					else if (p.y > LEVEL_WIDTH / 2) look = Look::LEFT;

					enemies->Add(p, enemyHit, area, look);
				}

				shot->SetAlive(false);
				p.x = box.pos.x;
				p.y = box.pos.y;
				particles->Add(p);
			}
			else if (playerHit)
			{
				shot->SetAlive(false);
				Point p;
				p.x = box.pos.x;
				p.y = box.pos.y;
				particles->Add(p);

				if (shot->GetEnemy() != EnemyType::_NULL && shot->GetEnemy() != EnemyType::NONE)
				{
					int rand;
					rand = GetRandomValue(0,4);
					objects->Add(p, (ObjectType)rand);
				}
			}
		}
	}
}
int ShotManager::NoEnemies() const
{
	for (Shot* shot : shots)
	{
		if (shot->GetEnemy() != EnemyType::_NULL && shot->GetEnemy() != EnemyType::NONE)
		{
			if (shot->IsAlive())
				return false;
		}
	}
	return true;
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
