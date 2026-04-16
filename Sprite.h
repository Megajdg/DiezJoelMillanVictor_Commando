#pragma once
#include "Actor.h"
#include "Vector2.h"
#include <string>
#include "SDL3/SDL.h"

class Sprite : public Actor
{
protected:
    std::string image_name;
    Vector2 size;

public:
    Sprite(Scene* myscene, std::string img_name, Vector2 size);
    Sprite(Scene* myscene, std::string img_name, Transform transform, Vector2 size);

    virtual void Update(float deltaTime) override;
    virtual void Render() override;
};
