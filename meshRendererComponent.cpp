#include "meshRendererComponent.h"
#include "transformComponent.h"
#include "gameObject.h"

void MeshRendererComponent::Draw()
{
    if (m_Mesh == nullptr)
        return;

    //transform取得
    auto transform = GetOwner()->GetComponent<TransformComponent>();
    if (transform == nullptr) { return; }


    //Animation設定
    SPRITE_ANIMATION anim{};
    anim.Frame = 0;
    anim.Columns = 1;
    anim.Rows = 1;
    Renderer::SetSpriteAnimation(anim);


    //ワールド行列設定
    Renderer::SetWorldMatrix(transform->GetWorldMatrix());


    //マテリアルをセット
    MATERIAL material{};
    material.Diffuse = m_Material.color;
    material.TextureEnable = (m_Material.texture != nullptr);
    Renderer::SetMaterial(material);


    //テクスチャをセット
    Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Material.texture);


    m_Mesh->Draw();
}

