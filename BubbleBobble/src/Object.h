#pragma once
#include "Entity.h"
#include "ParticleScoreManager.h"
#include "Globals.h"

#define OBJECT_PHYSICAL_SIZE	16
#define OBJECT_FRAME_SIZE		16

#define POINTS_MUSHROOM		100
#define POINTS_BANANA		500
#define POINTS_CHERRY		700
#define POINTS_ICE_CREAM	900
#define POINTS_FLAM			2000
#define POINTS_CAKE			2000
#define SIGNAL_CANDY		-1
#define SIGNAL_SHOES		-2

class Object : public Entity
{
public:
	Object(const Point& p, ObjectType t);
	~Object();

	//Set ParticleScoreManager reference
	void SetScoreParticles(ParticleScoreManager* part);

	int Points() const;

	void DrawDebug(const Color& col) const;

private:

	ObjectType type;
	ParticleScoreManager* scoreParticles;
};

