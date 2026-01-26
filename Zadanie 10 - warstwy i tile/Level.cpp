#include "Level.h"
#include <iostream>
#include <cmath>

Level::Level() {
    if (!wallTexture.loadFromFile("textures/Welna.png")) {
        std::cerr << "Blad wczytywania Welna.png" << std::endl;
        exit(1);
    }

    floorShape.setSize({ tileSize, tileSize });
    floorShape.setTexture(&wallTexture);
}

void Level::resolveCollision(Player& player) {
    sf::Sprite& sprite = player.getSprite();
    sf::FloatRect bounds = sprite.getGlobalBounds();

    float playerBottom = bounds.position.y + bounds.size.y;

    if (playerBottom > floorY) {

        float halfHeight = bounds.size.y / 2.0f;

        sprite.setPosition({ sprite.getPosition().x, floorY - halfHeight });

        player.velocity.y = 0;
        player.isGrounded = true;
    }
}

void Level::draw(sf::RenderWindow& window, const sf::View& view) {
    sf::Vector2f center = view.getCenter();
    sf::Vector2f size = view.getSize();
    float viewLeft = center.x - size.x / 2.f;
    float viewRight = center.x + size.x / 2.f;

    int startTileIndex = (int)std::floor(viewLeft / tileSize);
    int endTileIndex = (int)std::ceil(viewRight / tileSize);

    for (int i = startTileIndex; i <= endTileIndex; ++i) {
        float x = (float)i * tileSize;

        floorShape.setPosition({ x, floorY });
        window.draw(floorShape);
    }
}