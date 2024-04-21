#include "Enemy.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Enemy::Enemy(const Point&p, EnemyState s, EnemyLook l) :
	Entity (p, ENEMY_PHYSICAL_WIDTH, ENEMY_PHYSICAL_HEIGTH, ENEMY_FRAME_SIZE, ENEMY_FRAME_SIZE)
{
	state = s;
	look = l;
	angryTimer = 0;
	map = nullptr;
}
Enemy::~Enemy()
{
}
AppStatus Enemy::Initialise()
{
	int i;
	const int n = ENEMY_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_ZENCHAN, "images/zenchan.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(Resource::IMG_ZENCHAN));
	if (render == nullptr)
	{
		LOG("failed to allocate memory for zenchan sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)ZenchanAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)ZenchanAnim::WALK_RIGHT, ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)ZenchanAnim::WALK_RIGHT, { (float)i * n, 0, -n, n });
	sprite->SetAnimationDelay((int)ZenchanAnim::WALK_LEFT, ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)ZenchanAnim::WALK_LEFT, { (float)i * n, 0, n, n });

	if (look == EnemyLook::LEFT) sprite->SetAnimation((int)ZenchanAnim::WALK_LEFT);
	if (look == EnemyLook::RIGHT) sprite->SetAnimation((int)ZenchanAnim::WALK_RIGHT);

	return AppStatus::OK;
}
void Enemy::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
void Enemy::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
ZenchanAnim Enemy::GetAnimation() {
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (ZenchanAnim)sprite->GetAnimation();
}
void Enemy::Update()
{
	MoveX();
	MoveY();

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void Enemy::MoveX()
{
	AABB box;
	int prev_x = pos.x;
	box = GetHitbox();
	if (look == EnemyLook::RIGHT)
	{
		pos.x += ENEMY_SPEED;
		if (map->TestCollisionWallRight(box))
		{
			pos.x = prev_x;
			look = EnemyLook::LEFT;
			SetAnimation((int)ZenchanAnim::WALK_LEFT);
		}

	}
	else if (look == EnemyLook::LEFT)
	{
		pos.x += -ENEMY_SPEED;
		if (map->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			look = EnemyLook::RIGHT;
			SetAnimation((int)ZenchanAnim::WALK_RIGHT);
		}

	}
}
void Enemy::MoveY()
{

}
void Enemy::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
void Enemy::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_ZENCHAN);

	render->Release();
}