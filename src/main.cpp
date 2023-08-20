#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
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

class Camera {
private:
	float x, y;
	sf::Vector2f target;

public:
	Camera(sf::Vector2f target) {
		aimAt(target);
	}

	sf::Vector2f position() {
		return {x, y};
	}

	void aimAt(sf::Vector2f target) {
		this->target = target;
	}

	void clamp(sf::Vector2f min, sf::Vector2f max) {
		if (x < min.x) {
			x = min.x;
		}

		if (x > max.x) {
			x = max.x;
		}

		if (y < min.y) {
			y = min.y;
		}

		if (y > max.y) {
			y = max.y;
		}
	}

	void update(float dt) {
		x = target.x;
		y = target.y;
	}
};

class Car {
private:
	float x, y, angle, acceleration, speed, turnSpeed, turnAcceleration, maxSpeed, maxTurnSpeed;
	enum direction direction;
	enum direction turnDirection;
	sf::Texture texture;
	sf::Sprite sprite;

public:
	Car(int acceleration, int turnAcceleration, int maxSpeed, int maxTurnSpeed) {
		x = (float) windowWidth / 2;
		y = (float) windowHeight / 2;
		angle = 0;
		turnSpeed = 0;
		direction = NONE;
		turnDirection = NONE;

		this->acceleration = acceleration;
		this->turnAcceleration = turnAcceleration;
		this->maxSpeed = maxSpeed;
		this->maxTurnSpeed = maxTurnSpeed;
		
		texture.loadFromFile("res/car.png");
		sprite.setTexture(texture);
		sprite.setOrigin((float)texture.getSize().x / 2, (float)texture.getSize().y / 2);
		sprite.setRotation(90);
	}

	sf::Vector2f position() {
		return {x, y};
	}

	void resetPosition() {
		x = (float) windowWidth / 2;
		y = (float) windowHeight / 2;
	}

	void move(enum direction d) {
		direction = d;
	}

	void turn(enum direction d) {
		turnDirection = d;
	}

	void update(float dt) {
		turnSpeed += dt * turnDirection * turnAcceleration;
		
		if (turnDirection == NONE) {
			turnSpeed = 0;
		}

		if (abs(turnSpeed) > maxTurnSpeed) {
			turnSpeed = maxTurnSpeed * turnDirection;
		}

		angle += turnSpeed * dt;

		speed += acceleration * dt;
		if (speed > maxSpeed) {
			speed = maxSpeed;
		}

		if (direction == NONE) {
			speed = 0;
		}

		x += cos(angle * pi / 180) * speed * direction * dt;
		y += sin(angle * pi / 180) * speed * direction * dt;
		
		sprite.setRotation(90.f + angle);
		sprite.setPosition(x, y);
	}

	void draw(sf::RenderWindow& window) {
		window.draw(sprite);
	}
};

class Map {
private:
	sf::Texture texture;
	sf::Sprite sprite;

public:
	Map() {
		texture.loadFromFile("res/map.png");
		sprite.setTexture(texture);
		sprite.setPosition(0, 0);
	}

	void draw(sf::RenderWindow& window) {
		window.draw(sprite);
	}
};

int main() {
	int options = sf::Style::Titlebar;
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Car racing", options);
	sf::View view(sf::FloatRect(0.0f, 0.0f, windowWidth, windowHeight));
	window.setView(view);

	Car car(500, 450, 500, 200);
	Camera cam(car.position());
	Map map;

	sf::Clock clock;
	while (window.isOpen()) {
		sf::Event evt;
		while (window.pollEvent(evt)) {
			if (evt.type == sf::Event::Closed) {
				window.close();
				break;
			}

			if (evt.type == sf::Event::KeyPressed) {
				if (evt.key.scancode == sf::Keyboard::Scancode::R) {
					car.resetPosition();
				}
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
		
		cam.aimAt(car.position());
		cam.update(dt);
		cam.clamp(
		{0.f + view.getSize().x / 2, 0.f + view.getSize().y / 2},
		{1920.f - view.getSize().x / 2, 1080.f - view.getSize().y / 2}
		);
		view.setCenter(cam.position());
		window.setView(view);
		
		map.draw(window);
		car.draw(window);
		window.display();
	}

	return 0;
}