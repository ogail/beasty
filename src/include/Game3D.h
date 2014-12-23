#pragma once

#include "ActorMisc.h"
#include <Windows.h>
#include <string>
#include <vector>

namespace beasty
{
    class Game3D
    {
    public:
        void AddActor(StrongActorPtr actor);
        const ActorMap& GetActors() const { return m_actors; }
        ~Game3D();
        virtual void Update(float fTime, float fElapsedTime) = 0;
        virtual int Init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, std::wstring title);

    private:
        ActorMap m_actors;
    };
}