#include "sprite3DComponent.h"
#include "gameObject.h"
#include "renderer.h"
#include "textureManager.h"
#include "cameraManager.h"
#include "camera.h"

#include "transformComponent.h"
#include "spriteAnimationComponent.h"
#include "transparentRenderManager.h"

#include "debugUtils.h"


std::vector<Sprite3DComponent*> Sprite3DComponent::s_TransparentSprites;


void Sprite3DComponent::Init()
{
    m_Transform = GetOwner()->GetComponent<TransformComponent>();
}


void Sprite3DComponent::Draw()
{
    //通常描画用。距離ソート対象ならここでは描画しない
    if (m_SortByDistance)
        return;

    if (!m_Transform)
        return;

    ApplyBillboard(m_Transform);
    ApplyAnimation();

    m_Sprite->Draw();
}

//============================
//透過Render関数
//============================

void Sprite3DComponent::SubmitTransparentSprites()
{
    //透明スプライトはここで透明描画マネージャーへ登録
    for (auto* sprite : s_TransparentSprites)
    {
        if (!sprite->IsVisible()) continue;

        TransparentRenderManager::RegisterSprite(sprite);
    }
}

void Sprite3DComponent::RenderTransparent()
{
    //透過スプライト用の描画処理。透明描画マネージャーから呼ばれる。
    if (!m_Transform)
        return;

    ApplyBillboard(m_Transform);
    ApplyAnimation();

    m_Sprite->Draw();
}

//============================
//Apply関数
//============================

void Sprite3DComponent::ApplyAnimation()
{

    // デフォルト値
    SPRITE_ANIMATION data{};
    data.Frame = 0;
    data.Columns = 1;
    data.Rows = 1;

   
 
    auto* anim = GetOwner()->GetComponent<SpriteAnimationComponent>();
    if(anim)
    {
        data.Frame = anim->GetCurrentFrame();
        data.Columns = anim->GetColumns();
        data.Rows = anim->GetRows();
    }

    Renderer::SetSpriteAnimation(data);
}



void Sprite3DComponent::ApplyBillboard(TransformComponent* transform)
{
    switch (m_BillboardType)
    {
    case BillboardType::None:
    {
        Renderer::SetWorldMatrix(transform->GetWorldMatrix());
        break;
    }

    case BillboardType::Full:
    {

        Camera* cam = CameraManager::GetInstance().GetCamera();
        if (!cam)
        {
            Renderer::SetWorldMatrix(transform->GetWorldMatrix());
            return;
        }

        XMMATRIX view = cam->GetViewMatrix();


        XMMATRIX invView = XMMatrixInverse(nullptr, view);
        Vector3 pos = transform->GetPosition();
        Vector3 scale = transform->GetScale();


        //スケール
        XMMATRIX s = XMMatrixScaling(scale.x, scale.y, scale.z);

        //回転行列
        XMMATRIX rot = invView;
        rot.r[3] = XMVectorSet(0, 0, 0, 1); //平行移動を消す

        //平行移動
        XMMATRIX t = XMMatrixTranslation(pos.x, pos.y, pos.z);

        //S → R → T
        XMMATRIX world = s * rot * t;

        Renderer::SetWorldMatrix(world);
        break;
    }


    case BillboardType::Horizontal:
    {
        Vector3 pos = transform->GetPosition();
        Vector3 scale = transform->GetScale();

        //スケール
        XMMATRIX s = XMMatrixScaling(
            scale.x,
            scale.y,
            scale.z
        );


        //地面に寝かせる回転
        XMMATRIX rot =
            XMMatrixRotationX(XMConvertToRadians(90.0f));


        //座標
        XMMATRIX t =
            XMMatrixTranslation(
                pos.x,
                pos.y,
                pos.z
            );


        Renderer::SetWorldMatrix(
            s * rot * t
        );

        break;
    }
     
    }
}



//Setter
void Sprite3DComponent::SetSprite(const wchar_t* path)
{
    m_Sprite = new Polygon3D();
    m_Sprite->Init();
    auto tex = TextureManager::Load(path);
    m_Sprite->SetTexture(tex);
}

void  Sprite3DComponent::SetBillboard(BillboardType type)
{
    m_BillboardType = type;
}

void Sprite3DComponent::SetSortByDistance(bool enable)
{
    if (m_SortByDistance == enable)
        return;

    m_SortByDistance = enable;

    if (enable)
    {
        RegisterTransparentSprite();
    }
    else
    {
        UnregisterTransparentSprite();
    }
}


Vector3  Sprite3DComponent::GetPosition() const
{
    if (!m_Transform)
    {
        return Vector3(0, 0, 0);
    }

    return m_Transform->GetPosition();
}



Polygon3D* Sprite3DComponent::GetPolygon()
{
    return m_Sprite;
}

BillboardType  Sprite3DComponent::GetBillboard() const
{
    return m_BillboardType;
}


//=================================
//　透過スプライトの登録・削除・描写
//=================================

void Sprite3DComponent::RegisterTransparentSprite()
{
    s_TransparentSprites.push_back(this);
}



void Sprite3DComponent::UnregisterTransparentSprite()
{
    auto it = std::find(
        s_TransparentSprites.begin(),
        s_TransparentSprites.end(),
        this);

    if (it != s_TransparentSprites.end())
    {
        s_TransparentSprites.erase(it);
    }

}
