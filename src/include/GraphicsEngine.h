#pragma once

#include "DXUT\Core\DXUT.h"

namespace beasty
{
    class GraphicsEngine
    {
    public:
        GraphicsEngine();

        ~GraphicsEngine();
        
        static bool CALLBACK ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext);

        static bool CALLBACK OnDeviceRemoved(void* pUserContext);

        static bool CALLBACK IsD3D11DeviceAcceptable(const CD3D11EnumAdapterInfo *AdapterInfo,
                                                                     UINT Output,
                                                                     const CD3D11EnumDeviceInfo *DeviceInfo,
                                                                     DXGI_FORMAT BackBufferFormat, 
                                                                     bool bWindowed,
                                                                     void* pUserContext);

        static HRESULT CALLBACK OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext);

        static HRESULT CALLBACK OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext);

        static void CALLBACK OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime, float fElapsedTime, void* pUserContext);

        static void CALLBACK OnD3D11ReleasingSwapChain(void* pUserContext);

        static void CALLBACK OnD3D11DestroyDevice(void* pUserContext);

        static void CALLBACK OnFrameMove(double fTime, float fElapsedTime, void* pUserContext);

    private:
        static ID3D11VertexShader* m_pVertexShader;
        static ID3D11PixelShader* m_pPixelShader;
        static ID3D11InputLayout* m_pVertexLayout;
        static ID3D11Buffer* m_pVertexBuffer;
        static ID3D11Buffer* m_pIndexBuffer;
        static ID3D11Buffer* m_pCBChangesEveryFrame;
        static ID3D11ShaderResourceView* m_pTextureRV;
        static ID3D11SamplerState* m_pSamplerLinear;
        static DirectX::XMMATRIX m_World;
        static DirectX::XMMATRIX m_View;
        static DirectX::XMMATRIX m_Projection;
        static DirectX::XMFLOAT4 m_vMeshColor;
    };
}