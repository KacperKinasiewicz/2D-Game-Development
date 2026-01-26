#include "Level.h"
#include <fstream>
#include <algorithm>
#include <cmath>

Level::Level() : treatSprite(treatTexture) {
    if (!wallTexture.loadFromFile("textures/wool.jpg")) exit(1);
    if (!treatTexture.loadFromFile("textures/treat.png")) exit(1);
    treatSprite.setTexture(treatTexture, true);
}

bool Level::loadLevelFromFile(const std::string& filename) {
    tiles.clear();
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        lines.push_back(line);
    }

    int totalRows = (int)lines.size();
    int maxCols = 0;

    for (int row = 0; row < totalRows; ++row) {
        maxCols = std::max(maxCols, (int)lines[row].size());
        for (int col = 0; col < (int)lines[row].size(); ++col) {
            float posX = (float)col * tileSize;
            float posY = (float)row * tileSize;

            if (lines[row][col] == 'O') {
                Tile t;
                t.shape.setSize({ tileSize, tileSize });
                t.shape.setPosition({ posX, posY });
                t.shape.setTexture(&wallTexture);
                t.isWall = (col == 0 || col == (int)lines[row].size() - 1 || row == 0 || row == totalRows - 1);
                tiles.push_back(t);
            }
            else if (lines[row][col] == '*') {
                sf::Vector2u texSize = treatTexture.getSize();
                treatSprite.setOrigin({ (float)texSize.x / 2.f, (float)texSize.y / 2.f });
                treatSprite.setPosition({ posX + tileSize / 2.f, posY + tileSize / 2.f });
                float scale = (tileSize * 0.8f) / texSize.x;
                treatSprite.setScale({ scale, scale });
            }
        }
    }
    mapWidth = (float)maxCols * tileSize;
    mapHeight = (float)totalRows * tileSize;
    return true;
}

void Level::resolveWallCollisions(Player& player, bool checkX) {
    sf::Sprite& sprite = player.getSprite();
    sf::FloatRect pBounds = sprite.getGlobalBounds();
    sf::Vector2f pCenter = sprite.getPosition();

    for (const auto& tile : tiles) {
        sf::FloatRect tBounds = tile.shape.getGlobalBounds();

        if (player.getCollisionShape() == CollisionShape::Circle) {
            float radius = pBounds.size.x / 2.f;
            float closestX = std::clamp(pCenter.x, tBounds.position.x, tBounds.position.x + tBounds.size.x);
            float closestY = std::clamp(pCenter.y, tBounds.position.y, tBounds.position.y + tBounds.size.y);
            sf::Vector2f delta = { pCenter.x - closestX, pCenter.y - closestY };
            float distSq = delta.x * delta.x + delta.y * delta.y;

            if (distSq < radius * radius) {
                if (tile.isWall) {
                    if (distSq > 0) {
                        float distance = std::sqrt(distSq);
                        sf::Vector2f normal = delta / distance;
                        float overlap = radius - distance;
                        if (checkX) sprite.move({ normal.x * overlap, 0.f });
                        else {
                            sprite.move({ 0.f, normal.y * overlap });
                            if (normal.y < 0) { player.velocity.y = 0; player.isGrounded = true; }
                            else if (normal.y > 0) player.velocity.y = 0;
                        }
                    }
                }
                else if (!checkX && player.velocity.y > 0 && !player.isDropping) {
                    if (pCenter.y < tBounds.position.y) {
                        float overlapY = radius - std::sqrt(distSq);
                        sprite.move({ 0.f, -overlapY });
                        player.velocity.y = 0;
                        player.isGrounded = true;
                    }
                }
                pBounds = sprite.getGlobalBounds();
                pCenter = sprite.getPosition();
            }
        }
        else {
            auto intersection = pBounds.findIntersection(tBounds);
            if (intersection) {
                if (checkX) {
                    if (tile.isWall) {
                        float overlapX = intersection->size.x;
                        if (pCenter.x < tBounds.position.x + tBounds.size.x / 2.f) sprite.move({ -overlapX, 0.f });
                        else sprite.move({ overlapX, 0.f });
                    }
                }
                else {
                    if (tile.isWall) {
                        float overlapY = intersection->size.y;
                        if (pCenter.y < tBounds.position.y + tBounds.size.y / 2.f) {
                            sprite.move({ 0.f, -overlapY });
                            player.velocity.y = 0;
                            player.isGrounded = true;
                        } else {
                            sprite.move({ 0.f, overlapY });
                            player.velocity.y = 0;
                        }
                    }
                    else if (player.velocity.y > 0 && !player.isDropping) {
                        if (pCenter.y < tBounds.position.y) {
                            float overlapY = intersection->size.y;
                            sprite.move({ 0.f, -overlapY });
                            player.velocity.y = 0;
                            player.isGrounded = true;
                        }
                    }
                }
                pBounds = sprite.getGlobalBounds();
                pCenter = sprite.getPosition();
            }
        }
    }
}

void Level::draw(sf::RenderWindow& window) {
    for (const auto& tile : tiles) window.draw(tile.shape);
    window.draw(treatSprite);
}

float Level::getMapHeight() const { return mapHeight; }
float Level::getMapWidth() const { return mapWidth; }