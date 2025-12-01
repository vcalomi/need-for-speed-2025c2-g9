#pragma once
#include <algorithm>

class Camera {
public:
    Camera(int viewportW, int viewportH);
    float getX() const;
    float getY() const;
    void Follow(float targetX, float targetY, int mapW, int mapH);
    void SetPosition(float newX, float newY);
    void Clamp(int mapW, int mapH);
    int GetViewportWidth() const;
    int GetViewportHeight() const;
    void SetViewport(int w, int h);

private:
    int viewportWidth;
    int viewportHeight;
    float x = 0.0f;
    float y = 0.0f;
};
