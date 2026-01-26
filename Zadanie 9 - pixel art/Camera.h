#pragma once
#include <SFML/Graphics.hpp>

struct Camera {
    float targetLerpSpeed = 0.1f;
    sf::View gameView;
    sf::Vector2f currentCenter;

    Camera(sf::Vector2f windowSize, float mapWidth);
    void updateCameraPosition(sf::Sprite& target, float mapWidth, float mapHeight);
};