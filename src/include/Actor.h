#pragma once

#include "ActorMisc.h"
#include "json/json.h"
#include <memory>

namespace beasty
{
    class Actor
    {
        friend class ActorFactory;

    public:
        ~Actor() {}
        Actor(ActorId id) { m_id = id; }
        bool Init(Json::Value data);
        void PostInit();
        void Destroy();
        void Update(int deltaMs);
        ActorId GetId() const { return m_id; }
        
        template <class ComponentType>
        std::weak_ptr<ComponentType> GetComponent(ComponentId id)
        {
            ActorComponents::iterator findIt = m_components.find(id);
            if (findIt != m_components.end())
            {
                StrongActorComponentPtr pBase(findIt->second);
                // cast to subclass version of the pointer
                shared_ptr<ComponentType> pSub(
                    std::tr1::static_pointer_cast<ComponentType>(pBase));
                std::weak_ptr<ComponentType> pWeakSub(pSub); // convert strong pointer
                                                        // to weak pointer
                return pWeakSub; // return the weak pointer
            }
            else
            {
                return std::weak_ptr<ComponentType>();
            }
        }

    private:
        // This is called by the ActorFactory; no one else should be
        // adding components.
        void AddComponent(StrongActorComponentPtr pComponent);

        ActorId m_id;
        ActorComponents m_components;
    };
}