#pragma once

namespace beasty
{
    class Game3D
    {
    public:
        virtual void Update(float fTime, float fElapsedTime) = 0;
    };
}