#include "EnemyManager.h"
#include "Zenchan.h"
#include "Hidegons.h"
#include "Monsta.h"
#include "Player.h"

EnemyManager::EnemyManager()
{
	shots = nullptr;
	map = nullptr;
}
EnemyManager::~EnemyManager()
{
	Release();
}
AppStatus EnemyManager::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_ZENCHAN, "images/zenchan.png") != AppStatus::OK)
	{
		LOG("Failed to load zenchan sprite texture");
		return AppStatus::ERROR;
	}
	if (data.LoadTexture(Resource::IMG_HIDEGONS, "images/hidegons.png") != AppStatus::OK)
	{
		LOG("Failed to load hidegons sprite texture");
		return AppStatus::ERROR;
	}
	if (data.LoadTexture(Resource::IMG_MONSTA, "images/monsta_SkelMonsta.png") != AppStatus::OK)
	{
		LOG("Failed to load monsta/skelMonsta sprite texture");
		return AppStatus::ERROR;
	}
	
	return AppStatus::OK;
}
void EnemyManager::SetShotManager(ShotManager* shots)
{
	this->shots = shots;
}
void EnemyManager::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
void EnemyManager::Add(const Point& pos, EnemyType type, const AABB& area, Look look)
{
	Enemy* enemy;

	if (type == EnemyType::ZENCHAN)
	{
		enemy = new Zenchan(pos, ENEMY_PHYSICAL_WIDTH, ENEMY_PHYSICAL_HEIGHT, ENEMY_FRAME_SIZE, ENEMY_FRAME_SIZE);
	}
	else if (type == EnemyType::HIDEGONS)
	{
		enemy = new Zenchan(pos, ENEMY_PHYSICAL_WIDTH, ENEMY_PHYSICAL_HEIGHT, ENEMY_FRAME_SIZE, ENEMY_FRAME_SIZE);
	}
	else if (type == EnemyType::MONSTA)
	{
 		enemy = new Monsta(pos, ENEMY_PHYSICAL_WIDTH, ENEMY_PHYSICAL_HEIGHT, ENEMY_FRAME_SIZE, ENEMY_FRAME_SIZE);
	}
	else
	{
		LOG("Internal error: trying to add a new enemy with invalid type");
		return;
	}

	enemy->Initialise(look, area);
	enemy->SetTileMap(map);
	enemies.push_back(enemy);
}
AABB EnemyManager::GetEnemyHitBox(const Point& pos, EnemyType type) const
{
	int width, height;
	if (type == EnemyType::ZENCHAN)
	{
		width = ENEMY_PHYSICAL_WIDTH;
		height = ENEMY_PHYSICAL_WIDTH;
	}
	else if (type == EnemyType::HIDEGONS)
	{
		width = ENEMY_PHYSICAL_WIDTH;
		height = ENEMY_PHYSICAL_WIDTH;
	}
	else
	{
		LOG("Internal error while computing hitbox for an invalid enemy type");
		return {};
	}
	Point p(pos.x, pos.y - (height - 1));
	AABB hitbox(p, width, height);
	return hitbox;
}
bool EnemyManager::IsEmpty() const
{
	if (enemies.empty()) return true;
	return false;
}
void EnemyManager::Update(const AABB& player_hitbox, bool& hit)
{
	bool shoot;
	Point p, d;
	AABB box;
	hit = false;

	for (Enemy* enemy : enemies)
	{
		if (enemy->IsAlive()) {
			shoot = enemy->Update(player_hitbox);
			if (shoot)
			{
				enemy->GetShootingPosDir(&p, &d);
				shots->Add(p, d, ShotType::BUBBLE);
			}
			box = enemy->GetHitbox();
			if (!hit) hit = box.TestAABB(player_hitbox);
		}
	}
}
EnemyType EnemyManager::CheckBubbleCollisions(const AABB& bubble_hitbox)
{
	AABB box;
	EnemyType type = EnemyType::NONE;
	bool hit = false;


	for (Enemy* enemy : enemies)
	{
		if (enemy->IsAlive()) {
			hit = false;
			box = enemy->GetHitbox();
			hit = box.TestAABB(bubble_hitbox);

			if (hit)
			{
				enemy->SetAlive(false);
				type = enemy->GetEnemyType();
				return type;
			}
		}
	}

	return type;
}
void EnemyManager::Draw() const
{
	for (const Enemy* enemy : enemies)
		if (enemy->IsAlive())	enemy->Draw();
}
void EnemyManager::DrawDebug() const
{
	for (const Enemy* enemy : enemies)
	{
		if (enemy->IsAlive()) {
			enemy->DrawVisibilityArea(DARKGRAY);
			enemy->DrawHitbox(RED);
		}
	}
}
void EnemyManager::Release()
{
	for (Enemy* enemy : enemies)
		delete enemy;
	enemies.clear();
}