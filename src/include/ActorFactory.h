#pragma once

#include "ActorMisc.h"
#include "json/json.h"

namespace beasty
{
    class ActorFactory
    {
    public:
        ActorFactory(void);
        StrongActorPtr CreateActor(const char* actorResource);

    protected:
        ActorComponentCreatorMap m_actorComponentCreators;
        virtual StrongActorComponentPtr CreateComponent(const Json::Value& data);
    
    private:
        ActorId GetNextActorId(void) { ++m_lastActorId; return m_lastActorId; }

        ActorId m_lastActorId;
    };
}