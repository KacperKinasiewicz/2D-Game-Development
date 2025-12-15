#include <SFML/Graphics.hpp>
#include <iostream>

sf::Texture circleTexture;
sf::Texture rectangleTexture;

int main()
{
    if (!circleTexture.loadFromFile("circle.jpg"))
        return 1;
    if (!rectangleTexture.loadFromFile("rectangle.jpg"))
        return 1;

    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Zadanie 2. Gry 2D");

    sf::RectangleShape rectangle(sf::Vector2f(200.f, 200.f));
    rectangle.setTexture(&rectangleTexture);
    rectangle.setPosition(sf::Vector2f(400.f, 300.f));

    sf::CircleShape circle(100.f);
    circle.setTexture(&circleTexture);
    circle.setFillColor({ 255, 255, 255, 150 });
    circle.setPosition(sf::Vector2f(50.f, 80.f));

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
            {
                std::cout << "Player2 Moved! Current position: " << circle.getPosition().x << " " << circle.getPosition().y << "\n";
            }
        }
        
        sf::Vector2f velocity(0.f, 0.f);
        float speed = 20.f;
        float smooth = 0.9f;
        sf::Vector2f targetVelocity(sf::Mouse::getPosition(window));
        sf::Vector2f circlePos = circle.getPosition() + sf::Vector2f(circle.getRadius(), circle.getRadius());
        sf::Vector2f toMouse = targetVelocity - circlePos;
        velocity = toMouse * (1.f - smooth) + velocity * smooth;
        circle.move(velocity);
        
        float step = 10.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) { rectangle.move({ 0.f, -step }); std::cout << "Player1 Moved! Current position: " << rectangle.getPosition().x << " " << rectangle.getPosition().y << "\n"; }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) { rectangle.move({ 0.f, step }); std::cout << "Player1 Moved! Current position: " << rectangle.getPosition().x << " " << rectangle.getPosition().y << "\n";}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) { rectangle.move({ -step, 0.f }); std::cout << "Player1 Moved! Current position: " << rectangle.getPosition().x << " " << rectangle.getPosition().y << "\n";}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) { rectangle.move({ step, 0.f }); std::cout << "Player1 Moved! Current position: " << rectangle.getPosition().x << " " << rectangle.getPosition().y << "\n";}

        window.clear(sf::Color::Cyan);
        window.draw(rectangle);
        window.draw(circle);
        window.display();
    }
}