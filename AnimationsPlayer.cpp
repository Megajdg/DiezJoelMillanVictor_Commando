#include "Animation.h"
#include "AnimationSet.h"

AnimationSet LoadPlayerAnimations()
{
    // Creamos un set de animaciones vacio
    AnimationSet set;

    // Tamaño de cada frame dentro del spritesheet
    float frameW = 24;
    float frameH = 21;

    // Margen inicial desde la esquina superior izquierda
    float startX = 3;
    float startY = 11;

    // Separacion entre frames
    float sepX = 6;
    float sepY[4] = { 3, 3, 12, 3 };

    // Define una funcion interna que calcula la coordenada Y de una fila concreta
    auto rowY = [&](int row)
    {
        float y = startY;
        for (int i = 0; i < row; i++)
            y += frameH + sepY[i];
        return y;
    };

    // Define una funcion interna que crea una animacion, calcula los frames y los mete en el set
    auto AddAnim = [&](const std::string& name, int row, int colStart, int colEnd, float frameTime = 0.1f, bool loop = true)
    {
        // Creamos una animacion y le asignamos nombre, velocidad y si hace loop o no
        Animation* a = new Animation();
        a->name = name;
        a->frameTime = frameTime;
        a->loop = loop;

        // Calcula la Y de la fila
        float y = rowY(row);
        // Recorremos todas las columnas y calculamos la X de cada frame, añadiendolo al vector
        for (int i = colStart; i <= colEnd; i++)
            a->frames.push_back({ startX + i * (frameW + sepX), y, frameW, frameH });

        // Lo añadimos al set
        set.anims[name] = a;
    };

    // ANIMACIONES DE CORRER
    // Creamos una animacion de cuatro frames del jugador corriendo en diagonal hacia arriba a la izquierda
    AddAnim("run_diag_front_left", 0, 0, 3);
    // Creamos una animacion de cuatro frames del jugador corriendo hacia abajo
    AddAnim("run_front", 0, 4, 7);
    // Creamos una animacion de cuatro frames del jugador corriendo en diagonal hacia arriba a la derecha
    AddAnim("run_diag_front_right", 0, 8, 11);
    // Creamos una animacion de cuatro frames del jugador corriendo hacia la izquierda
    AddAnim("run_left", 1, 0, 3);
    // Creamos una animacion de cuatro frames del jugador corriendo hacia la derecha
    AddAnim("run_right", 1, 8, 11);
    // Creamos una animacion de cuatro frames del jugador corriendo en diagonal hacia abajo a la izquierda
    AddAnim("run_diag_back_left", 2, 0, 3);
    // Creamos una animacion de cuatro frames del jugador corriendo hacia arriba
    AddAnim("run_back", 2, 4, 7);
    // Creamos una animacion de cuatro frames del jugador corriendo en diagonal hacia abajo a la derecha
    AddAnim("run_diag_back_right", 2, 8, 11);

    // Creamos una animacion de tres frames del jugador lanzando una granada
    AddAnim("throw", 3, 0, 2, 0.09f, false);

    // Creamos una animacion de dos frames del jugador muriendo, pero las posiciones X las ponemos manualmente ya que la animacion se encuentra fuera del patron regular de la spritesheet
    {
        Animation* a = new Animation();
        a->name = "death";
        a->frameTime = 0.15f;
        a->loop = false;

        float y = rowY(3);
        a->frames.push_back({ 107, y, frameW, frameH });
        a->frames.push_back({ 137, y, frameW, frameH });

        set.anims["death"] = a;
    }

    // ANIMACIONES IDLE
    // Creamos una animacion estatica de un frame del jugador mirando hacia abajo
    {
        Animation* a = new Animation();
        a->name = "idle_front";
        a->frameTime = 0.2f;
        a->frames.push_back({ 183, rowY(0), frameW, frameH });
        set.anims["idle_front"] = a;
    }
    // Creamos una animacion estatica de un frame del jugador mirando hacia arriba
    {
        Animation* a = new Animation();
        a->name = "idle_back";
        a->frameTime = 0.2f;
        a->frames.push_back({ 183, rowY(2), frameW, frameH });
        set.anims["idle_back"] = a;
    }
    // Creamos una animacion estatica de un frame del jugador mirando hacia la izquierda
    {
        Animation* a = new Animation();
        a->name = "idle_left";
        a->frameTime = 0.2f;
        a->frames.push_back({ startX, rowY(1), frameW, frameH });
        set.anims["idle_left"] = a;
    }
    // Creamos una animacion estatica de un frame del jugador mirando hacia la derecha
    {
        Animation* a = new Animation();
        a->name = "idle_right";
        a->frameTime = 0.2f;
        a->frames.push_back({ startX + 8 * (frameW + sepX), rowY(1), frameW, frameH });
        set.anims["idle_right"] = a;
    }

    // Y devolvemos el set completo
    return set;
}
