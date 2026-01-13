#include "Player.h"
#include <algorithm>

Player::Player(const std::string& textureFilename, sf::Vector2f initialPosition,
               sf::Keyboard::Key keyUp, sf::Keyboard::Key keyDown, sf::Keyboard::Key keyLeft, sf::Keyboard::Key keyRight, CollisionShape shape)
    : sprite(texture), kUp(keyUp), kDown(keyDown), kLeft(keyLeft), kRight(keyRight), collisionShape(shape)
{
    if (!texture.loadFromFile(textureFilename)) exit(1);
    texture.setSmooth(true);
    sprite.setTexture(texture, true);
    sf::Vector2u textureSize = texture.getSize();
    float scaleFactor = targetPixelSize / static_cast<float>(std::max(textureSize.x, textureSize.y));
    sprite.setOrigin({ (float)textureSize.x / 2.f, (float)textureSize.y / 2.f });
    sprite.setScale({ scaleFactor, scaleFactor });
    sprite.setPosition(initialPosition);
    calculatePhysicsConstants();

    sf::Color navy(0, 0, 128);
    if (collisionShape == CollisionShape::Rectangle) {
        debugRect.setSize({ targetPixelSize, targetPixelSize });
        debugRect.setOrigin({ targetPixelSize / 2.f, targetPixelSize / 2.f });
        debugRect.setFillColor(sf::Color::Transparent);
        debugRect.setOutlineColor(navy);
        debugRect.setOutlineThickness(2.f);
    } else {
        debugCircle.setRadius(targetPixelSize / 2.f);
        debugCircle.setOrigin({ targetPixelSize / 2.f, targetPixelSize / 2.f });
        debugCircle.setFillColor(sf::Color::Transparent);
        debugCircle.setOutlineColor(navy);
        debugCircle.setOutlineThickness(2.f);
    }
}

void Player::calculatePhysicsConstants() {
    float tPeak = dHalf / moveSpeed;
    gravity = (2.0f * hMax) / (tPeak * tPeak);
    jumpVelocity = gravity * tPeak;
}

void Player::handleInput() {
    velocity.x = 0.f;
    if (sf::Keyboard::isKeyPressed(kLeft)) velocity.x = -moveSpeed;
    if (sf::Keyboard::isKeyPressed(kRight)) velocity.x = moveSpeed;

    bool jumpKeyPressed = sf::Keyboard::isKeyPressed(kUp);
    if (jumpKeyPressed && !jumpKeyWasPressed) {
        if (isGrounded || coyoteTimer > 0.f) {
            velocity.y = -jumpVelocity;
            isGrounded = false;
            coyoteTimer = 0.f;
            jumpCount = 1;
        } else if (jumpCount < 2) {
            velocity.y = -jumpVelocity;
            jumpCount = 2;
        }
    }
    jumpKeyWasPressed = jumpKeyPressed;
    isDropping = sf::Keyboard::isKeyPressed(kDown);
}

void Player::applyMovementX() {
    sprite.move({ velocity.x * (1.f / 60.f), 0.f });
}

void Player::applyMovementY(float dt) {
    if (isGrounded) {
        coyoteTimer = ghostJumpMaxTime;
        jumpCount = 0;
    } else {
        coyoteTimer -= dt;
    }

    float currentGravity = gravity;
    if (velocity.y < 0.f) {
        if (!sf::Keyboard::isKeyPressed(kUp)) currentGravity = gravity * (fastFallMultiplier + 2.f);
    } else {
        currentGravity = gravity * fastFallMultiplier;
    }

    float dy = velocity.y * dt + 0.5f * currentGravity * dt * dt;
    sprite.move({ 0.f, dy });
    velocity.y += currentGravity * dt;
    if (velocity.y > 1500.f) velocity.y = 1500.f;
    isGrounded = false;
}

void Player::draw(sf::RenderWindow& window, bool drawHitbox) {
    window.draw(sprite);
    if (drawHitbox) {
        if (collisionShape == CollisionShape::Rectangle) {
            debugRect.setPosition(sprite.getPosition());
            window.draw(debugRect);
        } else {
            debugCircle.setPosition(sprite.getPosition());
            window.draw(debugCircle);
        }
    }
}

sf::Sprite& Player::getSprite() {
    return sprite;
}

CollisionShape Player::getCollisionShape() const {
    return collisionShape;
}