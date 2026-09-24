#include "explosion.h"
#include "gameObjectManager.h"
#include "transformComponent.h"

#include "sprite3DComponent.h"

#include "debugUtils.h"

void Explosion::Init()
{
    auto* transform = AddComponent<TransformComponent>();
    auto* sprite = AddComponent<Sprite3DComponent>();
  


    transform->SetPosition({ 0, 0, 10 });
    transform->SetScale({ 10, 10, 10 });

    sprite->SetSprite(L"Asset/Texture/2DBillboard/Explosion.png");
    sprite->SetBillboard(BillboardType::Full);
    sprite->SetSortByDistance(true);

    m_Animation = AddComponent<SpriteAnimationComponent>();

  

    m_Animation->SetGrid(4, 4);
    m_Animation->SetFrameTime(0.05f);
    m_Animation->SetMaxFrame(15);


}


void Explosion::Update(float deltaTime)
{
    GameObject::Update(deltaTime);
}