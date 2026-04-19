#include "Camera.h"
#include "Parameters.h"

void Camera::Update()
{
    // Si no hay target, abortamos
    if (!target)
        return;

    // X fija siempre en 0
    position.x = 0.0f;

    // Obtenemos la posicion Y del jugador
    float targetY = target->transform.position.y;

    // La camara solo sube, nunca baja
    if (targetY < position.y) {
        position.y = position.y * 0.9f + targetY * 0.1f;
    }

    // Calculamos el limite superior del mapa
    float mapHeight = 6232.0f;
    float topLimit = -mapHeight + (Parameters::screenHeight * 0.5f);

    // Hacemos que no pueda subir mas alla del limite superior
    if (position.y < topLimit)
        position.y = topLimit;
}
