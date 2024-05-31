
#pragma once
#include <raylib.h>
#include "Point.h"
#include "ResourceManager.h"
#include "RenderComponent.h"
#include "AABB.h"

enum class Look { RIGHT, LEFT };
enum class EntityAnim {
	
	BUBCINEMATIC,
	BOBCINEMATIC,
	NUM_ANIMATIONS

	


};
class Entity
{
public:
	Entity();
	Entity(const Point& p, int width, int height);
	Entity(const Point& p, int width, int height, int frame_width, int frame_height);
	Entity(const Point& p, const Point& d, int width, int height, int frame_width, int frame_height);
	virtual ~Entity();

	void Set(const Point& p, const Point& d, int w, int h, int framew, int frameh);
	void SetPos(const Point& p);
	void Update();
	AppStatus Animations();
	void SetAnimationEntity(int id);
	void DrawAnimation();
	void Warp();
	AABB GetHitbox() const;
	Point GetPos() const;
	Point GetDir() const;

	void SetAlive(bool b);
	bool IsAlive() const;

	//Draw representation model
	void Draw() const;
	void DrawTint(const Color& col) const;
	
	//Draw logical/physical model (hitbox)
	void DrawHitbox(const Color& col) const;
	void DrawHitbox(int x, int y, int w, int h, const Color& col) const;

protected:
	Point GetRenderingPosition() const;

	//Logical/physical model
	Point pos, dir;
	int width, height;				

	//Representation model
	int frame_width, frame_height;

	RenderComponent *render;

	//Flag to mark wether an entity is active or inactive. Trick to manage dynamic arrays of
	//entities with static arrays without new/delete operations
	bool alive;
};