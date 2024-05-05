#include "Zenchan.h"
#include "Sprite.h"

Zenchan::Zenchan(const Point& p, int width, int height, int frame_width, int frame_height) :
	Enemy(p, ENEMY_PHYSICAL_WIDTH, ENEMY_PHYSICAL_HEIGHT, ENEMY_FRAME_SIZE, ENEMY_FRAME_SIZE)
{
	state = ZenchanState::ROAMING;
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

	return AppStatus::OK;
}
EnemyType Zenchan::GetEnemyType() const
{
	return EnemyType::ZENCHAN;
}
bool Zenchan::Update(const AABB& box)
{
	MoveX();
	MoveY();

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();

	return false;
}
void Zenchan::MoveX()
{
	AABB box;
	int prev_x = pos.x;
	box = GetHitbox();
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
void Zenchan::MoveY()
{

}
