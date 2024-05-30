#pragma once
#include "Object.h"

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	AppStatus Initialise();

	//Set the object reference to the Particle Score Manager
	void SetParticleScoreManager(ParticleScoreManager* part);

	void Add(const Point& pos, ObjectType type);

	int Update(const AABB& player_hitbox);

	//Draw all active objects
	void Draw() const;

	//Draw hitboxes of objects for debugging
	void DrawDebug(const Color& col) const;

	//Delete all objects and clear the "objects" vector
	void Release();

private:
	std::vector<Object*> objects;

	ParticleScoreManager* scoreParticles;
};