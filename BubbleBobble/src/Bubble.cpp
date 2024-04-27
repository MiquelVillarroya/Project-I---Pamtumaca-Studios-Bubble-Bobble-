#include "Bubble.h"
#include "Sprite.h"
//#include "TileMap.h"
//#include "Globals.h"
//#include "raymath.h"


Bubble::Bubble(const Point& p, const Point& d, int width, int height, int frame_width, int frame_heigth) :
	Shot(p, d, width, height, frame_width, frame_heigth)
{
	state = BubbleState::SHOT;

	alive = true;
	move = true;

	forceDelay = BUBBLE_FORCE_DELAY;
	forceMax = 0;
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
	sprite->SetAnimationDelay((int)BubbleAnim::INITIAL, ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)BubbleAnim::INITIAL, { (float)i*n, 0, n, n});
	
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

	SetAnimation((int)BubbleAnim::INITIAL);

	return AppStatus::OK;
}
//bool Bubble::IsAlive() const
//{
//	if (alive == true) return true;
//	return false;
//}
void Bubble::Update(const AABB& box)
{
	MoveX();
	MoveY();
	BubbleCounter();
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void Bubble::MoveX()
{
	AABB box;
	int prev_x = pos.x;
	if (move == true) {
		forceDelay--;
		if (forceDelay == 0 && state == BubbleState::SHOT)
		{
			if (IsMovingLeft()) pos.x -= HORIZONTAL_ADVANCE;
			else if (IsMovingRight()) pos.x += HORIZONTAL_ADVANCE;
			forceDelay = BUBBLE_FORCE_DELAY;
			forceMax++;
			box = GetHitbox();
			if (pos.x < 16)
			{
				pos.x = 16;
				forceDelay = BUBBLE_FORCE_DELAY * 2;
				state = BubbleState::NORMAL;
				SetAnimation((int)BubbleAnim::IDLE);
			}
			else if (pos.x > 226)
			{
				pos.x = 226;
				forceDelay = BUBBLE_FORCE_DELAY * 2;
				state = BubbleState::NORMAL;
				SetAnimation((int)BubbleAnim::IDLE);
			}
			if (forceMax == BUBBLE_MAX_FORCE) {
				forceDelay = BUBBLE_FORCE_DELAY * 2;
				state = BubbleState::NORMAL;
				SetAnimation((int)BubbleAnim::IDLE);
			}
		}
		else if (state != BubbleState::SHOT && pos.y <= MAX_HEIGHT) {
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
			forceDelay = BUBBLE_FORCE_DELAY * 2;
		}
	}

}
void Bubble::MoveY()
{
	if (move == true)
	{
		if (state != BubbleState::SHOT && pos.y > MAX_HEIGHT)
		{
			pos.y -= VERTICAL_ADVANCE;
		}
	}
}
void Bubble::BubbleCounter()
{
	if (state != BubbleState::SHOT) {
		bubbleTimer += GetFrameTime();
		if (bubbleTimer >= POP_TIME + 1)
		{
			alive = false;

		}
		else if (bubbleTimer >= POP_TIME)
		{
			SetAnimation((int)BubbleAnim::POP);
			move = false;
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
}