#pragma once

#include "Sprite.h"
#include "AnimationSet.h"
#include "Animation.h"
#include <string>

class AnimatedEntity : public Sprite
{
public:
    AnimationSet animationSet;
    Animation* currentAnimation = nullptr;

    AnimatedEntity(Scene* scene, std::string img, Transform t, Vector2 size)
        : Sprite(scene, img, t, size) {
    }

    AnimatedEntity(Scene* scene, std::string img, Vector2 size)
        : Sprite(scene, img, size) {
    }

    virtual void SetAnimation(const std::string& name)
    {
        Animation* a = animationSet.Get(name);
        if (!a) return;

        if (a != currentAnimation)
        {
            currentAnimation = a;
            currentAnimation->Reset();
        }
    }

    virtual void Update(float dt) override
    {
        Sprite::Update(dt);

        if (currentAnimation)
            currentAnimation->Update(dt);
    }
};
