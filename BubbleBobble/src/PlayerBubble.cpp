#include "PlayerBubble.h"
#include "Sprite.h"
#include "TileMap.h"
//#include "Globals.h"
//#include "raymath.h"

#define BUBBLE_SHOOTING_DELAY	ANIM_DELAY / 2

PlayerBubble::PlayerBubble(const Point& p, const Point& d, int width, int height, int frame_width, int frame_heigth) :
	Shot(p, d, width, height, frame_width, frame_heigth)
{
	forceDelay = BUBBLE_FORCE_DELAY;
	forceMax = 0;
}
PlayerBubble::~PlayerBubble()
{
}
AppStatus PlayerBubble::Initialise()
{
	int i;
	const int n = BUBBLE_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	render = new Sprite(data.GetTexture(Resource::IMG_BUBBLE));
	if (render == nullptr) {
		LOG("Failed to allocate memory for bubble sprite");
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations(1);

	//Set Animations
	sprite->SetAnimationDelay(0, BUBBLE_SHOOTING_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame(0, { (float)i * n, 0, n, n });

	SetAnimation(0);

	return AppStatus::OK;
}
bool PlayerBubble::Update()
{
	int finishFlag = false;

	AABB box;
	int prev_x = pos.x;
	forceDelay--;
	if (forceDelay == 0)
	{
		if (IsMovingLeft()) pos.x -= HORIZONTAL_ADVANCE;
		else if (IsMovingRight()) pos.x += HORIZONTAL_ADVANCE;
		forceDelay = BUBBLE_FORCE_DELAY;
		forceMax++;
		box = GetHitbox();
		if (pos.x < 16 || pos.x > 226 || forceMax == BUBBLE_MAX_FORCE)
		{
			//map->TestCollisionWallLeft();
			finishFlag = true;
		}
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();

	return finishFlag;

}
