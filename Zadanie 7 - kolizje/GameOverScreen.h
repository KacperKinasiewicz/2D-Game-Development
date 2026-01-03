#pragma once
#include <SFML/Graphics.hpp>

struct GameOverScreen {
    sf::Font font;
    sf::Texture dogTexture;
    sf::Texture catTexture;

    sf::Sprite dogSprite;
    sf::Sprite catSprite;
    
    sf::Text titleText;
    sf::Text winnerText;
    sf::Text scoreText;
    sf::Text restartText;

    GameOverScreen();
    void draw(sf::RenderWindow& window, int dogScore, int catScore);
};