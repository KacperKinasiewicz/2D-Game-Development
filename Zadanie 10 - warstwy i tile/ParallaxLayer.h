#ifndef PARALLAXLAYER_H
#define PARALLAXLAYER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class ParallaxLayer {
private:
    std::vector<sf::Texture> textures;
    std::vector<sf::Sprite> tiles;
    float parallaxFactor;
    float loopWidth;

    float cumulativeOffset = 0.f;
    float lastCameraX = 0.f;
    bool firstFrame = true;

public:
    ParallaxLayer(const std::vector<std::string>& filenames, float factor,
                  float minY, float maxY, int count, float loopWidth,
                  float baseTargetSize, bool alignBottom);

    void setFactor(float f) { parallaxFactor = f; }
    float getFactor() const { return parallaxFactor; }
    void draw(sf::RenderWindow& window, const sf::View& cameraView);
};

#endif