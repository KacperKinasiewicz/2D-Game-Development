#include "Camera.h"
#include <algorithm>

Camera::Camera(sf::Vector2f windowSize) {
    initialResolution = windowSize;
    gameView.setSize(initialResolution);
    gameView.setCenter({ initialResolution.x / 2.f, initialResolution.y / 2.f });
    currentCenter = gameView.getCenter();

    currentZoom = 10.f;
    currentLerpSpeed = 0.001f;
}

void Camera::keepPlayerInView(sf::Sprite& player) {
    sf::Vector2f playerPos = player.getPosition();
    sf::Vector2f scale = player.getScale();
    sf::FloatRect bounds = player.getLocalBounds();

    float halfWidth = bounds.size.x * scale.x / 2.f;
    float halfHeight = bounds.size.y * scale.y / 2.f;

    sf::Vector2f center = gameView.getCenter();
    sf::Vector2f size = gameView.getSize();
    float limitLeft = center.x - size.x / 2.f;
    float limitRight = center.x + size.x / 2.f;
    float limitTop = center.y - size.y / 2.f;
    float limitBottom = center.y + size.y / 2.f;

    if (playerPos.x - halfWidth < limitLeft) playerPos.x = limitLeft + halfWidth;
    if (playerPos.x + halfWidth > limitRight) playerPos.x = limitRight - halfWidth;
    if (playerPos.y - halfHeight < limitTop) playerPos.y = limitTop + halfHeight;
    if (playerPos.y + halfHeight > limitBottom) playerPos.y = limitBottom - halfHeight;

    player.setPosition(playerPos);
}

void Camera::updateCameraPosition(sf::Sprite& dog, sf::Sprite& cat) {
    sf::Vector2f dogPos = dog.getPosition();
    sf::Vector2f catPos = cat.getPosition();

    sf::Vector2f targetCenter = { (dogPos.x + catPos.x) / 2.f, (dogPos.y + catPos.y) / 2.f };

    float minX = std::min(dogPos.x, catPos.x);
    float maxX = std::max(dogPos.x, catPos.x);
    float minY = std::min(dogPos.y, catPos.y);
    float maxY = std::max(dogPos.y, catPos.y);

    float requiredWidth = (maxX - minX) + padding;
    float requiredHeight = (maxY - minY) + padding;

    float zoomX = requiredWidth / initialResolution.x;
    float zoomY = requiredHeight / initialResolution.y;
    float targetZoom = std::max(zoomX, zoomY);

    targetZoom = std::clamp(targetZoom, minZoom, maxZoom);

    if (currentLerpSpeed < targetLerpSpeed) {
        currentLerpSpeed += 0.001f;
    }

    currentCenter.x += (targetCenter.x - currentCenter.x) * currentLerpSpeed;
    currentCenter.y += (targetCenter.y - currentCenter.y) * currentLerpSpeed;
    currentZoom += (targetZoom - currentZoom) * currentLerpSpeed;

    gameView.setSize({ initialResolution.x * currentZoom, initialResolution.y * currentZoom });
    gameView.setCenter(currentCenter);
}