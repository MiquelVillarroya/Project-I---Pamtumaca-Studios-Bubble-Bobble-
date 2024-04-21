
#include "Player.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Sound playersound[10];


Player::Player(const Point& p, State s, Look view) :
	Entity(p, PLAYER_PHYSICAL_WIDTH, PLAYER_PHYSICAL_HEIGHT, PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE)
{
	state = s;
	look = view;
	jump_delay = PLAYER_JUMP_DELAY;
	map = nullptr;
	elapsedTimeBubble = 0;
	score = 0;
	lives = 3;
	deadTimer = 0;
	god = false;
	
}
Player::~Player()
{
	UnloadSound(playersound[0]);
	for (Bubble* bubl : bubbles)
	{
		delete bubl;
	}
	bubbles.clear();
}
AppStatus Player::Initialise()
{
	int i;
	const int n = PLAYER_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_PLAYER, "images/bubMove.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(Resource::IMG_PLAYER));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}

	playersound[0] = LoadSound("audio/FX/Characters/JumpSFX.wav");
	playersound[1] = LoadSound("audio/FX/Characters/BubbleAtkSFX.wav");

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)PlayerAnim::NUM_ANIMATIONS);
	
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_RIGHT, ANIM_IDLE_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_RIGHT, { 0, 0, -n, n });
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_RIGHT, { n, 0, -n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_LEFT, ANIM_IDLE_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_LEFT, { 0, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_LEFT, { n, 0, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_RIGHT, { (float)i*n, n, -n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_LEFT, ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_LEFT, { (float)i*n, n, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::FALLING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_RIGHT, { 0, 7*n, -n, n });
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_RIGHT, { n, 7*n, -n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::FALLING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_LEFT, { 0, 7*n, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_LEFT, { n, 7*n, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::JUMPING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_RIGHT, { 0, 6*n, -n, n });
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_RIGHT, { n, 6*n, -n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::JUMPING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_LEFT, { 0, 6*n, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_LEFT, { n, 6*n, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::LEVITATING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::LEVITATING_RIGHT, { n, 7*n, -n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::LEVITATING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::LEVITATING_LEFT, { n, 7*n, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::SHOT_RIGHT, ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::SHOT_RIGHT, { (float)i*n, 2*n, -n, n});
	sprite->SetAnimationDelay((int)PlayerAnim::SHOT_LEFT, ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::SHOT_LEFT, { (float)i*n, 2*n, n, n});

	sprite->SetAnimationDelay((int)PlayerAnim::DEATH, ANIM_DELAY);
	for (i = 0; i < 13; ++i) {
		sprite->AddKeyFrameOffset((int)PlayerAnim::DEATH, {(float)i*n, 10*n, n, 2*n}, 0, -n);
	}
		
	sprite->SetAnimation((int)PlayerAnim::IDLE_RIGHT);

	return AppStatus::OK;
}
void Player::InitScore()
{
	score = 0;
}
void Player::IncrScore(int n)
{
	score += n;
}
int Player::GetScore()
{
	return score;
}
int Player::GetLives() const
{
	return lives;
}
State Player::GetState() const
{
	return state;
}
bool Player::GetGod() const
{
	return god;
}
void Player::MinusLife()
{
	if (god == false)
	{
		lives--;
		state = State::DEAD;
		SetAnimation((int)PlayerAnim::DEATH);
	}
}
void Player::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
bool Player::IsLookingRight() const
{
	return look == Look::RIGHT;
}
bool Player::IsLookingLeft() const
{
	return look == Look::LEFT;
}
bool Player::IsAscending() const
{
	return dir.y < -PLAYER_LEVITATING_SPEED;
}
bool Player::IsLevitating() const
{
	return abs(dir.y) <= PLAYER_LEVITATING_SPEED;
}
bool Player::IsDescending() const
{
	return dir.y > PLAYER_LEVITATING_SPEED;
}
void Player::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
PlayerAnim Player::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (PlayerAnim)sprite->GetAnimation();
}
void Player::Stop()
{
	dir = { 0,0 };
	state = State::IDLE;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::IDLE_RIGHT);
	else					SetAnimation((int)PlayerAnim::IDLE_LEFT);
}
void Player::StartWalkingLeft()
{
	state = State::WALKING;
	look = Look::LEFT;
	SetAnimation((int)PlayerAnim::WALKING_LEFT);
}
void Player::StartWalkingRight()
{
	state = State::WALKING;
	look = Look::RIGHT;
	SetAnimation((int)PlayerAnim::WALKING_RIGHT);
}
void Player::StartFalling()
{
	dir.y = PLAYER_SPEED;
	state = State::FALLING;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::FALLING_RIGHT);
	else					SetAnimation((int)PlayerAnim::FALLING_LEFT);
	
}
void Player::StartJumping()
{
	PlaySound(playersound[0]);
	dir.y = -PLAYER_JUMP_FORCE;
	state = State::JUMPING;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::JUMPING_RIGHT);
	else					SetAnimation((int)PlayerAnim::JUMPING_LEFT);
	jump_delay = PLAYER_JUMP_DELAY;
}
void Player::ChangeAnimRight()
{
	look = Look::RIGHT;
	switch (state)
	{
		case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_RIGHT);    break; 
		case State::WALKING: SetAnimation((int)PlayerAnim::WALKING_RIGHT); break;
		case State::JUMPING: SetAnimation((int)PlayerAnim::JUMPING_RIGHT); break;
		case State::FALLING: SetAnimation((int)PlayerAnim::FALLING_RIGHT); break;
	}
}
void Player::ChangeAnimLeft()
{
	look = Look::LEFT;
	switch (state)
	{
		case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_LEFT);    break;
		case State::WALKING: SetAnimation((int)PlayerAnim::WALKING_LEFT); break;
		case State::JUMPING: SetAnimation((int)PlayerAnim::JUMPING_LEFT); break;
		case State::FALLING: SetAnimation((int)PlayerAnim::FALLING_LEFT); break;
	}
}
void Player::Update()
{
	//Player doesn't use the "Entity::Update() { pos += dir; }" default behaviour.
	//Instead, uses an independent behaviour for each axis.
	
	if (state == State::DEAD) {
		deadTimer += GetFrameTime();
		if (deadTimer >= DEAD_COOLDOWN)
		{
			state = State::IDLE;
			SetAnimation((int)PlayerAnim::IDLE_RIGHT);
			pos = { PLAYER_SPAWN_X, PLAYER_SPAWN_Y };
			deadTimer = 0;
		}
	}
	else {
		GodMode();
		MoveX();
		MoveY();
		BubbleShot();
		auto it = bubbles.begin();
		while (it != bubbles.end())
		{
			if ((*it)->IsAlive() == false)
			{
				delete* it;
				it = bubbles.erase(it);
			}
			else
			{
				(*it)->Update();
				++it;
			}

		}
	}
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
	Warp();
}
void Player::GodMode() {
	if (IsKeyPressed(KEY_F3))
	{
		god = !god;
	}
}
void Player::MoveX()
{
	AABB box;
	int prev_x = pos.x;

	if (IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT))
	{
		pos.x += -PLAYER_SPEED;
		if (state == State::IDLE) StartWalkingLeft();
		else
		{
			if (IsLookingRight()) ChangeAnimLeft();
		}

		box = GetHitbox();
		if (map->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			if (state == State::WALKING) Stop();
		}
	}
	else if (IsKeyDown(KEY_RIGHT))
	{
		pos.x += PLAYER_SPEED;
		if (state == State::IDLE) StartWalkingRight();
		else
		{
			if (IsLookingLeft()) ChangeAnimRight();
		}

		box = GetHitbox();
		if (map->TestCollisionWallRight(box))
		{
			pos.x = prev_x;
			if (state == State::WALKING) Stop();
		}
	}
	else
	{
		if (state == State::WALKING) Stop();
	}
}
void Player::MoveY()
{
	AABB box;

	if (state == State::JUMPING || state==State::SHOOTING)
	{
		LogicJumping();
	}
	else //idle, walking, falling
	{
		pos.y += PLAYER_SPEED;
		box = GetHitbox();
		if (map->TestCollisionGround(box, &pos.y))
		{
			if (state == State::FALLING) Stop();
			else if (IsKeyPressed(KEY_UP))
			{
				
				StartJumping();
			}
		}
		else
		{
			if (state != State::FALLING) StartFalling();
		}
	}
}
void Player::BubbleShot() {
 	elapsedTimeBubble += GetFrameTime();
	if (IsKeyPressed(KEY_SPACE) && elapsedTimeBubble >= .25)
	{
		PlaySound(playersound[1]);
		if (IsLookingLeft()) {
			Bubble* bubl = new Bubble({ pos.x - PLAYER_PHYSICAL_WIDTH, pos.y }, BubbleDirection::LEFT);
			bubl->Initialise();
			bubl->SetTileMap(map);
			bubbles.push_back(bubl);
			//if (shootTime >= .25) {
			//	state = State::
			//}
		}
		else if (IsLookingRight()) {
			Bubble* bubl = new Bubble({ pos.x + PLAYER_PHYSICAL_WIDTH, pos.y }, BubbleDirection::RIGHT);
			bubl->Initialise();
			bubl->SetTileMap(map);
			bubbles.push_back(bubl);
			SetAnimation((int)PlayerAnim::SHOT_RIGHT);
		}
		elapsedTimeBubble = 0;
	}
}
void Player::LogicJumping()
{
	AABB box, prev_box;
	int prev_y;

	jump_delay--;
	if (jump_delay == 0)
	{
		
		prev_y = pos.y;
		prev_box = GetHitbox();

		pos.y += dir.y;
		dir.y += GRAVITY_FORCE;
		jump_delay = PLAYER_JUMP_DELAY;

		//Is the jump finished?
		if (dir.y > PLAYER_JUMP_FORCE)
		{
			dir.y = PLAYER_SPEED;
			StartFalling();
		}
		else
		{
			//Jumping is represented with 3 different states
			if (IsAscending())
			{
				if (IsLookingRight())	SetAnimation((int)PlayerAnim::JUMPING_RIGHT);
				else					SetAnimation((int)PlayerAnim::JUMPING_LEFT);
			}
			else if (IsLevitating())
			{
				if (IsLookingRight())	SetAnimation((int)PlayerAnim::LEVITATING_RIGHT);
				else					SetAnimation((int)PlayerAnim::LEVITATING_LEFT);
			}
			else if (IsDescending())
			{
				if (IsLookingRight())	SetAnimation((int)PlayerAnim::FALLING_RIGHT);
				else					SetAnimation((int)PlayerAnim::FALLING_LEFT);
			}
		}
		//We check ground collision when jumping down
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
bool Player::CheckBubbleCollision(const AABB& enemy_box)
{
	AABB bubl_box;

	auto it = bubbles.begin();
	while (it != bubbles.end())
	{
		bubl_box = (*it)->GetHitbox();
		if (bubl_box.TestAABB(enemy_box))
		{
			delete* it;
			it = bubbles.erase(it);
			return true;
		}
		++it;
	}
	return false;
}
void Player::DrawDebug(const Color& col) const
{	
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);

	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 18*16, 0, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);
}
 void Player::DrawBubbles()
 {
	 auto it = bubbles.begin();
	 while (it != bubbles.end())
	 {
	 	(*it)->Draw();
		++it;
	 }
}
 void Player::DrawBubblesDebug(const Color& col) const
 {
	 auto it = bubbles.begin();
	 while (it != bubbles.end())
	 {
		 (*it)->DrawDebug(col);
		 ++it;
	 }
 }
 void Player::DrawGod(const Color& col) const
 {
	 DrawText(TextFormat("GOD MODE ON"), 90, -12, 10, col);
 }

 void Player::ClearBubbles()
 {
	 for (Bubble* bubl : bubbles)
	 {
		 delete bubl;
	 }
	 bubbles.clear();
 }
void Player::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_PLAYER);

	render->Release();
	auto it = bubbles.begin();
	while (it != bubbles.end())
	{
		(*it)->Release();
		++it;
	}
}