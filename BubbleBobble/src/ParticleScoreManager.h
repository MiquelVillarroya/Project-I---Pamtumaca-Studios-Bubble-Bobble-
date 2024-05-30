#pragma once
#include <array>
#include "ParticleScore.h"

#define MAX_SCORE_PARTICLES		16

class ParticleScoreManager
{
public:
	ParticleScoreManager();
	~ParticleScoreManager();

	AppStatus Initialise();

	//Add a new shot with the given position and direction
	void Add(const Point& pos, const ObjectType& type);

	//Remove all the shotsa
	void Clear();

	//Update shot positions and check for collisions with the level and player
	void Update();

	//Draw all active shots
	void Draw() const;

private:
	std::array<ParticleScore, MAX_SCORE_PARTICLES> particles;
};

