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
	bubbleTimer = 0;
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

	sprite->SetAnimationDelay((int)ZenchanAnim::BUBBLE, ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)ZenchanAnim::BUBBLE, { (float)i*n, 3*n, n, n });
	sprite->SetAnimationDelay((int)ZenchanAnim::RED_START, ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)ZenchanAnim::RED_START, { 6 + (float)i*n, 3*n, n, n });
	sprite->SetAnimationDelay((int)ZenchanAnim::RED_BLINK, ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)ZenchanAnim::RED_BLINK, { 9 + (float)i*n, 3*n, n, n });

	if (look == EnemyLook::LEFT) sprite->SetAnimation((int)ZenchanAnim::WALK_LEFT);
	if (look == EnemyLook::RIGHT) sprite->SetAnimation((int)ZenchanAnim::WALK_RIGHT);

	return AppStatus::OK;
}
void Enemy::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
EnemyState Enemy::GetState()
{
	return state;
}
void Enemy::SetState(const EnemyState& s)
{
	state = s;
	if (state == EnemyState::BUBBLE) SetAnimation((int)ZenchanAnim::BUBBLE);
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
	BubbleCounter();

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void Enemy::MoveX()
{
	AABB box;
	int prev_x = pos.x;
	if (state != EnemyState::BUBBLE) {
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
	else {
		if (pos.y <= MAX_HEIGHT) {
		//left movement
		if (WINDOW_WIDTH / 2 - PHYSICAL_OFFSET + TOP_OFFSET <= pos.x)
		{
			pos.x -= HORIZONTAL_ADVANCE_TOP;
			look = EnemyLook::LEFT;
		}
		//right movement
		else if (WINDOW_WIDTH / 2 - PHYSICAL_OFFSET - TOP_OFFSET >= pos.x)
		{
			pos.x += HORIZONTAL_ADVANCE_TOP;
			look = EnemyLook::RIGHT;
		}
		else
		{
			if (look == EnemyLook::LEFT) pos.x -= HORIZONTAL_ADVANCE_TOP;
			if (look == EnemyLook::RIGHT) pos.x += HORIZONTAL_ADVANCE_TOP;
		}
		}
	}
}
void Enemy::MoveY()
{
	if (state == EnemyState::BUBBLE)
	{
		if (pos.y > MAX_HEIGHT)
		{
			pos.y -= VERTICAL_ADVANCE;
		}
	}
}
void Enemy::BubbleCounter()
{
	if (state == EnemyState::BUBBLE) {
		bubbleTimer += GetFrameTime();
		if (bubbleTimer >= POP_TIME)
		{
			alive = false;
		}
		else if (bubbleTimer >= BLINK_TIME)
		{
			state = EnemyState::BLINK_END;
			SetAnimation((int)ZenchanAnim::RED_BLINK);
		}
		else if (bubbleTimer >= RED_TIME)
		{
			state = EnemyState::RED_END;
			SetAnimation((int)ZenchanAnim::RED_START);
		}
	}
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