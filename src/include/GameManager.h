#pragma once

#include "DXUT\Core\DXUT.h"

namespace beasty
{
    class GraphicsEngine;
    class InputEngine;

    class GameManager
    {
    public:
        GameManager(std::wstring title);
        ~GameManager();

        static void CALLBACK Update(double fTime, float fElapsedTime, void* pUserContext);

        virtual int Init(HINSTANCE hInstance,
                         HINSTANCE hPrevInstance,
                         LPWSTR lpCmdLine,
                         int nCmdShow);

        static LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext);

    private:
        std::wstring m_title;
        GraphicsEngine *m_graphicsEngine;
        InputEngine *m_inputEngine;
    };
}