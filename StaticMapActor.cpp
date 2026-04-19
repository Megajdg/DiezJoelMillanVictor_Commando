#include "StaticMapActor.h"
#include "Scene.h"

StaticMapActor::StaticMapActor(Scene* scene) : Actor(scene)
{
    isStatic = true;
    scene->AddActor(this);
}
