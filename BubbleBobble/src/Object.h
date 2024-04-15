#pragma once
#include "Entity.h"

#define OBJECT_PHYSICAL_SIZE	16
#define OBJECT_FRAME_SIZE		16

#define POINTS_MUSHROOM		100
#define POINTS_BANANA		500
#define POINTS_CHERRY		700
#define POINTS_ICE_CREAM	900
#define POINTS_FLAM			2000
#define POINTS_CAKE			2000

enum class ObjectType { MUSHROOM, BANANA, CHERRY, ICE_CREAM, FLAM, CAKE};

class Object : public Entity
{
public:
	Object(const Point& p, ObjectType t);
	~Object();

	void DrawDebug(const Color& col) const;
	int Points() const;

private:
	ObjectType type;
};

