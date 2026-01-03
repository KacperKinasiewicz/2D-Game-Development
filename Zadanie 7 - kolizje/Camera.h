#pragma once
#include <SFML/Graphics.hpp>

struct Camera {
    float padding = 250.0f;
    float minZoom = 0.75f;
    float maxZoom = 1.75f;

    float targetLerpSpeed = 0.1f;
    float currentLerpSpeed;

    sf::View gameView;
    sf::Vector2f initialResolution;
    sf::Vector2f currentCenter;
    float currentZoom;

    Camera(sf::Vector2f windowSize);
    void keepPlayerInView(sf::Sprite& player);
    void updateCameraPosition(sf::Sprite& dog, sf::Sprite& cat);
};