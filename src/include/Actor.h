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
        ~Actor(void);
        explicit Actor(ActorId id);
        bool Init(Json::Value data);
        void PostInit(void);
        void Destroy(void);
        void Update(int deltaMs);
        ActorId GetId(void) const { return m_id; }
        
        template <class ComponentType>
        std::weak_ptr<ComponentType> GetComponent(ComponentId id);

    private:
        // This is called by the ActorFactory; no one else should be
        // adding components.
        void AddComponent(StrongActorComponentPtr pComponent);

        ActorId m_id;
        ActorComponents m_components;
    };
}