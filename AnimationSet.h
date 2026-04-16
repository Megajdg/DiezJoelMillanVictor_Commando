#pragma once
#include <unordered_map>
#include <string>
#include "Animation.h"

struct AnimationSet
{
    std::unordered_map<std::string, Animation*> anims;

    Animation* Get(const std::string& name)
    {
        auto it = anims.find(name);
        if (it != anims.end())
            return it->second;
        return nullptr;
    }
};
