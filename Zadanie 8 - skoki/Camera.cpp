#include "Camera.h"

Camera::Camera(sf::Vector2f windowSize, float mapWidth) {
    float viewportWidth = (windowSize.x - 50.f) / 2.f;
    float ratio = mapWidth / viewportWidth;
    float viewHeight = windowSize.y * ratio;

    gameView.setSize({ mapWidth, viewHeight });
}

void Camera::updateCameraPosition(sf::Sprite& target, float mapWidth, float mapHeight) {
    sf::Vector2f targetPos = target.getPosition();
    sf::Vector2f viewSize = gameView.getSize();

    float targetCenterX = mapWidth / 2.f;
    float targetCenterY = targetPos.y;

    currentCenter.x += (targetCenterX - currentCenter.x) * targetLerpSpeed;
    currentCenter.y += (targetCenterY - currentCenter.y) * targetLerpSpeed;

    float halfHeight = viewSize.y / 2.f;
    if (currentCenter.y < halfHeight) currentCenter.y = halfHeight;
    if (currentCenter.y > mapHeight - halfHeight) currentCenter.y = mapHeight - halfHeight;

    gameView.setCenter(currentCenter);
}