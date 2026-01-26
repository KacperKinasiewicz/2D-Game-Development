#pragma once
#include <SFML/Graphics.hpp>

class Camera {
public:
    sf::View view;
    Camera(float width, float height);
    void update(sf::Vector2f targetPos);
};