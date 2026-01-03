#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Player.h"

struct Level {
    const float tileSize = 50.0f;

    sf::Texture wallTexture;
    sf::Texture treatTexture;

    std::vector<sf::RectangleShape> wallRects;
    sf::Sprite treatSprite;

    std::vector<sf::Vector2f> validSpawnPoints;

    float mapWidth = 0.f;
    float mapHeight = 0.f;

    Level();
    bool loadLevelFromFile(const std::string& filename);
    void draw(sf::RenderWindow& window);
    void constrainPlayerToMap(sf::Sprite& player);
    sf::Vector2f findRandomEmptyPosition();
    void respawnTreat();
    sf::Vector2f getTreatPosition() const;

    void resolveWallCollisions(Player& player, bool checkX);
};