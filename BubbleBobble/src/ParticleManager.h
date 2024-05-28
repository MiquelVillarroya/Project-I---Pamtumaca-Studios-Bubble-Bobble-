#pragma once
#include <array>
#include "Particle.h"

#define MAX_PARTICLES		64

class ParticleManager
{
public:
	ParticleManager();
	~ParticleManager();

	AppStatus Initialise();

	//Add a new shot with the given position and direction
	void Add(const Point& pos);

	//Remove all the shotsa
	void Clear();

	//Update shot positions and check for collisions with the level and player
	void Update();

	//Draw all active shots
	void Draw() const;

private:
	std::array<Particle, MAX_PARTICLES> particles;
};

