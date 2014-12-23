#pragma once

#include "DXUT\Core\DXUT.h"
#include "Game3D.h"

namespace beasty
{
    class GraphicsEngine;
    class InputEngine;

    class GameManager
    {
    public:
        static GameManager* Instance() { static GameManager *pGameManager = new GameManager(); return pGameManager; }
        static void CALLBACK Update(double fTime, float fElapsedTime, void* pUserContext);
        static LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext);

        GameManager();
        ~GameManager();
        bool HasModalDialog() const { return m_hasModalDialog != 0; }
        bool Quitting() const { return m_quitting; }
        int Init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, std::wstring title);
        HWND GetHwnd() const { return DXUTGetHWND(); }
        Game3D* Game() const { return m_game; }
        void Game(Game3D* game) { m_game = game; }

    protected:
        bool m_quitting; // true if the app is running the exit sequence
        int m_hasModalDialog; // determines if a modal dialog is up

    private:
        std::wstring m_title;
        Game3D *m_game;
    };
}