#include "GraphicsEngine.h"
#include "GameManager.h"
#include "Actor.h"
#include "RenderComponent.h"
#include "DXUT\Optional\SDKmisc.h"
#include "Misc.h"

using namespace beasty;
using namespace std;
using namespace DirectX;

ID3D11InputLayout* GraphicsEngine::m_pVertexLayout = nullptr;
ID3D11SamplerState* GraphicsEngine::m_pSamplerLinear = nullptr;
XMMATRIX GraphicsEngine::m_World;
XMMATRIX GraphicsEngine::m_View;
XMMATRIX GraphicsEngine::m_Projection;
XMFLOAT4 GraphicsEngine::m_vMeshColor(0.7f, 0.7f, 0.7f, 1.0f);

GraphicsEngine::GraphicsEngine()
{
}

GraphicsEngine::~GraphicsEngine()
{
}

bool CALLBACK GraphicsEngine::ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext)
{
    return true;
}

bool CALLBACK GraphicsEngine::OnDeviceRemoved(void* pUserContext)
{
    return true;
}

bool CALLBACK GraphicsEngine::IsD3D11DeviceAcceptable(const CD3D11EnumAdapterInfo *AdapterInfo,
                                                      UINT Output,
                                                      const CD3D11EnumDeviceInfo *DeviceInfo,
                                                      DXGI_FORMAT BackBufferFormat, 
                                                      bool bWindowed,
                                                      void* pUserContext)
{
    return true;
}

HRESULT CALLBACK GraphicsEngine::OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
    HRESULT hr = S_OK;

    auto pd3dImmediateContext = DXUTGetD3D11DeviceContext();

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;

    // Disable optimizations to further improve shader debugging
    dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    // Initialize the world matrices
    m_World = XMMatrixIdentity();

    // Initialize the view matrix
    static const XMVECTORF32 s_Eye = { 0.0f, 3.0f, -6.0f, 0.f };
    static const XMVECTORF32 s_At = { 0.0f, 1.0f, 0.0f, 0.f };
    static const XMVECTORF32 s_Up = { 0.0f, 1.0f, 0.0f, 0.f };
    m_View = XMMatrixLookAtLH(s_Eye, s_At, s_Up);

    // Create the sample state
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    V_RETURN(pd3dDevice->CreateSamplerState(&sampDesc, &m_pSamplerLinear));

    ActorMap actors = GameManager::Instance()->Game()->GetActors();

    for (pair<ActorId, StrongActorPtr> actorPair : actors)
    {
        weak_ptr<RenderComponent> renderComponent = actorPair.second->GetComponent<RenderComponent>(0);
        if (renderComponent.expired())
        {
            continue;
        }

        // Compile the vertex shader
        ID3DBlob* pVSBlob = nullptr;
        auto comp = renderComponent.lock();
        V_RETURN(DXUTCompileFromFile(comp->m_vertexShaderName.c_str(), nullptr, "VS", "vs_4_0", dwShaderFlags, 0, &pVSBlob));

        // Create the vertex shader
        hr = pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &comp->m_pVertexShader);
        if (FAILED(hr))
        {
            SAFE_RELEASE(pVSBlob);
            return hr;
        }

        // Define the input layout
        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        UINT numElements = ARRAYSIZE(layout);

        // Create the input layout
        hr = pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
            pVSBlob->GetBufferSize(), &m_pVertexLayout);
        SAFE_RELEASE(pVSBlob);
        if (FAILED(hr))
            return hr;

        // Set the input layout
        pd3dImmediateContext->IASetInputLayout(m_pVertexLayout);

        // Compile the pixel shader
        ID3DBlob* pPSBlob = nullptr;
        V_RETURN(DXUTCompileFromFile(comp->m_pixelShaderName.c_str(), nullptr, "PS", "ps_4_0", dwShaderFlags, 0, &pPSBlob));

        // Create the pixel shader
        hr = pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &comp->m_pPixelShader);
        SAFE_RELEASE(pPSBlob);
        if (FAILED(hr))
            return hr;

        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(SimpleVertex) * 24;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;
        D3D11_SUBRESOURCE_DATA InitData;
        ZeroMemory(&InitData, sizeof(InitData));
        InitData.pSysMem = &comp->m_vertices[0];
        V_RETURN(pd3dDevice->CreateBuffer(&bd, &InitData, &comp->m_pVertexBuffer));

        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(DWORD) * 36;
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.CPUAccessFlags = 0;
        bd.MiscFlags = 0;
        InitData.pSysMem = &comp->m_indices[0];
        V_RETURN(pd3dDevice->CreateBuffer(&bd, &InitData, &comp->m_pIndexBuffer));

        // Create the constant buffers
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.ByteWidth = sizeof(CBChangesEveryFrame);
        V_RETURN(pd3dDevice->CreateBuffer(&bd, nullptr, &comp->m_pCBChangesEveryFrame));
        V_RETURN(DXUTCreateShaderResourceViewFromFile(pd3dDevice, comp->m_textureName.c_str(), &comp->m_pTextureRV));
    }

    return S_OK;
}

HRESULT CALLBACK GraphicsEngine::OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
    // Setup the projection parameters
    float fAspect = static_cast<float>( pBackBufferSurfaceDesc->Width ) / static_cast<float>( pBackBufferSurfaceDesc->Height );
    m_Projection = XMMatrixPerspectiveFovLH( XM_PI * 0.25f, fAspect, 0.1f, 100.0f );

    return S_OK;
}

void CALLBACK GraphicsEngine::OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime, void* pUserContext)
{
    //
    // Clear the depth stencil
    //
    auto pDSV = DXUTGetD3D11DepthStencilView();
    pd3dImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0, 0);

    XMMATRIX mWorldViewProjection = m_World * m_View * m_Projection;

    for (pair<ActorId, StrongActorPtr> actorPair : GameManager::Instance()->Game()->GetActors())
    {
        weak_ptr<RenderComponent> renderComponent = actorPair.second->GetComponent<RenderComponent>(0);
        if (renderComponent.expired())
        {
            continue;
        }

        auto comp = renderComponent.lock();

        //
        // Clear the back buffer
        //
        auto pRTV = DXUTGetD3D11RenderTargetView();
        pd3dImmediateContext->ClearRenderTargetView(pRTV, comp->m_backgroundColor);

        // Set vertex buffer
        UINT stride = sizeof(SimpleVertex);
        UINT offset = 0;
        pd3dImmediateContext->IASetVertexBuffers(0, 1, &comp->m_pVertexBuffer, &stride, &offset);

        // Set index buffer
        pd3dImmediateContext->IASetIndexBuffer(comp->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

        // Set primitive topology
        pd3dImmediateContext->IASetPrimitiveTopology(comp->m_topology);

        // Update constant buffer that changes once per frame
        HRESULT hr;
        D3D11_MAPPED_SUBRESOURCE MappedResource;
        V(pd3dImmediateContext->Map(comp->m_pCBChangesEveryFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource));
        auto pCB = reinterpret_cast<CBChangesEveryFrame*>(MappedResource.pData);
        XMStoreFloat4x4(&pCB->mWorldViewProj, XMMatrixTranspose(mWorldViewProjection));
        XMStoreFloat4x4(&pCB->mWorld, XMMatrixTranspose(m_World));
        pCB->vMeshColor = m_vMeshColor;
        pd3dImmediateContext->Unmap(comp->m_pCBChangesEveryFrame, 0);

        //
        // Render the cube
        //
        pd3dImmediateContext->VSSetShader(comp->m_pVertexShader, nullptr, 0);
        pd3dImmediateContext->VSSetConstantBuffers(0, 1, &comp->m_pCBChangesEveryFrame);
        pd3dImmediateContext->PSSetShader(comp->m_pPixelShader, nullptr, 0);
        pd3dImmediateContext->PSSetConstantBuffers(0, 1, &comp->m_pCBChangesEveryFrame);
        pd3dImmediateContext->PSSetShaderResources(0, 1, &comp->m_pTextureRV);
        pd3dImmediateContext->PSSetSamplers(0, 1, &m_pSamplerLinear);
        pd3dImmediateContext->DrawIndexed(36, 0, 0);
    }
}

void CALLBACK GraphicsEngine::OnD3D11ReleasingSwapChain(void* pUserContext)
{

}

void CALLBACK GraphicsEngine::OnD3D11DestroyDevice(void* pUserContext)
{
    GameManager::Instance()->~GameManager();
    SAFE_RELEASE( m_pVertexLayout );
    SAFE_RELEASE( m_pSamplerLinear );
}

void CALLBACK GraphicsEngine::OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{
    // Rotate cube around the origin
    m_World = XMMatrixRotationY( 60.0f * XMConvertToRadians((float)fTime) );

    // Modify the color
    m_vMeshColor.x = ( sinf( ( float )fTime * 1.0f ) + 1.0f ) * 0.5f;
    m_vMeshColor.y = ( cosf( ( float )fTime * 3.0f ) + 1.0f ) * 0.5f;
    m_vMeshColor.z = ( sinf( ( float )fTime * 5.0f ) + 1.0f ) * 0.5f;
}