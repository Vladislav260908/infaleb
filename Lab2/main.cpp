#include <SFML/Graphics.hpp>
#include <functional>
#include <cmath>
#include <string>

// Функция для отрисовки графика
void drawGraph(sf::RenderWindow& window, std::function<float(float)> func,
    float xMin, float xMax, float scaleX, float scaleY,
    sf::Color color) {
    sf::VertexArray graph(sf::LineStrip);

    for (float x = xMin; x <= xMax; x += 0.1f) {
        float y = func(x);

        // Преобразование координат в экранные
        float screenX = 400 + x * scaleX;
        float screenY = 300 - y * scaleY;

        // Добавление точки в массив вершин
        graph.append(sf::Vertex(sf::Vector2f(screenX, screenY), color));
    }

    window.draw(graph);
}

int main() {
    // Создание окна
    sf::RenderWindow window(sf::VideoMode(800, 600), "Graphs");

    // Переменная для хранения пользовательской точки
    sf::CircleShape userPoint(5);
    userPoint.setFillColor(sf::Color::Red);
    bool userPointExists = false;

    // Загрузка шрифта
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1;
    }

    // Текст для отображения координат точки
    sf::Text coordinatesText;
    coordinatesText.setFont(font);
    coordinatesText.setCharacterSize(20);
    coordinatesText.setPosition(10, 10);
    coordinatesText.setFillColor(sf::Color::White);

    // Ось X
    sf::VertexArray xAxis(sf::Lines, 2);
    xAxis[0].position = sf::Vector2f(50, 300);
    xAxis[0].color = sf::Color::White;
    xAxis[1].position = sf::Vector2f(750, 300);
    xAxis[1].color = sf::Color::White;

    // Ось Y
    sf::VertexArray yAxis(sf::Lines, 2);
    yAxis[0].position = sf::Vector2f(400, 50);
    yAxis[0].color = sf::Color::White;
    yAxis[1].position = sf::Vector2f(400, 550);
    yAxis[1].color = sf::Color::White;

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Проверка клика мыши
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Позиция клика на экране
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    // Перевод координат экрана в математические
                    float mathX = (mousePos.x - 400) / 30.0f;
                    float mathY = -(mousePos.y - 300) / 10.0f;

                    // Ставим точку туда, куда кликнули
                    userPoint.setPosition(
                        mousePos.x - userPoint.getRadius(),
                        mousePos.y - userPoint.getRadius()
                    );
                    userPointExists = true;

                    // Определение зоны
                    float parabola = mathX * mathX - 2 * mathX - 6;
                    float line = 2;
                    int zone = 0;

                    // Если точка на линии или параболе — это граница
                    if (std::abs(mathY - line) < 0.05f ||
                        std::abs(mathY - parabola) < 0.05f) {
                        zone = 5;
                    }
                    else if (mathY > line) {
                        if (mathY < parabola) {
                            if (mathX < 1) {
                                zone = 1;
                            }
                            else {
                                zone = 4;
                            }
                        }
                    }
                    else {
                        if (mathY > parabola) {
                            zone = 3;
                        }
                        else {
                            zone = 2;
                        }
                    }

                    // Вывод слова Border или номера зоны
                    std::string zoneText;

                    if (zone == 5) {
                        zoneText = "Border";
                    }
                    else {
                        zoneText = "Zone: " + std::to_string(zone);
                    }

                    // Обновление текста
                    coordinatesText.setString(
                        "Coordinates: (" + std::to_string(mathX) + ", " +
                        std::to_string(mathY) + ")\n" + zoneText
                    );
                }
            }
        }

        // Очистка экрана
        window.clear(sf::Color::Black);

        // Отрисовка осей
        window.draw(xAxis);
        window.draw(yAxis);

        // Отрисовка параболы
        drawGraph(window,
            [](float x) { return x * x - 2 * x - 6; },
            -5, 7, 30, 10, sf::Color::Red);

        // Отрисовка прямой y = 2
        drawGraph(window,
            [](float x) { return 2.0f; },
            -10, 10, 30, 10, sf::Color::Blue);

        // Отрисовка точки и текста
        if (userPointExists) {
            window.draw(userPoint);
            window.draw(coordinatesText);
        }

        // Показ нового кадра
        window.display();
    }

    return 0;
}