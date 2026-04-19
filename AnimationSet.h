#pragma once
#include <unordered_map>
#include <string>
#include "Animation.h"

/// <summary>
/// Define un contenedor de animaciones
/// </summary>
struct AnimationSet
{
    std::unordered_map<std::string, Animation*> anims;  // Vector de animaciones por nombre

    // Busca la animacion por nombre, si existe devuelve el puntero, si no, null
    Animation* Get(const std::string& name)
    {
        auto it = anims.find(name);
        if (it != anims.end())
            return it->second;
        return nullptr;
    }
};