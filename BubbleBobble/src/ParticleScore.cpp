#include "ParticleScore.h"
#include "Sprite.h"

ParticleScore::ParticleScore()
{
	int i;
	const int n = PARTICLE_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	render = new Sprite(data.GetTexture(Resource::IMG_SCORES));

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations(1);

	sprite->SetAnimationDelay(0, PARTICLE_ANIM_DELAY);
	if (type == ObjectType::MUSHROOM)		sprite->AddKeyFrame(0, { 0, 0, PARTICLE_FRAME_SIZE, PARTICLE_FRAME_SIZE });
	else if (type == ObjectType::BANANA) 	sprite->AddKeyFrame(1, { n, 0, PARTICLE_FRAME_SIZE, PARTICLE_FRAME_SIZE });
	else if (type == ObjectType::CHERRY)	sprite->AddKeyFrame(2, { 2 * n, 0, PARTICLE_FRAME_SIZE, PARTICLE_FRAME_SIZE });
	else if (type == ObjectType::ICE_CREAM)	sprite->AddKeyFrame(3, { 3 * n, 0, PARTICLE_FRAME_SIZE, PARTICLE_FRAME_SIZE });
	else if (type == ObjectType::FLAM)		sprite->AddKeyFrame(4, { 4 * n, 0, PARTICLE_FRAME_SIZE, PARTICLE_FRAME_SIZE });
	else if (type == ObjectType::CAKE)		sprite->AddKeyFrame(4, { 4 * n, 0, PARTICLE_FRAME_SIZE, PARTICLE_FRAME_SIZE });

	sprite->SetAnimation(0);

	SetAlive(false);

	finished = false;
	timer = 0;
}
ParticleScore::~ParticleScore()
{
}
void ParticleScore::Init(const Point& p)
{
	//Set position
	Set(p, { 0,0 }, PARTICLE_FRAME_SIZE, PARTICLE_FRAME_SIZE, PARTICLE_FRAME_SIZE, PARTICLE_FRAME_SIZE);

	//Initialise animation
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(0);

}
void ParticleScore::SetType(const ObjectType& type)
{
	this->type = type;
}

bool ParticleScore::Update()
{
	pos.y--;
	timer += GetFrameTime();
	if (timer >= 0.4)
	{
		finished = true;
	}
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();

	return finished;
}
void ParticleScore::Draw() const
{
	render->Draw(pos.x, pos.y);
}