#define _USE_MATH_DEFINES
#include "Player.h"
#include <cmath>
#include <algorithm>

Player::Player(const std::string& textureFilename, CollisionShape shape, sf::Vector2f initialPosition,
               sf::Keyboard::Key keyUp, sf::Keyboard::Key keyDown,
               sf::Keyboard::Key keyLeft, sf::Keyboard::Key keyRight)
    : collisionShape(shape),
      kUp(keyUp), kDown(keyDown), kLeft(keyLeft), kRight(keyRight),
      sprite(texture), arrowSprite(arrowTexture)
{
    if (!texture.loadFromFile(textureFilename)) exit(1);

    sprite.setTexture(texture, true);
    texture.setSmooth(true);

    sf::Vector2u textureSize = texture.getSize();
    float maxDimension = static_cast<float>(std::max(textureSize.x, textureSize.y));
    float scaleFactor = targetPixelSize / maxDimension;

    sprite.setOrigin({ (float)textureSize.x / 2.f, (float)textureSize.y / 2.f });
    sprite.setScale({ scaleFactor, scaleFactor });
    sprite.setPosition(initialPosition);

    if (!arrowTexture.loadFromFile("textures/arrow.png")) exit(1);
    arrowSprite.setTexture(arrowTexture, true);
    arrowTexture.setSmooth(true);
    arrowSprite.setScale({ arrowScale, arrowScale });

    sf::FloatRect arrowBounds = arrowSprite.getLocalBounds();
    arrowSprite.setOrigin({ arrowBounds.size.x / 2.f, arrowBounds.size.y / 2.f });
}

void Player::handleInput() {
    velocity = { 0.f, 0.f };
    if (sf::Keyboard::isKeyPressed(kUp))    velocity.y = -movementSpeed;
    if (sf::Keyboard::isKeyPressed(kDown))  velocity.y = movementSpeed;
    if (sf::Keyboard::isKeyPressed(kLeft))  velocity.x = -movementSpeed;
    if (sf::Keyboard::isKeyPressed(kRight)) velocity.x = movementSpeed;
}

void Player::applyMovementX() {
    sprite.move({ velocity.x, 0.f });
}

void Player::applyMovementY() {
    sprite.move({ 0.f, velocity.y });
}

void Player::updateDirectionArrow(sf::Vector2f targetPosition) {
    sf::Vector2f playerPosition = sprite.getPosition();
    float diffX = targetPosition.x - playerPosition.x;
    float diffY = targetPosition.y - playerPosition.y;
    float distanceSquared = diffX * diffX + diffY * diffY;

    if (distanceSquared > arrowShowThreshold) {
        isArrowVisible = true;

        float angleDegrees = std::atan2(diffY, diffX) * 180.f / M_PI;
        arrowSprite.setRotation(sf::degrees(angleDegrees));

        float distance = std::sqrt(distanceSquared);
        float directionX = diffX / distance;
        float directionY = diffY / distance;

        arrowSprite.setPosition({ playerPosition.x + directionX * arrowDistance,
                                  playerPosition.y + directionY * arrowDistance });
    } else {
        isArrowVisible = false;
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    if (isArrowVisible) window.draw(arrowSprite);
}

sf::Sprite& Player::getSprite() {
    return sprite;
}

CollisionShape Player::getCollisionShape() const {
    return collisionShape;
}