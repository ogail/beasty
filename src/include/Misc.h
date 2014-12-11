#pragma once

#include "DXUT.h"

namespace beasty
{
    struct SimpleVertex
    {
        DirectX::XMFLOAT3 Pos;
        DirectX::XMFLOAT2 Tex;
    };

    struct CBChangesEveryFrame
    {
        DirectX::XMFLOAT4X4 mWorldViewProj;
        DirectX::XMFLOAT4X4 mWorld;
        DirectX::XMFLOAT4 vMeshColor;
    };
}