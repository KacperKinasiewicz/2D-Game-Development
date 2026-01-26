#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Player.h"

struct Tile {
    sf::RectangleShape shape;
    bool isWall;
};

class Level {
public:
    const float tileSize = 50.0f;
    sf::Sprite treatSprite;

    Level();
    bool loadLevelFromFile(const std::string& filename);
    void draw(sf::RenderWindow& window);
    void resolveWallCollisions(Player& player, bool checkX);

    float getMapHeight() const;
    float getMapWidth() const;

private:
    sf::Texture wallTexture;
    sf::Texture treatTexture;
    std::vector<Tile> tiles;

    float mapWidth = 0.f;
    float mapHeight = 0.f;
};