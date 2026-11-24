#pragma once

#include "component.h"
#include "polygon2D.h"
#include "transform.h"
#include "spriteAnimationComponent.h"

#include <vector>
#include <memory>

class Sprite2DComponent : public Component
{
private:
	std::unique_ptr<Polygon2D> m_Sprite{};
	Transform* m_Transform{};

	bool m_Visible = true;
	

public:
	void Init() override;
	void Draw() override;

	void SetSprite(const wchar_t* path);
	void ApplyAnimation();

};
 

