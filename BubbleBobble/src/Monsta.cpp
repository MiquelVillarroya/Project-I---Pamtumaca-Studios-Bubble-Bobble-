#include "Monsta.h"
#include "Sprite.h"

Monsta::Monsta(const Point& p, int width, int height, int frame_width, int frame_height) :
	Enemy(p, ENEMY_PHYSICAL_WIDTH, ENEMY_PHYSICAL_HEIGHT, ENEMY_FRAME_SIZE, ENEMY_FRAME_SIZE)
{
	vert_dir = VertDir::UP;
}
Monsta::~Monsta()
{
}
AppStatus Monsta::Initialise(Look look, const AABB& area)
{
	int i;
	const int n = ENEMY_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	render = new Sprite(data.GetTexture(Resource::IMG_MONSTA));
	if (render == nullptr)
	{
		LOG("failed to allocate memory for monsta sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)MonstaAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)MonstaAnim::WALKING_RIGHT, MONSTA_ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)MonstaAnim::WALKING_RIGHT, { (float)i * n, 0, -n, n });
	sprite->SetAnimationDelay((int)MonstaAnim::WALKING_LEFT, MONSTA_ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)MonstaAnim::WALKING_LEFT, { (float)i * n, 0, n, n });

	sprite->SetAnimationDelay((int)MonstaAnim::WALKING_ANGRY_RIGHT, MONSTA_ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)MonstaAnim::WALKING_ANGRY_RIGHT, { (float)i * n, n, n, n });
	sprite->SetAnimationDelay((int)MonstaAnim::WALKING_ANGRY_LEFT, MONSTA_ANIM_DELAY);
	for (i = 0; i < 2; ++i)
		sprite->AddKeyFrame((int)MonstaAnim::WALKING_ANGRY_LEFT, { (float)i * n, n, -n, n });

	sprite->SetAnimationDelay((int)MonstaAnim::DYING, MONSTA_ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)MonstaAnim::DYING, { (float)i*n + 2*n, 2*n, n, n });

	this->look = look;
	if (look == Look::LEFT)			sprite->SetAnimation((int)MonstaAnim::WALKING_LEFT);
	else if (look == Look::RIGHT) 	sprite->SetAnimation((int)MonstaAnim::WALKING_RIGHT);

	visibility_area = area;

	return AppStatus::OK;
}
EnemyType Monsta::GetEnemyType() const
{
	return EnemyType::MONSTA;
}
bool Monsta::Update(const AABB& box)
{
	MoveX();
	MoveY();

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
	Warp();
	return false;
}
void Monsta::MoveX()
{
	AABB box;
	int prev_x = pos.x;
	box = GetHitbox();
	if (look == Look::RIGHT)
	{
		pos.x += MONSTA_SPEED;
		if (map->TestCollisionWallRight(box) || map->TestCollisionPlatRight(box))
		{
			pos.x = prev_x;
			look = Look::LEFT;
			SetAnimation((int)MonstaAnim::WALKING_LEFT);
		}
	}
	else if (look == Look::LEFT || map->TestCollisionPlatLeft(box))
	{
		pos.x += -MONSTA_SPEED;
		if (map->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			look = Look::RIGHT;
			SetAnimation((int)MonstaAnim::WALKING_RIGHT);
		}
	}
}
void Monsta::MoveY()
{
	AABB box;
	int prev_y = pos.y;
	box = GetHitbox();
	if (vert_dir == VertDir::UP)
	{
		pos.y += -MONSTA_SPEED;
		if (map->TestCollisionCeiling(box, &pos.y))
		{
			pos.y = prev_y;
			vert_dir = VertDir::DOWN;
		}
	}
	else if (vert_dir == VertDir::DOWN)
	{
		pos.y += MONSTA_SPEED;
		if (map->TestCollisionGround(box, &pos.y))
		{
			pos.y = prev_y;
			vert_dir = VertDir::UP;
		}
	}
}
