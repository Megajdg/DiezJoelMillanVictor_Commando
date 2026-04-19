#include "StaticMapActor.h"
#include "Scene.h"

// Constructor del mapa estatico, llama al constructor, marca al actor como estatico y lo registra en escena
StaticMapActor::StaticMapActor(Scene* scene) : Actor(scene)
{
    isStatic = true;        // El mapa no se mueve ni debe ser empujado por fisicas
    scene->AddActor(this);  // Registra el mapa en la escena para que exista desde el frame 0
}