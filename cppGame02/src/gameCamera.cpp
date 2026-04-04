#include "gameCamera.h"
#include <cmath>

GameCamera::GameCamera() {
    camera = { 0 };
    camera.rotation = 0.0f;
    camera.zoom = 3.0f;
    smoothSpeed = 10.0f;
    
    handleResize(0); 
}

void GameCamera::update(Vector2 targetPosition, float deltaTime) {
    camera.target.x += (targetPosition.x - camera.target.x) * smoothSpeed * deltaTime;
    camera.target.y += (targetPosition.y - camera.target.y) * smoothSpeed * deltaTime;

    if (shakeIntensity > 0) {
        camera.offset.x = baseOffset.x + GetRandomValue(-shakeIntensity, shakeIntensity);
        camera.offset.y = baseOffset.y + GetRandomValue(-shakeIntensity, shakeIntensity);

        shakeIntensity -= 15.0f * deltaTime; 
    } else {
        shakeIntensity = 0;
        camera.offset = baseOffset;
    }
}

void GameCamera::handleResize(int targetWidth) {
    float screenWidth = (float)GetScreenWidth();
    float screenHeight = (float)GetScreenHeight();

    baseOffset.x = floorf(screenWidth / 2.0f - targetWidth * camera.zoom / 2.0f);
    baseOffset.y = floorf(screenHeight / 2.0f - targetWidth * camera.zoom / 2.0f + 96);
    
    camera.offset = baseOffset;
}

Camera2D GameCamera::getRaylibCam() const {
    return camera;
}

void GameCamera::screenShake(float intensity) {
    shakeIntensity = intensity;
}