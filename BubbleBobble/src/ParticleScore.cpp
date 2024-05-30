#include "ParticleScore.h"
#include "Sprite.h"

ParticleScore::ParticleScore()
{
	int i;
	const int n = PARTICLE_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	render = new Sprite(data.GetTexture(Resource::IMG_SCORES));
	if (render == nullptr)
	{
		LOG("failed to allocate memory for score sprite");
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations(5);
	sprite->SetAnimationDelay(0, PARTICLE_ANIM_DELAY);

	sprite->AddKeyFrame(0, { 0, 0, PARTICLE_FRAME_SIZE, PARTICLE_FRAME_SIZE });
	sprite->AddKeyFrame(1, { n, 0, PARTICLE_FRAME_SIZE, PARTICLE_FRAME_SIZE });
	sprite->AddKeyFrame(2, { 2 * n, 0, PARTICLE_FRAME_SIZE, PARTICLE_FRAME_SIZE });
	sprite->AddKeyFrame(3, { 3 * n, 0, PARTICLE_FRAME_SIZE, PARTICLE_FRAME_SIZE });
	sprite->AddKeyFrame(4, { 4 * n, 0, PARTICLE_FRAME_SIZE, PARTICLE_FRAME_SIZE });

	sprite->SetAnimation(0);

	SetAlive(false);

	finished = false;
	timer = 0;
	type = ObjectType::MUSHROOM;
}
ParticleScore::~ParticleScore()
{
}
void ParticleScore::Init(const Point& p)
{
	//Set position
	Set(p, { 0,0 }, PARTICLE_FRAME_SIZE, PARTICLE_FRAME_SIZE, PARTICLE_FRAME_SIZE, PARTICLE_FRAME_SIZE);
	finished = false;
	timer = 0;

	//Initialise animation
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	if (type == ObjectType::MUSHROOM)		sprite->SetAnimation(0);
	else if (type == ObjectType::BANANA) 	sprite->SetAnimation(1);
	else if (type == ObjectType::CHERRY)	sprite->SetAnimation(2);
	else if (type == ObjectType::ICE_CREAM)	sprite->SetAnimation(3);
	else if (type == ObjectType::FLAM)		sprite->SetAnimation(4);
	else if (type == ObjectType::CAKE)		sprite->SetAnimation(4);
	else sprite->SetAnimation(0);

}
void ParticleScore::SetType(const ObjectType& type)
{
	this->type = type;
}

bool ParticleScore::Update()
{
	pos.y--;
	timer += GetFrameTime();
	if (timer >= 0.5)
	{
		finished = true;
		timer = 0;
	}
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();

	return finished;
}
void ParticleScore::Draw() const
{
	render->Draw(pos.x, pos.y);
}