#include "Shot.h"
#include "StaticImage.h"


Shot::Shot(const Point& p, const Point& d, int width, int heigth, int frame_width, int frame_height) :
	Entity (p, d, width, heigth, frame_width, frame_height)
{
	map = nullptr;
}
Shot::~Shot()
{
}
void Shot::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
void Shot::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
bool Shot::IsMovingLeft() const
{
	return dir.x < 0;
}
bool Shot::IsMovingRight() const
{
	return dir.x > 0;
}
