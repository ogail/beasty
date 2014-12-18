#pragma once

#include <map>
#include <memory>

namespace beasty
{
    class ActorComponent;
    class Actor;

    typedef ActorComponent *(*ActorComponentCreator)(void);
    typedef std::map<std::string, ActorComponentCreator> ActorComponentCreatorMap;    
    typedef unsigned long ActorId;
    typedef unsigned int ComponentId;
    typedef std::shared_ptr<Actor> StrongActorPtr;
    typedef std::shared_ptr<ActorComponent> StrongActorComponentPtr;
    typedef std::map<ComponentId, StrongActorComponentPtr> ActorComponents;
}