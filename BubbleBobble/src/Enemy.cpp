#include "Enemy.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Enemy::Enemy(const Point&p, EnemyState s, EnemyLook l, EnemyType t) :
	Entity (p, ENEMY_PHYSICAL_WIDTH, ENEMY_PHYSICAL_HEIGTH, ENEMY_FRAME_SIZE, ENEMY_FRAME_SIZE)
{
	state = s;
	look = l;
	type = t; //Temporal Approach
	alive = true;
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
		sprite->AddKeyFrame((int)ZenchanAnim::RED_START, { 6*n + (float)i*n, 3*n, n, n });
	sprite->SetAnimationDelay((int)ZenchanAnim::RED_BLINK, ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)ZenchanAnim::RED_BLINK, { 9*n + (float)i*n, 3*n, n, n });

	//Zenchan Temporal Approach
	sprite->SetAnimationDelay((int)ZenchanAnim::WALK_RIGHT_H, ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)ZenchanAnim::WALK_RIGHT_H, { (float)i*n, 5*n, -n, n });
	sprite->SetAnimationDelay((int)ZenchanAnim::WALK_LEFT_H, ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)ZenchanAnim::WALK_LEFT_H, { (float)i*n, 5*n, n, n });

	sprite->SetAnimationDelay((int)ZenchanAnim::BUBBLE_H, ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)ZenchanAnim::BUBBLE_H, { (float)i*n, 9*n, n, n });
	sprite->SetAnimationDelay((int)ZenchanAnim::RED_START_H, ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)ZenchanAnim::RED_START_H, { 6*n + (float)i*n, 9*n, n, n });
	sprite->SetAnimationDelay((int)ZenchanAnim::RED_BLINK_H, ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)ZenchanAnim::RED_BLINK_H, { 9*n + (float)i*n, 9*n, n, n });

	if (look == EnemyLook::LEFT && type == EnemyType::ZENCHAN) sprite->SetAnimation((int)ZenchanAnim::WALK_LEFT);
	if (look == EnemyLook::RIGHT && type == EnemyType::ZENCHAN) sprite->SetAnimation((int)ZenchanAnim::WALK_RIGHT);
	if (look == EnemyLook::LEFT && type == EnemyType::HIDEGONS) sprite->SetAnimation((int)ZenchanAnim::WALK_LEFT_H);
	if (look == EnemyLook::RIGHT && type == EnemyType::HIDEGONS) sprite->SetAnimation((int)ZenchanAnim::WALK_RIGHT_H);

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
	if (state == EnemyState::BUBBLE && type == EnemyType::ZENCHAN) SetAnimation((int)ZenchanAnim::BUBBLE);
	if (state == EnemyState::BUBBLE && type == EnemyType::HIDEGONS) SetAnimation((int)ZenchanAnim::BUBBLE_H);

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
				if (type == EnemyType::ZENCHAN) SetAnimation((int)ZenchanAnim::WALK_LEFT);
				else if (type == EnemyType::HIDEGONS) SetAnimation((int)ZenchanAnim::WALK_LEFT_H);
			}

		}
		else if (look == EnemyLook::LEFT)
		{
			pos.x += -ENEMY_SPEED;
			if (map->TestCollisionWallLeft(box))
			{
				pos.x = prev_x;
				look = EnemyLook::RIGHT;
				if (type == EnemyType::ZENCHAN) SetAnimation((int)ZenchanAnim::WALK_RIGHT);
				else if (type == EnemyType::HIDEGONS) SetAnimation((int)ZenchanAnim::WALK_RIGHT_H);
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
			if (type == EnemyType::ZENCHAN) SetAnimation((int)ZenchanAnim::RED_BLINK);
			else if (type == EnemyType::HIDEGONS) SetAnimation((int)ZenchanAnim::RED_BLINK_H);
		}
		else if (bubbleTimer >= RED_TIME)
		{
			if (type == EnemyType::ZENCHAN) SetAnimation((int)ZenchanAnim::RED_START);
			else if (type == EnemyType::HIDEGONS) SetAnimation((int)ZenchanAnim::RED_START_H);
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