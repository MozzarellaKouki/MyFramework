#include "meshRendererComponent.h"
#include "transformComponent.h"
#include "gameObject.h"
#include "renderer.h"

ID3D11InputLayout* MeshRendererComponent::s_VertexLayout = nullptr;
ID3D11VertexShader* MeshRendererComponent::s_VertexShader = nullptr;
ID3D11PixelShader* MeshRendererComponent::s_PixelShader = nullptr;
unsigned int MeshRendererComponent::s_ReferenceCount = 0;

void MeshRendererComponent::Init()
{
    if (m_Initialized)
        return;

    if (s_ReferenceCount == 0)
    {
        Renderer::CreateVertexShader(
            &s_VertexShader,
            &s_VertexLayout,
            "Shader//unlitTextureVS.cso");
        Renderer::CreatePixelShader(
            &s_PixelShader,
            "Shader//unlitTexturePS.cso");
    }

    ++s_ReferenceCount;
    m_Initialized = true;
}

void MeshRendererComponent::Uninit()
{
    if (!m_Initialized)
        return;

    m_Initialized = false;
    if (s_ReferenceCount > 0)
        --s_ReferenceCount;

    if (s_ReferenceCount == 0)
    {
        if (s_PixelShader)
        {
            s_PixelShader->Release();
            s_PixelShader = nullptr;
        }
        if (s_VertexShader)
        {
            s_VertexShader->Release();
            s_VertexShader = nullptr;
        }
        if (s_VertexLayout)
        {
            s_VertexLayout->Release();
            s_VertexLayout = nullptr;
        }
    }
}

void MeshRendererComponent::Draw()
{
    if (!m_Initialized || !m_Mesh || !GetOwner())
        return;

    TransformComponent* transform = GetOwner()->GetComponent<TransformComponent>();
    if (!transform)
        return;

    ID3D11DeviceContext* context = Renderer::GetDeviceContext();
    context->IASetInputLayout(s_VertexLayout);
    context->VSSetShader(s_VertexShader, nullptr, 0);
    context->PSSetShader(s_PixelShader, nullptr, 0);

    SPRITE_ANIMATION animation{};
    animation.Frame = 0;
    animation.Columns = 1;
    animation.Rows = 1;
    Renderer::SetSpriteAnimation(animation);
    Renderer::SetWorldMatrix(transform->GetWorldMatrix());

    MATERIAL material{};
    material.Diffuse = m_Material.color;
    material.TextureEnable = (m_Material.texture != nullptr);
    Renderer::SetMaterial(material);
    context->PSSetShaderResources(0, 1, &m_Material.texture);

    m_Mesh->Draw();
}