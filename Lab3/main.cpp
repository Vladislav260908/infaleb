#include <SFML/Graphics.hpp>

int main()
{
    constexpr int n = 10;
    constexpr float cellSize = 50.f;

    sf::RenderWindow window(
        sf::VideoMode(500, 500),
        "Grid 10x10"
    );

    sf::RectangleShape cell({ cellSize, cellSize });
    cell.setOutlineThickness(1.f);
    cell.setOutlineColor(sf::Color(190, 190, 190));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                cell.setPosition(j * cellSize, i * cellSize);

                if (i < j)
                    cell.setFillColor(sf::Color::Green);
                else
                    cell.setFillColor(sf::Color::White);

                window.draw(cell);
            }
        }

        window.display();
    }

    return 0;
}