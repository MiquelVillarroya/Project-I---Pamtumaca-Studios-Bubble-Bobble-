#include "ObjectManager.h"

ObjectManager::ObjectManager()
{
	scoreParticles = nullptr;
	map = nullptr;
}
ObjectManager::~ObjectManager()
{
	Release();
}
AppStatus ObjectManager::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_SCORES, "images/scores.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	return AppStatus::OK;
}
void ObjectManager::SetParticleScoreManager(ParticleScoreManager* part)
{
	this->scoreParticles = part;
}
void  ObjectManager::SetObjectsTileMap(TileMap* tilemap)
{
	map = tilemap;
}
void ObjectManager::Add(const Point& pos, ObjectType type)
{
	Object* object;
	object = new Object(pos, type);

	object->Initialise();
	object->SetTileMap(map);
	object->SetScoreParticles(scoreParticles);
	objects.push_back(object);
}
int ObjectManager::Update(const AABB& player_hitbox)
{
	AABB obj_box;
	int score = 0;

	for (Object* obj : objects)
	{
		if (obj->IsAlive())
		{
			obj->Update();
			obj_box = obj->GetHitbox();
			if (player_hitbox.TestAABB(obj_box))
			{
				score = obj->Points();
				obj->SetAlive(false);
			}
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