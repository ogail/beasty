#include "ActorFactory.h"
#include "Actor.h"
#include "json/json.h"
#include "ActorComponent.h"
#include "RenderComponent.h"
#include <fstream>
#include <assert.h>

using namespace beasty;
using namespace Json;
using namespace std;

StrongActorPtr ActorFactory::CreateActor()
{
    StrongActorPtr actor(new Actor(GetNextActorId()));
    StrongActorComponentPtr component(new RenderComponent());
    component->Init(Json::Value(true));
    actor->AddComponent(component);

    return actor;
}

StrongActorPtr ActorFactory::CreateActor(const char* actorResource)
{
    Value root;
    Reader reader;
    ifstream actorStream(actorResource);
    bool parsingSuccessful = reader.parse(actorStream, root);

    if (!parsingSuccessful)
    {
        throw new logic_error("Failed to create actor for resource: " + string(actorResource));
    }

    // create the actor instance
    StrongActorPtr pActor(new Actor(GetNextActorId()));
    if (!pActor->Init(root))
    {
        throw new logic_error("Failed to initialize actor for resource: " + string(actorResource));
    }

    assert(root.isArray());

    // Loop through each child element and load the component
    for (Value value : root)
    {
        StrongActorComponentPtr pComponent(CreateComponent(value));
        
        if (pComponent)
        {
            pActor->AddComponent(pComponent);
            pComponent->SetOwner(pActor);
        }
        else
        {
            return StrongActorPtr();
        }
    }
    // Now that the actor has been fully created, run the post init phase
    pActor->PostInit();
    
    return pActor;
}

StrongActorComponentPtr ActorFactory::CreateComponent(const Value& data)
{
    assert(data.getMemberNames().size() == 1);
    string name(data.getMemberNames()[0].c_str());
    StrongActorComponentPtr pComponent;
    auto findIt = m_actorComponentCreators.find(name);
    if (findIt != m_actorComponentCreators.end())
    {
        ActorComponentCreator creator = findIt->second;
        pComponent.reset(creator());
    }
    else
    {
        throw new logic_error("Couldn't find ActorComponent named " + name);
    }
    
    // initialize the component if we found one
    if (pComponent)
    {
        if (!pComponent->Init(data))
        {
            throw new logic_error("Component failed to initialize: " + name);
        }
    }

    // pComponent will be NULL if the component wasn't found. This isn't
    // necessarily an error since you might have a custom CreateComponent()
    // function in a subclass.
    return pComponent;
}