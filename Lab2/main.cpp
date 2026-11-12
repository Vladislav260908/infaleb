#include <SFML/Graphics.hpp>
#include <functional>
#include <cmath>
#include <string>

// Функция для отрисовки графика
void drawGraph(sf::RenderWindow& window, std::function<float(float)> func, float xMin, float xMax, float scaleX, float scaleY, sf::Color color) {
    sf::VertexArray graph(sf::LinesStrip);

    for (float x = xMin; x <= xMax; x += 0.1f) {
        float y = func(x); // Вычисление значения функции

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
    sf::CircleShape userPoint(5); // Радиус 5 пикселей
    userPoint.setFillColor(sf::Color::Red);
    bool userPointExists = false; // Есть ли точка на экране

    // 1 _ Загрузка шрифта
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) { // файл arial.ttf должен лежать рядом с программой
        return -1;                         // если шрифт не загрузился - выходим
    }

    // 2 _ Текст для отображения координат точки
    sf::Text coordinatesText;
    coordinatesText.setFont(font);
    coordinatesText.setCharacterSize(20);
    coordinatesText.setPosition(10, 10);
    coordinatesText.setFillColor(sf::Color::White);

    // Оси X и Y
    sf::VertexArray xAxis(sf::Lines, 2);
    xAxis[0].position = sf::Vector2f(50, 300);
    xAxis[0].color = sf::Color::White;
    xAxis[1].position = sf::Vector2f(750, 300);
    xAxis[1].color = sf::Color::White;

    sf::VertexArray yAxis(sf::Lines, 2);
    yAxis[0].position = sf::Vector2f(400, 50);
    yAxis[0].color = sf::Color::White;
    yAxis[1].position = sf::Vector2f(400, 550);
    yAxis[1].color = sf::Color::White;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Проверка клика мыши
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Позиция клика на экране
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    // Перевод экранных координат в "математические"
                    // Масштаб: 30 пикселей на 1 по X, 10 пикселей на 1 по Y
                    float mathX = (mousePos.x - 400) / 30.0f;
                    float mathY = -(mousePos.y - 300) / 10.0f;

                    // Ставим точку туда, куда кликнули
                    userPoint.setPosition(mousePos.x - userPoint.getRadius(), mousePos.y - userPoint.getRadius());
                    userPointExists = true;

                    // 3 _ Определение зоны
                    float parabola = mathX * mathX - 2 * mathX - 6; // y параболы при этом x
                    float line = 2;                                  // y прямой
                    int zone = 0;                                    // 0 = точка не в пронумерованной зоне

                    if (mathY > line) {              // точка ВЫШЕ прямой y = 2
                        if (mathY < parabola) {      // и НИЖЕ параболы (снаружи от неё)
                            if (mathX < 1) {         // 1 - это x вершины параболы
                                zone = 1;            // слева
                            }
                            else {
                                zone = 4;            // справа
                            }
                        }
                        // иначе точка внутри параболы над прямой - без номера (0)
                    }
                    else {                         // точка НИЖЕ прямой y = 2
                        if (mathY > parabola) {      // и ВЫШЕ параболы (внутри неё)
                            zone = 3;
                        }
                        else {                     // и НИЖЕ параболы
                            zone = 2;
                        }
                    }

                    // Обновляем текст: координаты + номер зоны
                    coordinatesText.setString("Coordinates: (" + std::to_string(mathX) + ", " + std::to_string(mathY) + ")\n"
                        + "Zone: " + std::to_string(zone));
                }
            }
        }

        // 4 _ Очистка экрана
        window.clear(sf::Color::Black);

        // Отрисовка осей
        window.draw(xAxis);
        window.draw(yAxis);

        // 5 _ Отрисовка графика y = x*x - 2*x - 6 (красный)
        drawGraph(window, [](float x) { return x * x - 2 * x - 6; }, -5, 7, 30, 10, sf::Color::Red);

        // 5 _ Отрисовка графика y = 2 (синий)
        drawGraph(window, [](float x) { return 2.0f; }, -10, 10, 30, 10, sf::Color::Blue);

        // Отрисовка пользовательской точки, если она есть
        if (userPointExists) {
            window.draw(userPoint);
            window.draw(coordinatesText);
        }

        // Показ нового кадра
        window.display();
    }

    return 0;
}