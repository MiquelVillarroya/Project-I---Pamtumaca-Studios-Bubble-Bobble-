#include "Object.h"
#include "StaticImage.h"

Object::Object(const Point& p, ObjectType t) : Entity(p, OBJECT_PHYSICAL_SIZE, OBJECT_PHYSICAL_SIZE, OBJECT_FRAME_SIZE, OBJECT_FRAME_SIZE)
{
	type = t; 
	
	Rectangle rc;
	const int n = 2*TILE_SIZE;
	switch (type)
	{
		case ObjectType::MUSHROOM: rc = { 5*n, 0, n, n}; break;
		case ObjectType::BANANA: rc = { 5*n, n, n, n }; break;
		case ObjectType::CHERRY: rc = { 5*n, 2*n, n, n }; break;
		case ObjectType::ICE_CREAM: rc = { 5*n, 3*n, n, n }; break;
		case ObjectType::FLAM: rc = { 5*n, 4*n, n, n }; break;
		case ObjectType::CAKE: rc = { 5*n, 5*n, n, n }; break;

		default: LOG("Internal error: object creation of invalid type");
	}

	ResourceManager& data = ResourceManager::Instance();
	render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
}
Object::~Object()
{
}
void Object::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
int Object::Points() const
{
	if (type == ObjectType::MUSHROOM)		return POINTS_MUSHROOM;
	else if (type == ObjectType::BANANA)	return POINTS_BANANA;
	else if (type == ObjectType::CHERRY)	return POINTS_CHERRY;
	else if (type == ObjectType::ICE_CREAM)	return POINTS_ICE_CREAM;
	else if (type == ObjectType::FLAM)		return POINTS_FLAM;
	else if (type == ObjectType::CAKE)		return POINTS_CAKE;
	else
	{
		LOG("Internal error: object type invalid when giving points");
		return 0;
	}
}