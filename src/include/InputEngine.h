#pragma once

#include "DXUT\Core\DXUT.h"

namespace beasty
{
    class InputEngine
    {
    public:
        static void CALLBACK OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext);
    };
}