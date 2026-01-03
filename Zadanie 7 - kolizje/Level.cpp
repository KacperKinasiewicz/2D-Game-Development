#include "Level.h"
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>

Level::Level() : treatSprite(treatTexture) {
    std::srand((unsigned int)std::time(nullptr));
}

bool Level::loadLevelFromFile(const std::string& filename) {
    wallRects.clear();
    validSpawnPoints.clear();
    mapWidth = 0;
    mapHeight = 0;

    if (!wallTexture.loadFromFile("textures/wool.jpg")) return false;
    if (!treatTexture.loadFromFile("textures/treat.png")) return false;

    treatSprite.setTexture(treatTexture, true);

    sf::Vector2u texSize = treatTexture.getSize();
    treatSprite.setOrigin({ (float)texSize.x / 2.f, (float)texSize.y / 2.f });

    float scaleFactor = (tileSize / 2.f) / texSize.x;
    treatSprite.setScale({ scaleFactor, scaleFactor });

    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string line;
    int currentRow = 0;
    int maxColumns = 0;

    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if ((int)line.size() > maxColumns) maxColumns = (int)line.size();

        for (int col = 0; col < (int)line.size(); ++col) {
            char tileChar = line[col];
            float centerX = (float)col * tileSize + tileSize / 2.f;
            float centerY = (float)currentRow * tileSize + tileSize / 2.f;

            if (tileChar == 'O') {
                sf::RectangleShape wall({ tileSize, tileSize });
                wall.setPosition({ (float)col * tileSize, (float)currentRow * tileSize });
                wall.setTexture(&wallTexture);
                wallRects.push_back(wall);
            }
            else {
                validSpawnPoints.push_back({ centerX, centerY });
            }
        }
        currentRow++;
    }

    mapWidth = (float)maxColumns * tileSize;
    mapHeight = (float)currentRow * tileSize;
    return true;
}

void Level::draw(sf::RenderWindow& window) {
    for (const auto& wall : wallRects) window.draw(wall);
    window.draw(treatSprite);
}

void Level::constrainPlayerToMap(sf::Sprite& player) {
    sf::Vector2f pos = player.getPosition();
    sf::FloatRect b = player.getLocalBounds();
    sf::Vector2f s = player.getScale();

    float hw = b.size.x * s.x / 2.f;
    float hh = b.size.y * s.y / 2.f;

    if (pos.x - hw < tileSize) pos.x = hw + tileSize;
    if (pos.x + hw > mapWidth - tileSize) pos.x = mapWidth - hw - tileSize;
    if (pos.y - hh < tileSize) pos.y = hh + tileSize;
    if (pos.y + hh > mapHeight - tileSize) pos.y = mapHeight - hh - tileSize;

    player.setPosition(pos);
}

sf::Vector2f Level::findRandomEmptyPosition() {
    if (validSpawnPoints.empty()) return { 100.f, 100.f };
    int index = std::rand() % validSpawnPoints.size();
    return validSpawnPoints[index];
}

void Level::respawnTreat() {
    treatSprite.setPosition(findRandomEmptyPosition());
}

sf::Vector2f Level::getTreatPosition() const {
    return treatSprite.getPosition();
}

void Level::resolveWallCollisions(Player& player, bool checkX) {
    sf::Sprite& sprite = player.getSprite();
    sf::FloatRect pBounds = sprite.getGlobalBounds();
    sf::Vector2f pCenter = sprite.getPosition();

    for (const auto& wall : wallRects) {
        sf::FloatRect wBounds = wall.getGlobalBounds();

        if (player.getCollisionShape() == CollisionShape::Circle) {
            float radius = pBounds.size.x / 2.f;

            float closestX = std::clamp(pCenter.x, wBounds.position.x, wBounds.position.x + wBounds.size.x);
            float closestY = std::clamp(pCenter.y, wBounds.position.y, wBounds.position.y + wBounds.size.y);

            sf::Vector2f delta = { pCenter.x - closestX, pCenter.y - closestY };
            float distSq = delta.x * delta.x + delta.y * delta.y;

            if (distSq > 0 && distSq < radius * radius) {
                float distance = std::sqrt(distSq);
                sf::Vector2f normal = delta / distance;
                float overlap = radius - distance;

                sprite.move(normal * overlap);
                pCenter = sprite.getPosition();
            }
        }
        else {
            auto intersection = pBounds.findIntersection(wBounds);
            if (intersection) {
                if (checkX) {
                    float overlapX = intersection->size.x;
                    if (pCenter.x < wBounds.position.x) sprite.move({ -overlapX, 0.f });
                    else sprite.move({ overlapX, 0.f });
                }
                else {
                    float overlapY = intersection->size.y;
                    if (pCenter.y < wBounds.position.y) sprite.move({ 0.f, -overlapY });
                    else sprite.move({ 0.f, overlapY });
                }

                pBounds = sprite.getGlobalBounds();
                pCenter = sprite.getPosition();
            }
        }
    }
}