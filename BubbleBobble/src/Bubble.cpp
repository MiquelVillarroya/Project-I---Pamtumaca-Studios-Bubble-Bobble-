#include "Bubble.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Enemy.h"
//#include "Globals.h"
//#include "raymath.h"


Bubble::Bubble(const Point& p, const Point& d, int width, int height, int frame_width, int frame_heigth, EnemyType type) :
	Shot(p, d, width, height, frame_width, frame_heigth)
{
	state = BubbleState::NORMAL;
	enemyType = type;
	bubbleTimer = 0;
}
Bubble::~Bubble()
{
}
AppStatus Bubble::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();
	render = new Sprite(data.GetTexture(Resource::IMG_BUBBLE));
	if (render == nullptr){
		LOG("Failed to allocate memory for bubble sprite");
	}
	
	//Set Animations
	SetAnimations();

	return AppStatus::OK;
}
EnemyType Bubble::Update(const AABB& box)
{
	EnemyType popFlag = EnemyType::NONE;;
	MoveX();
	MoveY();
	BubbleCounter(popFlag);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();

	return popFlag;
}
void Bubble::MoveX()
{
	AABB box;
	int prev_x = pos.x;
	if (pos.y <= MAX_HEIGHT) {
		//left movement
		if (WINDOW_WIDTH / 2 - PHYSICAL_OFFSET + TOP_OFFSET <= pos.x)
		{
			pos.x -= HORIZONTAL_ADVANCE_TOP;
			dir.x = -dir.x;
		}
		//right movement
		else if (WINDOW_WIDTH / 2 - PHYSICAL_OFFSET - TOP_OFFSET >= pos.x)
		{
			pos.x += HORIZONTAL_ADVANCE_TOP;
			dir.x = -dir.x;
		}
		else
		{
			if (IsMovingLeft()) pos.x -= HORIZONTAL_ADVANCE_TOP;
			if (IsMovingRight()) pos.x += HORIZONTAL_ADVANCE_TOP;
		}
	}
}
void Bubble::MoveY()
{
	if (pos.y > MAX_HEIGHT)
	{
		pos.y -= VERTICAL_ADVANCE;
	}
}
void Bubble::BubbleCounter(EnemyType& flag)
{
bubbleTimer += GetFrameTime();
if (bubbleTimer >= POP_TIME + 0.5)
	{
		this->SetAlive(false);
		if (enemyType != EnemyType::NONE)	flag = enemyType;
	}
else if (bubbleTimer >= POP_TIME && GetAnimation() == BubbleAnim::RED_BLINK)
	{
		SetAnimation((int)BubbleAnim::POP);
	}
else if (bubbleTimer >= BLINK_TIME && GetAnimation() == BubbleAnim::RED_START)
	{
		state = BubbleState::BLINK_END;
		SetAnimation((int)BubbleAnim::RED_BLINK);
	}
else if (bubbleTimer >= RED_TIME && GetAnimation() == BubbleAnim::IDLE)
	{
		state = BubbleState::RED_END;
		SetAnimation((int)BubbleAnim::RED_START);
	}
}
BubbleAnim Bubble::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (BubbleAnim)sprite->GetAnimation();
}
void Bubble::SetAnimations()
{
	int i, k;
	const int n = BUBBLE_FRAME_SIZE;

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)BubbleAnim::NUM_ANIMATIONS);

	if (enemyType == EnemyType::NONE) {
		sprite->SetAnimationDelay((int)BubbleAnim::IDLE, ANIM_DELAY);
		sprite->AddKeyFrame((int)BubbleAnim::IDLE, { 0, n, n, n });

		sprite->SetAnimationDelay((int)BubbleAnim::RED_START, ANIM_DELAY);
		sprite->AddKeyFrame((int)BubbleAnim::RED_START, { 6*n, n, n, n });

		sprite->SetAnimationDelay((int)BubbleAnim::RED_BLINK, ANIM_DELAY);
		sprite->AddKeyFrame((int)BubbleAnim::RED_BLINK, { 6*n, n, n, n });
		sprite->AddKeyFrame((int)BubbleAnim::RED_BLINK, { 9*n, n, n, n });

	}
	else {
		if (enemyType == EnemyType::ZENCHAN)		k = 2;
		else if (enemyType == EnemyType::HIDEGONS)	k = 3;
		else if (enemyType == EnemyType::MONSTA)	k = 4;
		sprite->SetAnimationDelay((int)BubbleAnim::IDLE, ANIM_DELAY);
		for (i = 0; i < 3; ++i)
			sprite->AddKeyFrame((int)BubbleAnim::IDLE, { (float)i * n, (float)k * n, n, n });

		sprite->SetAnimationDelay((int)BubbleAnim::RED_START, ANIM_DELAY);
		for (i = 0; i < 3; ++i)
			sprite->AddKeyFrame((int)BubbleAnim::RED_START, { (float)i * n + 6 * n, (float)k * n, n, n });

		sprite->SetAnimationDelay((int)BubbleAnim::RED_BLINK, ANIM_DELAY);
		for (i = 0; i < 3; ++i) {
			sprite->AddKeyFrame((int)BubbleAnim::RED_BLINK, { (float)i * n + 6 * n, (float)k * n, n, n });
			sprite->AddKeyFrame((int)BubbleAnim::RED_BLINK, { (float)i * n + 9 * n, (float)k * n, n, n });
		}
	}

	sprite->SetAnimationDelay((int)BubbleAnim::POP, ANIM_DELAY);
	sprite->AddKeyFrame((int)BubbleAnim::POP, { 0, 0, n, n });
	sprite->AddKeyFrame((int)BubbleAnim::POP, { n, 0, n, n });

	SetAnimation((int)BubbleAnim::IDLE);
}