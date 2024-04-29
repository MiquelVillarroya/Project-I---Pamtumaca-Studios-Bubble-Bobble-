#pragma once
#include "Entity.h"
#include "TileMap.h"
#include "ShotManager.h"

//Representation model size: 16x16
#define PLAYER_FRAME_SIZE		16

//Logical model size: 12x14
#define PLAYER_PHYSICAL_WIDTH	12
#define PLAYER_PHYSICAL_HEIGHT	14

//Horizontal speed and vertical speed while falling down
#define PLAYER_SPEED			1

//Frame animation delay while idle
#define ANIM_IDLE_DELAY			(4*ANIM_DELAY)

//Vertical speed while on a ladder
#define PLAYER_LADDER_SPEED		1

//Frame animation delay while on a ladder
#define ANIM_LADDER_DELAY		(2*ANIM_DELAY)

//When jumping, initial jump speed and maximum falling speed
#define PLAYER_JUMP_FORCE		9

//Frame delay for updating the jump velocity
#define PLAYER_JUMP_DELAY		2

//Player is levitating when abs(speed) <= this value
#define PLAYER_LEVITATING_SPEED	0.75

//Gravity affects jumping velocity when jump_delay is 0
#define GRAVITY_FORCE			1

//Spawn locations
#define PLAYER_SPAWN_X			24
#define PLAYER_SPAWN_Y			199

//Dead timer animation
#define DEAD_COOLDOWN			1.67

//Bubble shot speed
#define BUBBLE_SHOT_SPEED		1

//Logic states
enum class State { IDLE, WALKING, JUMPING, FALLING, SHOOTING, DEAD };

//Rendering states
enum class PlayerAnim {
	IDLE_LEFT, IDLE_RIGHT,
	WALKING_LEFT, WALKING_RIGHT,
	JUMPING_LEFT, JUMPING_RIGHT,
	LEVITATING_LEFT, LEVITATING_RIGHT,
	FALLING_LEFT, FALLING_RIGHT,
	SHOT_RIGHT, SHOT_LEFT,
	DEATH,
	NUM_ANIMATIONS
};

class Player: public Entity
{
public:
	Player(const Point& p, State s, Look view);
	~Player();
	
	AppStatus Initialise();
	void SetShotManager(ShotManager* shots);
	void SetTileMap(TileMap* tilemap);

	void InitScore();
	void IncrScore(int n);
	int GetScore();
	int GetLives() const;
	bool GetGod() const;
	State GetState() const;
	void MinusLife();

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

private:
	bool IsLookingRight() const;
	bool IsLookingLeft() const;

	//Player mechanics
	void GodMode();
	void MoveX();
	void MoveY();
	void LogicJumping();
	void BubbleShot();

	//Animation management
	void SetAnimation(int id);
	PlayerAnim GetAnimation();
	void Stop();
	void StartWalkingLeft();
	void StartWalkingRight();
	void StartFalling();
	void StartJumping();
	void ChangeAnimRight();
	void ChangeAnimLeft();

	//Jump steps
	bool IsAscending() const;
	bool IsLevitating() const;
	bool IsDescending() const;

	State state;
	Look look;
	int jump_delay;
	float deadTimer;
	bool god;

	float elapsedTimeBubble;

	TileMap *map;
	ShotManager *shots;

	int score;
	int lives;
};

