#include "Object.h"
#include "StaticImage.h"
#include "MemLeaks.h"

Sound objectsound[10];

Object::Object(const Point& p, ObjectType t) : Entity(p, OBJECT_PHYSICAL_SIZE, OBJECT_PHYSICAL_SIZE, OBJECT_FRAME_SIZE, OBJECT_FRAME_SIZE)
{
	type = t;
	scoreParticles = nullptr;
	map = nullptr;
}
Object::~Object()
{
}
void Object::Initialise()
{	
	Rectangle rc;
	const int n = 2*TILE_SIZE;
	switch (type)
	{
		case ObjectType::MUSHROOM: rc = { 8*n, 0, n, n}; break;
		case ObjectType::BANANA: rc = { 8*n, n, n, n }; break;
		case ObjectType::CHERRY: rc = { 8*n, 5*n, n, n }; break;
		case ObjectType::ICE_CREAM: rc = { 8*n, 2*n, n, n }; break;
		case ObjectType::FLAM: rc = { 8*n, 3*n, n, n }; break;
		case ObjectType::CAKE: rc = { 8*n, 4*n, n, n }; break;
		case ObjectType::YELLOW_CANDY: rc = { 6*n, 6*n, n, n }; break;
		case ObjectType::SHOES: rc = { 7*n, 6*n, n, n }; break;

		default: LOG("Internal error: object creation of invalid type");
	}

	ResourceManager& data = ResourceManager::Instance();
	render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
}
void Object::SetScoreParticles(ParticleScoreManager* part)
{
	scoreParticles = part;
}
void Object::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
int Object::Points() const
{

	objectsound[0] = LoadSound("audio/FX/Items/GrabFruitSFX.wav");
	if (type == ObjectType::MUSHROOM)
	{
		PlaySound(objectsound[0]);
		scoreParticles->Add(pos, type);
		return POINTS_MUSHROOM;
	}
	else if (type == ObjectType::BANANA)
	{
		PlaySound(objectsound[0]);
		scoreParticles->Add(pos, type);
		return POINTS_BANANA;
	}
	else if (type == ObjectType::CHERRY)
	{
		PlaySound(objectsound[0]);
		scoreParticles->Add(pos, type);
		return POINTS_CHERRY;
	}
	else if (type == ObjectType::ICE_CREAM)
	{
		PlaySound(objectsound[0]);
		scoreParticles->Add(pos, type);
		return POINTS_ICE_CREAM;
	}
	else if (type == ObjectType::FLAM)
	{
		PlaySound(objectsound[0]);
		scoreParticles->Add(pos, type);
		return POINTS_FLAM;
	}
	else if (type == ObjectType::CAKE)
	{
		PlaySound(objectsound[0]);
		scoreParticles->Add(pos, type);
		return POINTS_CAKE;
	}
	else if (type == ObjectType::YELLOW_CANDY)
	{
		PlaySound(objectsound[0]);
		return SIGNAL_CANDY;
	}
	else if (type == ObjectType::SHOES)
	{
		PlaySound(objectsound[0]);
		return SIGNAL_SHOES;
	}
	else
	{
		LOG("Internal error: object type invalid when giving points");
		return 0;
	}
}
void Object::Update()
{
	AABB box;
	int prev_y;

	box = GetHitbox();
	pos.y++;
	if (map->TestCollisionGround(box, &pos.y))
	{
	}
}
void Object::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}