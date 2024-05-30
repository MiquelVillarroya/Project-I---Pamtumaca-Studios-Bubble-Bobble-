#include "ObjectManager.h"

ObjectManager::ObjectManager()
{
	scoreParticles = nullptr;
}
ObjectManager::~ObjectManager()
{
	Release();
}
AppStatus ObjectManager::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_BUBBLE, "images/scores.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	return AppStatus::OK;
}
void ObjectManager::SetParticleScoreManager(ParticleScoreManager* part)
{
	this->scoreParticles = part;
}
void ObjectManager::Add(const Point& pos, ObjectType type)
{
	Object* object;

	object = new Object(pos, type);
	object->SetScoreParticles(scoreParticles);
	objects.push_back(object);
}
int ObjectManager::Update(const AABB& player_hitbox)
{
	AABB obj_box;
	int score = 0;

	auto it = objects.begin();
	while (it != objects.end())
	{
		obj_box = (*it)->GetHitbox();
		if (player_hitbox.TestAABB(obj_box))
		{
			score = (*it)->Points();

			//Delete the object
			delete* it;
			//Erase the object from the vector and get the iterator to the next valid element
			it = objects.erase(it);
		}
		else
		{
			//Move to the next object
			++it;
		}
	}

	return score;
}
void ObjectManager::Draw() const
{
	for (const Object* obj : objects)
		if (obj->IsAlive())	obj->Draw();
}
void ObjectManager::DrawDebug(const Color& col) const
{
	for (const Object* obj : objects)
		if (obj->IsAlive())	obj->DrawHitbox(col);
}
void ObjectManager::Release()
{
	for (Object* obj : objects)
		delete obj;
	objects.clear();
}