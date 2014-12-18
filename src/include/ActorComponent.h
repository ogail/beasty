#pragma once

#include "ActorMisc.h"
#include "json/json.h"

namespace beasty
{
    class ActorComponent
    {
        friend class ActorFactory;

    public:
        ActorComponent() {}
        ~ActorComponent(void) {}
        virtual bool Init(Json::Value data) = 0;
        virtual void PostInit(void) { }
        virtual void Update(int deltaMs) { }
        virtual ComponentId GetComponentId(void) const = 0;

    protected:
        StrongActorPtr m_pOwner;

    private:
        void SetOwner(StrongActorPtr pOwner) { m_pOwner = pOwner; }
    };
}