#include "ParallaxLayer.h"
#include <iostream>
#include <random>
#include <cmath>
#include <algorithm>

ParallaxLayer::ParallaxLayer(const std::vector<std::string>& filenames, float factor,
                              float minY, float maxY, int count, float loopW,
                              float baseTargetSize, bool alignBottom)
    : parallaxFactor(factor), loopWidth(loopW)
{
    for (const auto& path : filenames) {
        sf::Texture tex;
        if (!tex.loadFromFile(path)) continue;
        textures.push_back(tex);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(0.f, loopWidth);
    std::uniform_real_distribution<float> distY(minY, maxY);
    std::uniform_real_distribution<float> distSizeMod(0.85f, 1.15f);

    for (int i = 0; i < count; ++i) {
        if (textures.empty()) break;
        int idx = std::uniform_int_distribution<int>(0, (int)textures.size() - 1)(gen);
        sf::Sprite sprite(textures[idx]);

        sf::Vector2u texSize = textures[idx].getSize();
        float currentTarget = baseTargetSize * distSizeMod(gen);
        float scale = currentTarget / static_cast<float>(std::max(texSize.x, texSize.y));
        sprite.setScale({scale, scale});

        if (alignBottom)
            sprite.setOrigin({ (float)texSize.x / 2.f, (float)texSize.y });
        else
            sprite.setOrigin({ (float)texSize.x / 2.f, (float)texSize.y / 2.f });

        sprite.setPosition({ distX(gen), distY(gen) });
        tiles.push_back(sprite);
    }
}

void ParallaxLayer::draw(sf::RenderWindow& window, const sf::View& cameraView) {
    float cameraX = cameraView.getCenter().x;
    float viewWidth = cameraView.getSize().x;
    float viewLeft = cameraX - viewWidth / 2.f;

    if (firstFrame) {
        lastCameraX = cameraX;
        firstFrame = false;
    }

    float deltaX = cameraX - lastCameraX;
    lastCameraX = cameraX;

    cumulativeOffset -= deltaX * (1.0f - parallaxFactor);

    for (auto& sprite : tiles) {
        sf::Vector2f basePos = sprite.getPosition();
        float x = std::fmod(basePos.x + cumulativeOffset, loopWidth);
        if (x < 0) x += loopWidth;

        sf::Vector2f originalPos = sprite.getPosition();
        sprite.setPosition({ viewLeft + x - 400.f, basePos.y });
        window.draw(sprite);
        sprite.setPosition(originalPos);
    }
}