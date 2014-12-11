#include "GameManager.h"
#include "GraphicsEngine.h"
#include "InputEngine.h"

using namespace beasty;

GameManager::GameManager(std::wstring title) : m_title(title)
{
    m_graphicsEngine = new GraphicsEngine();
}

GameManager::~GameManager()
{
}

int GameManager::init(HINSTANCE hInstance,
                  HINSTANCE hPrevInstance,
                  LPWSTR lpCmdLine,
                  int nCmdShow)
{
    // Enable run-time memory check for debug builds.
#ifdef _DEBUG
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    // DXUT will create and use the best device
    // that is available on the system depending on which D3D callbacks are set below

    // Set general DXUT callbacks
    DXUTSetCallbackFrameMove(GraphicsEngine::onFrameMove);
    DXUTSetCallbackKeyboard(InputEngine::onKeyboard);
    DXUTSetCallbackMsgProc(GameManager::msgProc);
    DXUTSetCallbackDeviceChanging(GraphicsEngine::modifyDeviceSettings);
    DXUTSetCallbackDeviceRemoved(GraphicsEngine::onDeviceRemoved);

    // Set the D3D11 DXUT callbacks. Remove these sets if the app doesn't need to support D3D11
    DXUTSetCallbackD3D11DeviceAcceptable(GraphicsEngine::isD3D11DeviceAcceptable);
    DXUTSetCallbackD3D11DeviceCreated(GraphicsEngine::onD3D11CreateDevice);
    DXUTSetCallbackD3D11SwapChainResized(GraphicsEngine::onD3D11ResizedSwapChain);
    DXUTSetCallbackD3D11FrameRender(GraphicsEngine::onD3D11FrameRender);
    DXUTSetCallbackD3D11SwapChainReleasing(GraphicsEngine::onD3D11ReleasingSwapChain);
    DXUTSetCallbackD3D11DeviceDestroyed(GraphicsEngine::onD3D11DestroyDevice);

    // Perform any application-level initialization here
    DXUTInit( true, true, nullptr ); // Parse the command line, show msgboxes on error, no extra command line params
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    DXUTCreateWindow( m_title.c_str() );

    // Only require 10-level hardware or later
    DXUTCreateDevice( D3D_FEATURE_LEVEL_11_0, true, 800, 600 );
    DXUTMainLoop(); // Enter into the DXUT render loop

    // Perform any application-level cleanup here

    return DXUTGetExitCode();
}

void CALLBACK GameManager::update(double fTime, float fElapsedTime, void* pUserContext)
{
    
}

LRESULT CALLBACK GameManager::msgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext)
{
    return 0;
}