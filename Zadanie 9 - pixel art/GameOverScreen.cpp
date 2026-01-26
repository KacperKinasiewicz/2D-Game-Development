#include "GameOverScreen.h"

GameOverScreen::GameOverScreen()
    : dogSprite(dogTexture), catSprite(catTexture), titleText(font), winnerText(font), restartText(font)
{
    if (!font.openFromFile("arial.ttf")) exit(1);
    if (!dogTexture.loadFromFile("textures/dog.png")) exit(1);
    if (!catTexture.loadFromFile("textures/cat.png")) exit(1);

    dogSprite.setTexture(dogTexture, true);
    catSprite.setTexture(catTexture, true);

    float iconSize = 120.0f;
    dogSprite.setScale({ iconSize / dogTexture.getSize().x, iconSize / dogTexture.getSize().x });
    catSprite.setScale({ iconSize / catTexture.getSize().x, iconSize / catTexture.getSize().x });
    dogSprite.setOrigin({ dogTexture.getSize().x / 2.f, dogTexture.getSize().y / 2.f });
    catSprite.setOrigin({ catTexture.getSize().x / 2.f, catTexture.getSize().y / 2.f });

    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::Black);
    titleText.setString("GAME OVER");

    winnerText.setCharacterSize(45);
    winnerText.setFillColor(sf::Color(243, 135, 187));

    restartText.setCharacterSize(20);
    restartText.setFillColor(sf::Color(80, 80, 80));
    restartText.setString("Press SPACE to restart");
}

void GameOverScreen::draw(sf::RenderWindow& window, const std::string& winnerName) {
    sf::Vector2u windowSize = window.getSize();
    float centerX = windowSize.x / 2.f;
    float centerY = windowSize.y / 2.f;

    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin({ titleBounds.position.x + titleBounds.size.x / 2.f, titleBounds.position.y + titleBounds.size.y / 2.f });
    titleText.setPosition({ centerX, centerY - 180.f });

    winnerText.setString("Winner: " + winnerName);
    sf::FloatRect winBounds = winnerText.getLocalBounds();
    winnerText.setOrigin({ winBounds.position.x + winBounds.size.x / 2.f, winBounds.position.y + winBounds.size.y / 2.f });
    winnerText.setPosition({ centerX, centerY - 80.f });

    if (winnerName == "Dog") dogSprite.setPosition({ centerX, centerY + 30.f }), window.draw(dogSprite);
    else catSprite.setPosition({ centerX, centerY + 30.f }), window.draw(catSprite);

    sf::FloatRect restBounds = restartText.getLocalBounds();
    restartText.setOrigin({ restBounds.position.x + restBounds.size.x / 2.f, restBounds.position.y + restBounds.size.y / 2.f });
    restartText.setPosition({ centerX, centerY + 160.f });

    window.draw(titleText);
    window.draw(winnerText);
    window.draw(restartText);
}