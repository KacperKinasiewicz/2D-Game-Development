#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <optional>
#include <cmath>

int main() {
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "5. 2D Game");
    window.setFramerateLimit(60);

    std::ifstream file("level.txt");
    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku level.txt\n";
        return 1;
    }

    const float tileSize = 50.f;
    sf::Texture wallTex, woodTex, starTex;
    if (!wallTex.loadFromFile("textures/pinkwool.jpg")) std::cerr << "Nie udało się wczytać wall.png\n";
    if (!woodTex.loadFromFile("textures/lavenderwool.jpg")) std::cerr << "Nie udało się wczytać wood.png\n";
    if (!starTex.loadFromFile("textures/treat.png")) std::cerr << "Nie udało się wczytać star.png\n";

    std::vector<sf::RectangleShape> objects;
    std::string line;
    int row = 0;
    int maxCols = 0;
    int maxRows = 0;
    while (std::getline(file, line)) {
        if ((int)line.size() > maxCols) maxCols = line.size();
        for (size_t col = 0; col < line.size(); ++col) {
            sf::Texture* tex = nullptr;
            char c = line[col];
            if (c == '|') tex = &wallTex;
            else if (c == '-') tex = &woodTex;
            else if (c == '*') tex = &starTex;
            if (tex) {
                sf::RectangleShape rect(sf::Vector2f(tileSize, tileSize));
                rect.setPosition({ col * tileSize, row * tileSize });
                rect.setTexture(tex);
                objects.push_back(rect);
            }
        }
        row++;
    }
    maxRows = row;
    float levelWidth = tileSize * maxCols;
    float levelHeight = tileSize * maxRows;

    sf::Texture dogTex, catTex;
    if (!dogTex.loadFromFile("textures/dog.png")) std::cerr << "Nie udało się wczytać dog.png\n";
    if (!catTex.loadFromFile("textures/cat.png")) std::cerr << "Nie udało się wczytać cat.png\n";
    dogTex.setSmooth(true);
    catTex.setSmooth(true);

    sf::Sprite dogPlayer(dogTex);
    dogPlayer.setScale({ 0.1f, 0.1f });
    dogPlayer.setPosition({ 100.f, 100.f });
    sf::Vector2f dogBounds = dogPlayer.getLocalBounds().getCenter();
    dogPlayer.setOrigin(dogBounds);

    sf::Sprite catPlayer(catTex);
    catPlayer.setScale({ 0.1f, 0.1f });
    catPlayer.setPosition({ 200.f, 200.f });
    sf::Vector2f catBounds = catPlayer.getLocalBounds().getCenter();
    catPlayer.setOrigin(catBounds);

    sf::View view = window.getDefaultView();
    sf::Vector2f baseViewSize = view.getSize();
    float currentZoom = 100.f;

    float dogSpeed = 5.f;
    float catSpeed = 5.f;

    float centerLerp = 0.1f;
    float zoomLerp = 0.1f;
    float padding = 120.f;
    float minZoom = 0.5f;
    float maxZoom = 1.5f;

    sf::Vector2f cameraCenter = view.getCenter();

    enum class CameraMode { CenteredCamera, CameraWindow};
    CameraMode cameraMode = CameraMode::CenteredCamera;

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (event->is<sf::Event::KeyPressed>()) {
                if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Tab) {
                    cameraMode = (cameraMode == CameraMode::CenteredCamera) ? CameraMode::CameraWindow : CameraMode::CenteredCamera;
                }
            }
        }

        sf::Vector2f dogMove(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) dogMove.y -= dogSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) dogMove.y += dogSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) dogMove.x -= dogSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) dogMove.x += dogSpeed;
        dogPlayer.move(dogMove);

        sf::Vector2f catMove(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) catMove.y -= catSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) catMove.y += catSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) catMove.x -= catSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) catMove.x += catSpeed;
        catPlayer.move(catMove);

        auto clampPos = [&](sf::Sprite& player) {
            sf::Vector2f playerPosition = player.getPosition();
            sf::Vector2f playerBound = player.getLocalBounds().getCenter();
            sf::Vector2f playerScale = player.getScale();
            float halfWidth = (playerBound.x * playerScale.x);
            float halfHeight = (playerBound.y * playerScale.y);
            if (playerPosition.x - halfWidth < tileSize) playerPosition.x = halfWidth + tileSize;
            if (playerPosition.x + halfWidth > levelWidth - tileSize) playerPosition.x = levelWidth - halfWidth - tileSize;
            if (playerPosition.y - halfHeight < tileSize) playerPosition.y = halfHeight + tileSize;
            if (playerPosition.y + halfHeight > levelHeight - tileSize) playerPosition.y = levelHeight - halfHeight - tileSize;
            player.setPosition(playerPosition);
            };

        auto clampToCamera = [&](sf::Sprite& player) {
            sf::Vector2f playerPosition = player.getPosition();
            sf::Vector2f playerBound = player.getLocalBounds().getCenter();
            sf::Vector2f playerScale = player.getScale();
            float halfWidth = (playerBound.x * playerScale.x);
            float halfHeight = (playerBound.y * playerScale.y);
            sf::Vector2f camSize = view.getSize();
            sf::Vector2f camTopLeft = view.getCenter() - camSize / 2.f;
            sf::Vector2f camBottomRight = view.getCenter() + camSize / 2.f;
            if (playerPosition.x - halfWidth < camTopLeft.x) playerPosition.x = camTopLeft.x + halfWidth;
            if (playerPosition.x + halfWidth > camBottomRight.x) playerPosition.x = camBottomRight.x - halfWidth;
            if (playerPosition.y - halfHeight < camTopLeft.y) playerPosition.y = camTopLeft.y + halfHeight;
            if (playerPosition.y + halfHeight > camBottomRight.y) playerPosition.y = camBottomRight.y - halfHeight;
            player.setPosition(playerPosition);
            };

        auto clampToCameraWindow = [&](sf::Sprite& player) {
            sf::Vector2f playerPosition = player.getPosition();
            sf::Vector2f playerBound = player.getLocalBounds().getCenter();
            sf::Vector2f halfSize(playerBound.x / 2.f, playerBound.y / 2.f);
            sf::Vector2f cameraWindowSize = baseViewSize - sf::Vector2f(padding / 2, padding / 2);
            sf::Vector2f windowLeftTop = cameraCenter - cameraWindowSize * 0.5f;
            sf::Vector2f windowRightBottom = cameraCenter + cameraWindowSize * 0.5f;
            if (playerPosition.x - halfSize.x < windowLeftTop.x) playerPosition.x = windowLeftTop.x + halfSize.x;
            if (playerPosition.x + halfSize.x > windowRightBottom.x) playerPosition.x = windowRightBottom.x - halfSize.x;
            if (playerPosition.y - halfSize.y < windowLeftTop.y) playerPosition.y = windowLeftTop.y + halfSize.y;
            if (playerPosition.y + halfSize.y > windowRightBottom.y) playerPosition.y = windowRightBottom.y - halfSize.y;
            player.setPosition(playerPosition);
            };

        clampPos(dogPlayer);
        clampPos(catPlayer);
        clampToCamera(dogPlayer);
        clampToCamera(catPlayer);

        sf::Vector2f dogCenter = dogPlayer.getGlobalBounds().getCenter();
        sf::Vector2f catCenter = catPlayer.getGlobalBounds().getCenter();

        sf::Vector2f targetCenter;
        float targetZoom = currentZoom;

		//  zoom-to-fit + position-averaging + lerp-smoothing
        if (cameraMode == CameraMode::CenteredCamera) {
            targetCenter = (dogCenter + catCenter) * 0.5f;
            float left = std::min(dogCenter.x, catCenter.x);
            float right = std::max(dogCenter.x, catCenter.x);
            float top = std::min(dogCenter.y, catCenter.y);
            float bottom = std::max(dogCenter.y, catCenter.y);
            float requiredWidth = (right - left) + padding;
            float requiredHeight = (bottom - top) + padding;
            float zoomX = requiredWidth / baseViewSize.x;
            float zoomY = requiredHeight / baseViewSize.y;
            targetZoom = std::max(zoomX, zoomY);
            targetZoom = std::clamp(targetZoom, minZoom, maxZoom);
            cameraCenter = cameraCenter + (targetCenter - cameraCenter) * centerLerp;
        }
		// camera-window
        else if (cameraMode == CameraMode::CameraWindow) {
            sf::Vector2f cameraWindowSize = baseViewSize - sf::Vector2f(padding / 2, padding / 2);
            sf::Vector2f windowLeftTop = cameraCenter - cameraWindowSize * 0.5f;
            sf::Vector2f windowRightBottom = cameraCenter + cameraWindowSize * 0.5f;

            sf::Vector2f camDelta(0.f, 0.f);

            auto checkPlayerWindow = [&](sf::Sprite& player) {
                sf::Vector2f delta(0.f, 0.f);
                sf::Vector2f playerBound = player.getLocalBounds().getCenter();
                sf::Vector2f playerPosition = player.getPosition();
                sf::Vector2f halfSize(playerBound.x / 2.f, playerBound.y / 2.f);

                if (playerPosition.x - halfSize.x < windowLeftTop.x) delta.x = playerPosition.x - halfSize.x - windowLeftTop.x;
                else if (playerPosition.x + halfSize.x > windowRightBottom.x) delta.x = playerPosition.x + halfSize.x - windowRightBottom.x;
                if (playerPosition.y - halfSize.y < windowLeftTop.y) delta.y = playerPosition.y - halfSize.y - windowLeftTop.y;
                else if (playerPosition.y + halfSize.y > windowRightBottom.y) delta.y = playerPosition.y + halfSize.y - windowRightBottom.y;

                return delta;
                };

            camDelta += checkPlayerWindow(dogPlayer);
            camDelta += checkPlayerWindow(catPlayer);

            cameraCenter += camDelta;

            clampToCameraWindow(dogPlayer);
            clampToCameraWindow(catPlayer);

            targetZoom = 0.75f;
        }

        currentZoom = currentZoom + (targetZoom - currentZoom) * zoomLerp;
        view.setSize({ baseViewSize.x * currentZoom, baseViewSize.y * currentZoom });
        view.setCenter(cameraCenter);

        window.setView(view);
        window.clear(sf::Color(255, 241, 203));
        for (auto& obj : objects) window.draw(obj);
        window.draw(dogPlayer);
        window.draw(catPlayer);
        window.display();
    }

    return 0;
}
