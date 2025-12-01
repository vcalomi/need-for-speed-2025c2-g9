#include "camera.h"

Camera::Camera(int viewportW, int viewportH): viewportWidth(viewportW), viewportHeight(viewportH) {}

float Camera::getX() const { return x; }

float Camera::getY() const { return y; }

void Camera::SetViewport(int w, int h) {
    viewportWidth = w;
    viewportHeight = h;
}


void Camera::SetPosition(float newX, float newY) {
    x = newX;
    y = newY;
}

void Camera::Clamp(int mapW, int mapH) {
    x = std::max(0.0f, std::min(x, static_cast<float>(mapW - viewportWidth)));
    y = std::max(0.0f, std::min(y, static_cast<float>(mapH - viewportHeight)));
}

void Camera::Follow(float targetX, float targetY, int mapW, int mapH) {
    // Centrar en el objetivo
    x = targetX - viewportWidth / 2.0f;
    y = targetY - viewportHeight / 2.0f;

    // Limitar dentro del mapa
    Clamp(mapW, mapH);
}

int Camera::GetViewportWidth() const { return viewportWidth; }

int Camera::GetViewportHeight() const { return viewportHeight; }
