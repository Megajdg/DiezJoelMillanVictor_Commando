#include "Animation.h"

void Animation::Update(float dt)
{
    // Si la animacion ha terminado, abortamos
    if (finished || frames.empty())
        return;

    // Hacemos correr el reloj interno
    timer += dt;

    // Si ha pasado el tiempo de frame, avanzamos un frame
    while (timer >= frameTime)
    {
        timer -= frameTime;
        currentFrame++;

        // Si llegamos al ultimo frame de la animacion y debe hacer loop, la reiniciamos
        if (currentFrame >= (int)frames.size())
        {
            if (loop)
            {
                currentFrame = 0;
            }
            else
            {
                // Si no, nos quedamos en el ultimo frame y la marcamos como acabada
                currentFrame = (int)frames.size() - 1;
                finished = true;
                break;
            }
        }
    }
}

// Devuelve el frame actual
Frame Animation::GetCurrentFrame() const
{
    return frames[currentFrame];
}

// Devuelve la animacion al primer frame, reseteando el timer y marcandola como no acabada
void Animation::Reset()
{
    currentFrame = 0;
    timer = 0.0f;
    finished = false;
}