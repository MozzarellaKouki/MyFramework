#pragma once
#include "component.h"
#include "polygon2D.h"
#include "textureManager.h"


class Sprite2DComponent : public Component
{
private:
	Polygon2D* m_Sprite{};


public:

	~Sprite2DComponent() { delete m_Sprite; }

	void SetSprite(const wchar_t* path);

};
 

