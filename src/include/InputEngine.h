#pragma once

#include "DXUT.h"

namespace beasty
{
    class InputEngine
    {
    public:
        static void CALLBACK onKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext);
    };
}