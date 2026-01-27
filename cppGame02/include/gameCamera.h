#ifndef GAMECAMERA_H
#define GAMECAMERA_H

#include <raylib.h>
class GameCamera {
private:
    Camera2D camera;
    float smoothSpeed;

public:
    GameCamera();

    void update(Vector2 targetPosition, float deltaTime);
    void handleResize(int targetWidth);
    Camera2D getRaylibCam() const;
};

#endif