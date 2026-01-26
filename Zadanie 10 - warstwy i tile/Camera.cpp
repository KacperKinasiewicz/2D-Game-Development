#include "Camera.h"

Camera::Camera(float width, float height) {
    view.setSize({ width, height });
    view.setCenter({ width / 2.f, height / 2.f });
}

void Camera::update(sf::Vector2f targetPos) {
    sf::Vector2f currentCenter = view.getCenter();
    view.setCenter({ targetPos.x, currentCenter.y });
}