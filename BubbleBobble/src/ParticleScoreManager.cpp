#include "ParticleScoreManager.h"

ParticleScoreManager::ParticleScoreManager()
{
}
ParticleScoreManager::~ParticleScoreManager()
{
}
AppStatus ParticleScoreManager::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_SCORES, "images/scores.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	return AppStatus::OK;
}
void ParticleScoreManager::Add(const Point& pos, const ObjectType& type)
{
	bool found = false;
	for (ParticleScore& particle : particles)
	{
		if (!particle.IsAlive())
		{
			particle.SetType(type);
			particle.Init(pos);
			found = true;
			break;
		}
	}
	if (!found) LOG("Not enough space to add a new particle in the ParticleScoreManager!");
}
void ParticleScoreManager::Clear()
{
	for (ParticleScore& particle : particles)
		particle.SetAlive(false);
}
void ParticleScoreManager::Update()
{
	bool animation_complete;

	for (ParticleScore& particle : particles)
	{
		if (particle.IsAlive())
		{
			//Update animation
			animation_complete = particle.Update();

			if (animation_complete)
				particle.SetAlive(false);
		}
	}
}
void ParticleScoreManager::Draw() const
{
	//Iterate over the particles vector by reference instead of by value to avoid a copy
	//of each Entity object
	for (const ParticleScore& particle : particles)
		if (particle.IsAlive())	particle.Draw();
}