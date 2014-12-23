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
        ~ActorComponent() {}
        virtual bool Init(Json::Value data) = 0;
        virtual void PostInit() { }
        virtual void Update(int deltaMs) { }
        virtual ComponentId GetId() const = 0;

    protected:
        StrongActorPtr m_pOwner;

    private:
        void SetOwner(StrongActorPtr pOwner) { m_pOwner = pOwner; }
    };
}