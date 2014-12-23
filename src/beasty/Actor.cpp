#include "Actor.h"
#include "ActorComponent.h"

using namespace beasty;
using namespace std;

void Actor::AddComponent(StrongActorComponentPtr pComponent)
{
    m_components.insert(std::make_pair(pComponent->GetId(), pComponent));
}

bool Actor::Init(Json::Value data)
{
    return true;
}

void Actor::PostInit()
{

}

void Actor::Destroy()
{
    for (auto componentPair : m_components)
    {
        componentPair.second->~ActorComponent();
    }
}