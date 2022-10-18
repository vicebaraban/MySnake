#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>


class Map
{
public:
	int size;
	std::vector<std::vector<int>> field;
	Map(int s)
	{
		size = s;
		for (int i = 0; i < size; i++)
			field.push_back(std::vector<int>(size, 0));
	}
	void update(std::pair<int, int> itemCoords, std::vector<std::pair<int, int>> snakeCoords)
	{
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				field[i][j] = 0;
		field[itemCoords.first][itemCoords.second] = 1;
		field[snakeCoords[0].first][snakeCoords[0].second] = 2;
		for (int i = 1; i < snakeCoords.size(); i++)
			field[snakeCoords[i].first][snakeCoords[i].second] = 3;
	}
};


class Snake
{
public:
	int cooldown = 0;
	int speed = 8;
	bool eat = false;
	std::vector<std::pair<int, int>> body;
	Snake(std::pair<int, int> headCoords, std::pair<int, int> direction)
	{
		body.push_back(headCoords);
		for (int i = 1; i < 3; i++)
		{
			std::pair<int, int> element(headCoords.first - i * direction.first,
				headCoords.second - i * direction.second);
			body.push_back(element);
		}
	}
	void update(std::pair<int, int> dir)
	{
		if (cooldown == speed)
		{
			std::pair<int, int> head(body[0].first + dir.first, body[0].second + dir.second);
			body.insert(body.begin(), head);
			if (not eat) body.pop_back();
			else eat = false;
			cooldown = 0;
		}
		else cooldown++;
	}
};


int randomNumber(int min, int max)
{
	std::srand(time(0));
	int num = min + std::rand() % (max - min + 1);
	return num;
}


int main()
{
	const int gameWidth = 600;
	const int gameHeight = 600;

	std::pair<int, int> direction (1, 0);
	std::pair<int, int> item(10, 7);

	Map map(24);
	Snake snake(std::pair<int, int> (5, 6), direction);

	sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 32), "MySnake",
		sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	window.setFramerateLimit(60);

	sf::Clock clock;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && direction.first == 0)
		{
			direction.first = -1;
			direction.second = 0;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && direction.first == 0)
		{
			direction.first = 1;
			direction.second = 0;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && direction.second == 0)
		{
			direction.first = 0;
			direction.second = -1;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && direction.second == 0)
		{
			direction.first = 0;
			direction.second = 1;
		}

		snake.update(direction);

		for (int i = 1; i < snake.body.size(); i++)
			if (snake.body[0] == snake.body[i])
			{
				window.close();
				break;
			}
		if (snake.body[0].first == -1 || snake.body[0].first == map.size ||
			snake.body[0].second == -1 || snake.body[0].second == map.size)
		{
			window.close();
			break;
		}

		if (snake.body[0] == item)
		{
			snake.eat = true;
			while (true)
			{
				int x = randomNumber(0, map.size - 1);
				int y = randomNumber(0, map.size - 1);
				bool check = true;
				for (int i = 0; i < snake.body.size(); i++)
				{
					if (std::pair<int, int>(x, y) == snake.body[i]) check = false;
				}
				if (check)
				{
					item.first = x;
					item.second = y;
					break;
				}
				else check = true;
			}
		}
		map.update(item, snake.body);

		window.clear(sf::Color(0, 255, 0));
		for (int i = 0; i < map.size; i++)
		{
			for (int j = 0; j < map.size; j++)
			{
				int elem = map.field[i][j];
				if (elem != 0)
				{
					sf::RectangleShape creature(sf::Vector2f(25, 25));
					creature.setPosition(25 * i, 25 * j);
					if (elem == 1) creature.setFillColor(sf::Color(255, 0, 0));
					else if (elem == 2) creature.setFillColor(sf::Color(0, 100, 0));
					else creature.setFillColor(sf::Color(0, 180, 0));
					window.draw(creature);
				}
			}
		}
		window.display();
	}
	return 0;
}
