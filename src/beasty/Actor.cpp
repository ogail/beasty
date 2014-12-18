#include "Actor.h"

using namespace beasty;
using namespace std;

template <class ComponentType>
weak_ptr<ComponentType> Actor::GetComponent(ComponentId id)
{
    ActorComponents::iterator findIt = m_components.find(id);
    if (findIt != m_components.end())
    {
        StrongActorComponentPtr pBase(findIt->second);
        // cast to subclass version of the pointer
        shared_ptr<ComponentType> pSub(
            std::tr1::static_pointer_cast<ComponentType>(pBase));
        weak_ptr<ComponentType> pWeakSub(pSub); // convert strong pointer
        // to weak pointer
        return pWeakSub; // return the weak pointer
    }
    else
    {
        return weak_ptr<ComponentType>();
    }
}