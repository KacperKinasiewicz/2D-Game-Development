#include "GameOverScreen.h"
#include <string>

GameOverScreen::GameOverScreen()
    : dogSprite(dogTexture),
      catSprite(catTexture),
      titleText(font),
      winnerText(font),
      scoreText(font),
      restartText(font)
{
    if (!font.openFromFile("arial.ttf")) exit(1);
    if (!dogTexture.loadFromFile("textures/dog.png")) exit(1);
    if (!catTexture.loadFromFile("textures/cat.png")) exit(1);

    dogSprite.setTexture(dogTexture, true);
    catSprite.setTexture(catTexture, true);

    dogTexture.setSmooth(true);
    catTexture.setSmooth(true);

    float iconSize = 60.0f;
    float scaleDog = iconSize / dogTexture.getSize().x;
    float scaleCat = iconSize / catTexture.getSize().x;

    dogSprite.setScale({ scaleDog, scaleDog });
    catSprite.setScale({ scaleCat, scaleCat });

    dogSprite.setOrigin({ dogTexture.getSize().x / 2.f, dogTexture.getSize().y / 2.f });
    catSprite.setOrigin({ catTexture.getSize().x / 2.f, catTexture.getSize().y / 2.f });

    titleText.setFont(font);
    titleText.setString("GAME OVER");
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color::Black);
    titleText.setStyle(sf::Text::Bold);

    winnerText.setFont(font);
    winnerText.setCharacterSize(35);
    winnerText.setFillColor(sf::Color::Black);

    scoreText.setFont(font);
    scoreText.setCharacterSize(40);
    scoreText.setFillColor(sf::Color::Black);

    restartText.setFont(font);
    restartText.setString("Press SPACE to restart");
    restartText.setCharacterSize(20);
    restartText.setFillColor(sf::Color(80, 80, 80));
}

void GameOverScreen::draw(sf::RenderWindow& window, int dogScore, int catScore) {
    sf::Vector2u windowSize = window.getSize();
    float centerX = windowSize.x / 2.f;
    float centerY = windowSize.y / 2.f;

    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin({ titleBounds.position.x + titleBounds.size.x / 2.f,
                          titleBounds.position.y + titleBounds.size.y / 2.f });
    titleText.setPosition({ centerX, centerY - 150.f });

    std::string message = "Draw!";
    if (dogScore > catScore) message = "Winner: Dog!";
    else if (catScore > dogScore) message = "Winner: Cat!";

    winnerText.setString(message);
    sf::FloatRect winBounds = winnerText.getLocalBounds();
    winnerText.setOrigin({ winBounds.position.x + winBounds.size.x / 2.f,
                           winBounds.position.y + winBounds.size.y / 2.f });
    winnerText.setPosition({ centerX, centerY - 80.f });

    scoreText.setString(std::to_string(dogScore) + " : " + std::to_string(catScore));
    sf::FloatRect scoreBounds = scoreText.getLocalBounds();
    scoreText.setOrigin({ scoreBounds.position.x + scoreBounds.size.x / 2.f,
                          scoreBounds.position.y + scoreBounds.size.y / 2.f });
    scoreText.setPosition({ centerX, centerY });

    dogSprite.setPosition({ centerX - 100.f, centerY});
    catSprite.setPosition({ centerX + 100.f, centerY });

    sf::FloatRect restBounds = restartText.getLocalBounds();
    restartText.setOrigin({ restBounds.position.x + restBounds.size.x / 2.f,
                            restBounds.position.y + restBounds.size.y / 2.f });
    restartText.setPosition({ centerX, centerY + 120.f });

    window.draw(titleText);
    window.draw(winnerText);
    window.draw(dogSprite);
    window.draw(scoreText);
    window.draw(catSprite);
    window.draw(restartText);
}