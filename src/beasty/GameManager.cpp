#include "GameManager.h"
#include "GraphicsEngine.h"
#include "InputEngine.h"
#include <string>

using namespace beasty;
using namespace std;

GameManager::GameManager() :
    m_hasModalDialog(0),
    m_quitting(false)

{
}

GameManager::~GameManager()
{
    m_game->~Game3D();
}

int GameManager::Init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, wstring title)
{
    // Enable run-time memory check for debug builds.
#ifdef _DEBUG
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    // DXUT will create and use the best device
    // that is available on the system depending on which D3D callbacks are set below

    // Set general DXUT callbacks
    DXUTSetCallbackFrameMove(GameManager::Update);
    DXUTSetCallbackKeyboard(InputEngine::OnKeyboard);
    DXUTSetCallbackMsgProc(GameManager::MsgProc);
    DXUTSetCallbackDeviceChanging(GraphicsEngine::ModifyDeviceSettings);
    DXUTSetCallbackDeviceRemoved(GraphicsEngine::OnDeviceRemoved);

    // Set the D3D11 DXUT callbacks. Remove these sets if the app doesn't need to support D3D11
    DXUTSetCallbackD3D11DeviceAcceptable(GraphicsEngine::IsD3D11DeviceAcceptable);
    DXUTSetCallbackD3D11DeviceCreated(GraphicsEngine::OnD3D11CreateDevice);
    DXUTSetCallbackD3D11SwapChainResized(GraphicsEngine::OnD3D11ResizedSwapChain);
    DXUTSetCallbackD3D11FrameRender(GraphicsEngine::OnD3D11FrameRender);
    DXUTSetCallbackD3D11SwapChainReleasing(GraphicsEngine::OnD3D11ReleasingSwapChain);
    DXUTSetCallbackD3D11DeviceDestroyed(GraphicsEngine::OnD3D11DestroyDevice);

    // Perform any application-level initialization here
    DXUTInit( true, true, nullptr ); // Parse the command line, show msgboxes on error, no extra command line params
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    DXUTCreateWindow( m_title.c_str() );

    // Only require 10-level hardware or later
    DXUTCreateDevice( D3D_FEATURE_LEVEL_11_0, true, 800, 600 );
    DXUTMainLoop(); // Enter into the DXUT render loop

    m_title = title;

    // Perform any application-level cleanup here

    return DXUTGetExitCode();
}

void CALLBACK GameManager::Update(double fTime, float fElapsedTime, void* pUserContext)
{
    GraphicsEngine::OnFrameMove(fTime, fElapsedTime, pUserContext);

    if (GameManager::Instance()->HasModalDialog())
    {	
        // don't update the game if a modal dialog is up.
        return;
    }

    if (GameManager::Instance()->Quitting())
    {
        PostMessage(GameManager::Instance()->GetHwnd(), WM_CLOSE, 0, 0);
    }

    if (GameManager::Instance()->Game())
    {
        GameManager::Instance()->Game()->Update(float(fTime), fElapsedTime);
    }
}

LRESULT CALLBACK GameManager::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext)
{
    return 0;
}