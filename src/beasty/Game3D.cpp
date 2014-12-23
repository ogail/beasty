#include "Game3D.h"
#include "GameManager.h"
#include "Actor.h"

using namespace beasty;
using namespace std;

int Game3D::Init(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, std::wstring title)
{
    GameManager::Instance()->Game(this);
    return GameManager::Instance()->Init(hInstance, hPrevInstance, lpCmdLine, nCmdShow, title);
}

void Game3D::AddActor(StrongActorPtr actor)
{
    m_actors.insert(make_pair(actor->GetId(), actor));
}

Game3D::~Game3D()
{
    for (auto actorPair : m_actors)
    {
        actorPair.second->Destroy();
    }
}