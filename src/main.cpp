#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>
#include <math.h>

const long double pi = 3.141592653589793238462643383279502884L;
const int windowWidth = 960;
const int windowHeight = 540;

enum direction {
	NONE = 0,
	FORWARD = -1,
	BACKWARD = 1,
	RIGHT = 1,
	LEFT = -1,
};

class Car {
private:
	float x, y, angle, speed, turnSpeed;
	bool turning;
	enum direction direction;
	enum direction turnDirection;
	sf::Vector2i velocity;
	sf::Texture texture;
	sf::Sprite sprite;

public:
	Car(int speed, int turnSpeed) {
		x = (float) windowWidth / 2;
		y = (float) windowHeight / 2;
		angle = 0;
		this->speed = speed;
		this->turnSpeed = turnSpeed;
		turning = false;
		direction = NONE;
		turnDirection = NONE;
		texture.loadFromFile("res/car.png");
		sprite.setTexture(texture);
		sprite.setOrigin((float)texture.getSize().x / 2, (float)texture.getSize().y / 2);
		sprite.setRotation(90);
	}

	void move(enum direction d) {
		direction = d;
	}

	void turn(enum direction d) {
		turnDirection = d;
	}

	void update(float dt) {
		angle += turnSpeed * dt * turnDirection;
		velocity.x = cos(angle * pi / 180) * speed * direction;
		velocity.y = sin(angle * pi / 180) * speed * direction;
		x += velocity.x * dt;
		y += velocity.y * dt;
		sprite.setRotation(90.f + angle);
		sprite.setPosition(x, y);
	}

	void draw(sf::RenderWindow& window) {
		window.draw(sprite);
	}
};

int main() {
	int options = sf::Style::Titlebar;
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Car racing", options);
	Car car(500.f, 360.f);

	sf::Clock clock;
	while (window.isOpen()) {
		sf::Event evt;
		while (window.pollEvent(evt)) {
			if (evt.type == sf::Event::Closed) {
				window.close();
				break;
			}
		}

		float dt = clock.restart().asSeconds();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			car.move(FORWARD);
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			car.move(BACKWARD);
		} else {
			car.move(NONE);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			car.turn(RIGHT);
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			car.turn(LEFT);
		} else {
			car.turn(NONE);
		}

		window.clear(sf::Color::Blue);
		car.update(dt);
		car.draw(window);
		window.display();
	}

	return 0;
}