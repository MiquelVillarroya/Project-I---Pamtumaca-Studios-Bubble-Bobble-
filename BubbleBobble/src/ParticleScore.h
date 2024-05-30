#pragma once
#include "Entity.h"
#include "Globals.h"

#define PARTICLE_FRAME_SIZE		16
#define PARTICLE_ANIM_DELAY		(ANIM_DELAY)

class ParticleScore : public Entity
{
public:
	ParticleScore();
	~ParticleScore();

	void Init(const Point& p);
	void SetType(const ObjectType& type);

	//"Overriden" function, particles don't move and returns true if the animation is complete
	bool Update();

	//Overridden function because the control point is located at the top left instead of the
	//bottom left (which is the default control point position for entities)
	void Draw() const;

private:
	float timer;
	bool finished;
	ObjectType type;
};

