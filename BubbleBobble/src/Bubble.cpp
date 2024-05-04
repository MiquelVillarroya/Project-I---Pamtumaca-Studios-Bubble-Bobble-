#include "Bubble.h"
#include "Sprite.h"
//#include "TileMap.h"
//#include "Globals.h"
//#include "raymath.h"


Bubble::Bubble(const Point& p, const Point& d, int width, int height, int frame_width, int frame_heigth) :
	Shot(p, d, width, height, frame_width, frame_heigth)
{
	state = BubbleState::NORMAL;

	alive = true;
	bubbleTimer = 0;
}
Bubble::~Bubble()
{
}
AppStatus Bubble::Initialise()
{
	int i;
	const int n = BUBBLE_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	render = new Sprite(data.GetTexture(Resource::IMG_BUBBLE));
	if (render == nullptr){
		LOG("Failed to allocate memory for bubble sprite");
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)BubbleAnim::NUM_ANIMATIONS);
	
	//Set Animations
	sprite->SetAnimationDelay((int)BubbleAnim::IDLE, ANIM_DELAY);
	sprite->AddKeyFrame((int)BubbleAnim::IDLE, { n, n, n, n});
	
	sprite->SetAnimationDelay((int)BubbleAnim::RED_START, ANIM_DELAY);
	sprite->AddKeyFrame((int)BubbleAnim::RED_START, { n, 2*n, n, n });

	sprite->SetAnimationDelay((int)BubbleAnim::RED_BLINK, ANIM_DELAY);
	sprite->AddKeyFrame((int)BubbleAnim::RED_BLINK, { n, 2*n, n, n });
	sprite->AddKeyFrame((int)BubbleAnim::RED_BLINK, { n, 3*n, n, n });
	
	sprite->SetAnimationDelay((int)BubbleAnim::POP, ANIM_DELAY);
	sprite->AddKeyFrame((int)BubbleAnim::POP, { 0, 10*n, n, n });
	sprite->AddKeyFrame((int)BubbleAnim::POP, { n, 10*n, n, n });

	SetAnimation((int)BubbleAnim::IDLE);

	return AppStatus::OK;
}
bool Bubble::Update(const AABB& box)
{
	bool popFlag;
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
void Bubble::BubbleCounter(bool& flag)
{
bubbleTimer += GetFrameTime();
if (bubbleTimer >= POP_TIME + 1)
	{
		flag = true;
		this->SetAlive(false);
	}
else if (bubbleTimer >= POP_TIME)
	{
		SetAnimation((int)BubbleAnim::POP);
	}
else if (bubbleTimer >= BLINK_TIME)
	{
		state = BubbleState::BLINK_END;
		SetAnimation((int)BubbleAnim::RED_BLINK);
	}
else if (bubbleTimer >= RED_TIME)
	{
		state = BubbleState::RED_END;
		SetAnimation((int)BubbleAnim::RED_START);
	}
}