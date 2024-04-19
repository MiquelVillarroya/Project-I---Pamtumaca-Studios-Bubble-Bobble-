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
	alive = true;
	forceDelay = BUBBLE_FORCE_DELAY;
	forceMax = 0;
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
	sprite->AddKeyFrame((int)BubbleAnim::INITIAL, { 0, 0, n, n });
	sprite->AddKeyFrame((int)BubbleAnim::INITIAL, { n, 0, n, n });


	SetAnimation((int)BubbleAnim::INITIAL);

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
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void Bubble::MoveX()
{
	forceDelay--;
	if (forceDelay == 0 && state == BubbleState::SHOT)
	{
		if (direction == BubbleDirection::LEFT) pos.x -= HORIZONTAL_ADVANCE;
		if (direction == BubbleDirection::RIGHT) pos.x += HORIZONTAL_ADVANCE;
		forceDelay = BUBBLE_FORCE_DELAY;
		forceMax++;
		if (forceMax == BUBBLE_MAX_FORCE) {
			forceDelay = BUBBLE_FORCE_DELAY * 2;
			state = BubbleState::NORMAL;
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
void Bubble::MoveY()
{
	if (state != BubbleState::SHOT && pos.y > MAX_HEIGHT)
	{
		pos.y -= VERTICAL_ADVANCE; 
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