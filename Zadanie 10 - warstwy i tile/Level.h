#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

class Level {
public:
    const float floorY = 800.0f;
    const float tileSize = 100.0f;

    Level();

    void draw(sf::RenderWindow& window, const sf::View& view);

    void resolveCollision(Player& player);

private:
    sf::Texture wallTexture;
    sf::RectangleShape floorShape;
};