#include "Camera.h"
#include "Parameters.h"

void Camera::Update()
{
    if (!target)
        return;

    // X fija siempre en 0
    position.x = 0.0f;

    // La cámara solo sigue en Y hacia arriba
    float targetY = target->transform.position.y;

    // La cámara solo sube, nunca baja
    if (targetY < position.y) {
        position.y = position.y * 0.9f + targetY * 0.1f;
    }

    float mapHeight = 6232.0f;
    float topLimit = -mapHeight + (Parameters::screenHeight * 0.5f);

    if (position.y < topLimit)
        position.y = topLimit;
}
