#include "AnimationSet.h"
#include "Animation.h"
#include "AnimationPowerUp.h"

AnimationSet LoadPowerUpAnimations()
{
    // Creamos un set de animaciones vacio
    AnimationSet set;

    // Tamaño de cada frame dentro del spritesheet
    float frameW = 26;
    float frameH = 21;

    // Margen inicial desde la esquina superior izquierda
    float startX = 2;
    float startY = 2;

    // Separacion entre frames
    float sepX = 4;
    float sepY = 0;

    // Define una funcion interna que crea una animacion, calcula los frames y los mete en el set
    auto AddAnim = [&](const std::string& name, int row, int colStart, int colEnd, float frameTime = 0.08f, bool loop = true)
    {
        // Creamos una animacion y le asignamos nombre, velocidad y si hace loop o no
        Animation* a = new Animation();
        a->name = name;
        a->frameTime = frameTime;
        a->loop = loop;

        // Calculamos la coordenada Y del frame segun la fila
        float y = startY + row * (frameH + sepY);

        // Recorremos todas las columnas y calculamos la X de cada frame, añadiendolo al vector
        for (int i = colStart; i <= colEnd; i++)
        {
            float x = startX + i * (frameW + sepX);
            a->frames.push_back({ x, y, frameW, frameH });
        }

        // Por ultimo, registramos la animacion en el set
        set.anims[name] = a;
    };

    // Creamos una animacion de dos frames (un parpadeo)
    AddAnim("grenadePU", 0, 0, 1, 0.2f);

    // Y devolvemos el set completo
    return set;
}