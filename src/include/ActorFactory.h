#pragma once

#include "ActorMisc.h"
#include "json/json.h"

namespace beasty
{
    class ActorFactory
    {
    public:
        static ActorFactory GetInstance() { static ActorFactory m_instance; return m_instance; }
        ActorFactory() {}
        StrongActorPtr CreateActor(const char* actorResource);
        StrongActorPtr CreateActor();

    protected:
        ActorComponentCreatorMap m_actorComponentCreators;
        virtual StrongActorComponentPtr CreateComponent(const Json::Value& data);
    
    private:
        ActorId GetNextActorId() { ++m_lastActorId; return m_lastActorId; }

        ActorId m_lastActorId;
    };
}