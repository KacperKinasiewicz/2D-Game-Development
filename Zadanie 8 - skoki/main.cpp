#include <SFML/Graphics.hpp>

sf::Texture circleTexture;
sf::Texture rectangleTexture;

int main()
{
    if (!circleTexture.loadFromFile("circle.jpg"))
        return 1;
    if (!rectangleTexture.loadFromFile("rectangle.jpg"))
        return 1;

    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Zadanie 1. Gry 2D");
    sf::CircleShape circle(100.f);
	circle.setTexture(&circleTexture);
    circle.setPosition(sf::Vector2f(50.f, 80.f));
    sf::Vector2f circleVelocity(0.f, -0.1f);

    sf::RectangleShape rectangle(sf::Vector2f(300.f, 200.f));
	rectangle.setTexture(&rectangleTexture);
    rectangle.setPosition(sf::Vector2f(400.f, 300.f));
    sf::Vector2f rectVelocity(0.1f, 0.f);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        circle.move(circleVelocity);
        if (circle.getPosition().y < -circle.getRadius() * 2)
            circle.setPosition({ circle.getPosition().x, static_cast<float>(window.getSize().y) });
        
        rectangle.move(rectVelocity);
        if (rectangle.getPosition().x > window.getSize().x)
            rectangle.setPosition({ -rectangle.getSize().x, rectangle.getPosition().y });

        window.clear(sf::Color::Cyan);
        window.draw(circle);
        window.draw(rectangle);
        window.display();
    }
}