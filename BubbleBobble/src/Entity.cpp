
#include "Entity.h"
#include <cmath>
#include "Globals.h"
#include "Sprite.h"

Entity::Entity() :
	pos({ 0,0 }), dir({ 0,0 }), width(0), height(0), frame_width(0), frame_height(0), render(nullptr)
{
	alive = false;
}
Entity::Entity(const Point& p, int w, int h) :
	pos(p), dir({ 0,0 }), width(w), height(h), frame_width(w), frame_height(h), render(nullptr)
{
	alive = true;
}
Entity::Entity(const Point& p, int w, int h, int frame_w, int frame_h) :
	pos(p), dir({ 0,0 }), width(w), height(h), frame_width(frame_w), frame_height(frame_h), render(nullptr)
{
	alive = true;
}
Entity::Entity(const Point& p, const Point& d, int w, int h, int frame_w, int frame_h) :
	pos(p), dir(d), width(w), height(h), frame_width(frame_w), frame_height(frame_h), render(nullptr)
{
	alive = true;
}
Entity::~Entity()
{
	if (render != nullptr)
	{
		delete render;
		render = nullptr;
	}
}

AppStatus Entity::Animations() {


	float i;
	const int n = 16;
	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_PLAYER, "images/bubMove.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	render = new Sprite(data.GetTexture(Resource::IMG_PLAYER));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)EntityAnim::NUM_ANIMATIONS);

	/*sprite->SetAnimationDelay((int)EntityAnim::BUBCINEMATIC, ANIM_DELAY);
	for (i = 0;i <3;++i) {

		sprite->AddKeyFrame((int)EntityAnim::BUBCINEMATIC, { i * n, 16 * n, 2 * n, 2 * n });

	}*/
	
	sprite->SetAnimationDelay((int)EntityAnim::BUBCINEMATIC, ANIM_DELAY);
	sprite->AddKeyFrame((int)EntityAnim::BUBCINEMATIC, { 0 * n, 16 * n, 2 * n, 2 * n });
	sprite->AddKeyFrame((int)EntityAnim::BUBCINEMATIC, { 2 * n, 16 * n, 2 * n, 2 * n });
	sprite->AddKeyFrame((int)EntityAnim::BUBCINEMATIC, { 4 * n, 16 * n, 2 * n, 2 * n });
}
void Entity::SetAnimationEntity(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);

}
void Entity::DrawAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
	Draw();
}

void Entity::SetPos(const Point& p)
{
	pos = p;
}
void Entity::Set(const Point& p, const Point& d, int w, int h, int framew, int frameh)
{
	pos = p;
	dir = d;
	width = w;
	height = h;
	frame_width = framew;
	frame_height = frameh;
	alive = true;
}
void Entity::Update()
{
	pos += dir;
}
AABB Entity::GetHitbox() const
{
	Point p(pos.x, pos.y - (height - 1));
	AABB hitbox(p, width, height);
	return hitbox;
}
Point Entity::GetPos() const
{
	Point p(pos.x, pos.y);
	return p;
}
Point Entity::GetDir() const
{
	Point p(dir.x, dir.y);
	return p;
}
void Entity::SetAlive(bool b)
{
	alive = b;
}
bool Entity::IsAlive() const
{
	return alive;
}
Point Entity::GetRenderingPosition() const
{
	Point p;
	p.x = pos.x + width / 2 - frame_width / 2;
	p.y = pos.y - (frame_height-1);
	return p;
}
void Entity::Draw() const
{
	Point p = GetRenderingPosition();
	render->Draw(p.x, p.y);
}
void Entity::DrawTint(const Color& col) const
{
	Point p = GetRenderingPosition();
	render->DrawTint(p.x, p.y, col);
}
void Entity::DrawHitbox(const Color& col) const
{
	Color c = col;
	c.a = 128;		//50% transparent

	render->DrawBox(pos.x, pos.y-(height-1), width, height, c);
	render->DrawCorners(pos.x, pos.y-(height-1), width, height);
}
void Entity::DrawHitbox(int x, int y, int w, int h, const Color& col) const
{
	Color c = col;
	c.a = 128;		//50% transparent

	render->DrawBox(x, y-(h-1), w, h, c);
	render->DrawCorners(x, y-(h-1), w, h);
}