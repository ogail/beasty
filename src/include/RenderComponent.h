#pragma once

#include "ActorComponent.h"
#include "Misc.h"
#include <vector>

namespace beasty
{
    class RenderComponent : public ActorComponent
    {
    public:
        ~RenderComponent();
        bool Init(Json::Value data);
        ComponentId GetId() const { return 0; }

        std::wstring m_vertexShaderName;
        std::wstring m_pixelShaderName;
        std::wstring m_textureName;
        std::vector<SimpleVertex> m_vertices;
        std::vector<DWORD> m_indices;
        D3D11_PRIMITIVE_TOPOLOGY m_topology;
        DirectX::XMVECTORF32 m_backgroundColor;
        ID3D11VertexShader* m_pVertexShader;
        ID3D11PixelShader* m_pPixelShader;
        ID3D11ShaderResourceView* m_pTextureRV;
        ID3D11Buffer* m_pVertexBuffer;
        ID3D11Buffer* m_pIndexBuffer;
        ID3D11Buffer* m_pCBChangesEveryFrame;
    };
}