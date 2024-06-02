#include "Zenchan.h"
#include "Sprite.h"
#include "TileMap.h"

Zenchan::Zenchan(const Point& p, int width, int height, int frame_width, int frame_height) :
	Enemy(p, ENEMY_PHYSICAL_WIDTH, ENEMY_PHYSICAL_HEIGHT, ENEMY_FRAME_SIZE, ENEMY_FRAME_SIZE)
{
	state = ZenchanState::ROAMING;
	jump_delay = 0;
}
Zenchan::~Zenchan()
{
}
AppStatus Zenchan::Initialise(Look look, const AABB& area)
{
	int i;
	const int n = ENEMY_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	render = new Sprite(data.GetTexture(Resource::IMG_ZENCHAN));
	if (render == nullptr)
	{
		LOG("failed to allocate memory for zenchan sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)ZenchanAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)ZenchanAnim::WALKING_RIGHT, ZENCHAN_ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)ZenchanAnim::WALKING_RIGHT, {(float)i*n, 0, -n, n});
	sprite->SetAnimationDelay((int)ZenchanAnim::WALKING_LEFT, ZENCHAN_ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)ZenchanAnim::WALKING_LEFT, { (float)i*n, 0, n, n});

	sprite->SetAnimationDelay((int)ZenchanAnim::WALKING_ANGRY_RIGHT, ZENCHAN_ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)ZenchanAnim::WALKING_ANGRY_RIGHT, { (float)i*n, n, n, n });
	sprite->SetAnimationDelay((int)ZenchanAnim::WALKING_ANGRY_LEFT, ZENCHAN_ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)ZenchanAnim::WALKING_ANGRY_LEFT, { (float)i*n, n, -n, n });

	sprite->SetAnimationDelay((int)ZenchanAnim::DYING, ZENCHAN_ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)ZenchanAnim::DYING, { (float)i*n, 3*n, n, n });

	this->look = look;
	if (look == Look::LEFT)			sprite->SetAnimation((int)ZenchanAnim::WALKING_LEFT);
	else if (look == Look::RIGHT) 	sprite->SetAnimation((int)ZenchanAnim::WALKING_RIGHT);

	visibility_area = area;
	//player = pl;

	return AppStatus::OK;
}
EnemyType Zenchan::GetEnemyType() const
{
	return EnemyType::ZENCHAN;
}
bool Zenchan::Update(const AABB& box)
{
	MoveX(box);
	MoveY(box);

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
	Warp();

	return false;
}
void Zenchan::MoveX(const AABB& player_hitbox)
{
	AABB box, sweptArea;
	Point player_pos;
	int prev_x = pos.x;
	box = GetHitbox();
	sweptArea = map->GetSweptAreaX(box);

	if (state == ZenchanState::ROAMING)
	{
		if (sweptArea.TestAABB(player_hitbox)) {
			player_pos.x = player_hitbox.pos.x;
			if (player_pos.x < pos.x) {
				pos.x += -ZENCHAN_SPEED;
				if (look == Look::RIGHT)
				{
					look = Look::LEFT;
					SetAnimation((int)ZenchanAnim::WALKING_LEFT);
				}

			}
			else if (player_pos.x >= pos.x) {
				pos.x += ZENCHAN_SPEED;
				if (look == Look::LEFT)
				{
					look = Look::RIGHT;
					SetAnimation((int)ZenchanAnim::WALKING_RIGHT);
				}
			}
		}
		else
		{
			if (look == Look::RIGHT)
			{
				pos.x += ZENCHAN_SPEED;
				if (map->TestCollisionWallRight(box))
				{
					pos.x = prev_x;
					look = Look::LEFT;
					SetAnimation((int)ZenchanAnim::WALKING_LEFT);
				}
			}
			else if (look == Look::LEFT)
			{
				pos.x += -ZENCHAN_SPEED;
				if (map->TestCollisionWallLeft(box))
				{
					pos.x = prev_x;
					look = Look::RIGHT;
					SetAnimation((int)ZenchanAnim::WALKING_RIGHT);
				}
			}
		}

	}
}
void Zenchan::MoveY(const AABB& player_hitbox)
{
	AABB box;

	if (state == ZenchanState::JUMPING)
	{
		LogicJumping();
	}
	else //roaming, falling
	{
		pos.y += ZENCHAN_SPEED;
		box = GetHitbox();
		if (map->TestCollisionGround(box, &pos.y))
		{
			if (state == ZenchanState::FALLING)	Stop();
			else if (GetRandomValue(1, 180) == 1)
			{
				StartJumping();
			}
		}
		else
		{
			if (state != ZenchanState::FALLING) StartFalling();
		}
	}
}
void Zenchan::StartFalling()
{
	dir.y = ZENCHAN_SPEED;
	state = ZenchanState::FALLING;
}
void Zenchan::Stop()
{
	dir = { 0,0 };
	state = ZenchanState::ROAMING;
}
void Zenchan::LogicJumping()
{
	AABB box, prev_box;
	int prev_y;

	jump_delay--;
	if (jump_delay == 0)
	{

		prev_y = pos.y;
		prev_box = GetHitbox();

		pos.y += dir.y;
		dir.y ++;
		jump_delay = ZENCHAN_JUMP_DELAY;

		//Is the jump finished?
		if (dir.y > ZENCHAN_JUMP_FORCE)
		{
			dir.y = ZENCHAN_SPEED;
			StartFalling();
		}

		if (dir.y >= 0)
		{
			box = GetHitbox();

			//A ground collision occurs if we were not in a collision state previously.
			//This prevents scenarios where, after levitating due to a previous jump, we found
			//ourselves inside a tile, and the entity would otherwise be placed above the tile,
			//crossing it.
			if (!map->TestCollisionGround(prev_box, &prev_y) &&
				map->TestCollisionGround(box, &pos.y))
			{
				Stop();
			}
		}
	}
}
void Zenchan::StartJumping()
{
	dir.y = -ZENCHAN_JUMP_FORCE;
	state = ZenchanState::JUMPING;
	jump_delay = ZENCHAN_JUMP_DELAY;
}

