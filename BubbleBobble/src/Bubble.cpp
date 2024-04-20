#include "Bubble.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include "raymath.h"


Bubble::Bubble(const Point& p, BubbleDirection bd) :
	Entity(p, BUBBLE_PHYSICAL_WIDTH, BUBBLE_PHYSICAL_HEIGHT, BUBBLE_FRAME_SIZE, BUBBLE_FRAME_SIZE)
{
	state = BubbleState::SHOT;
	direction = bd;
	map = nullptr;

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
	if (data.LoadTexture(Resource::IMG_BUBBLE, "images/bubbles.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

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
void Bubble::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
void Bubble::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
BubbleAnim Bubble::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (BubbleAnim)sprite->GetAnimation();
}
bool Bubble::IsAlive() const
{
	if (alive == true) return true;
	return false;
}
void Bubble::Update()
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
	if (move == true) {
		forceDelay--;
		if (forceDelay == 0 && state == BubbleState::SHOT)
		{
			if (direction == BubbleDirection::LEFT) pos.x -= HORIZONTAL_ADVANCE;
			if (direction == BubbleDirection::RIGHT) pos.x += HORIZONTAL_ADVANCE;
			forceDelay = BUBBLE_FORCE_DELAY;
			forceMax++;
			box = GetHitbox();
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
				direction = BubbleDirection::LEFT;
			}
			//right movement
			else if (WINDOW_WIDTH / 2 - PHYSICAL_OFFSET - TOP_OFFSET >= pos.x)
			{
				pos.x += HORIZONTAL_ADVANCE_TOP;
				direction = BubbleDirection::RIGHT;
			}
			else
			{
				if (direction == BubbleDirection::LEFT) pos.x -= HORIZONTAL_ADVANCE_TOP;
				if (direction == BubbleDirection::RIGHT) pos.x += HORIZONTAL_ADVANCE_TOP;
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
void Bubble::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
void Bubble::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_BUBBLE);

	render->Release();
}