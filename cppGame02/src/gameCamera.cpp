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
}

void GameCamera::handleResize(int targetWidth) {
    float screenWidth = (float)GetScreenWidth();
    float screenHeight = (float)GetScreenHeight();

    camera.offset.x = floorf(screenWidth / 2.0f - targetWidth * camera.zoom / 2.0f);
    
    camera.offset.y = floorf(screenHeight / 2.0f - targetWidth * camera.zoom / 2.0f + 96);
}

Camera2D GameCamera::getRaylibCam() const {
    return camera;
}