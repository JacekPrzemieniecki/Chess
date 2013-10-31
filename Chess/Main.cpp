#include "BoardInterface.h"
#include <SFML/Graphics.hpp>
#include "Test.h"
#include <iostream>

int main()
{
	Test t;
	clock_t begin = clock();
	t.GeneratorTest();
	clock_t end = clock();
	std::cout << "Time: " << double(end - begin) / CLOCKS_PER_SEC << std::endl;
	sf::RenderWindow window(sf::VideoMode(700, 700), "Chess", sf::Style::Close);
	window.setFramerateLimit(30);
	BoardInterface boardInterface(window);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				boardInterface.OnClick(event.mouseButton.x, event.mouseButton.y);
			}
		}

		window.clear();
		boardInterface.Draw();
		window.display();
	}

	return 0;
}