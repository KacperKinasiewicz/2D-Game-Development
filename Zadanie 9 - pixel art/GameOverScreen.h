#pragma once
#include <SFML/Graphics.hpp>
#include <string>

struct GameOverScreen {
    sf::Font font;
    sf::Texture dogTexture;
    sf::Texture catTexture;
    sf::Sprite dogSprite;
    sf::Sprite catSprite;
    sf::Text titleText;
    sf::Text winnerText;
    sf::Text restartText;

    GameOverScreen();
    void draw(sf::RenderWindow& window, const std::string& winnerName);
};