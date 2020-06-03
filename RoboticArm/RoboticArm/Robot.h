#pragma once

#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

class Robot
{
private:
	sf::RenderWindow* window;
	sf::VideoMode videomode;
	sf::Event event;
	sf::Texture *texture;

	//Arm parts
	sf::RectangleShape arm1;
	sf::RectangleShape arm2;
	sf::RectangleShape gripper;
	sf::CircleShape circle1;
	sf::CircleShape circle2;
	sf::CircleShape circle3;

	//Arm variables
	sf::Vector2f armsize;
	sf::Vector2f grippersize;
	sf::Vector2f armorigin;
	sf::Vector2f arm1position;
	sf::Vector2f arm2position;
	sf::Vector2f gripperposition;
	sf::Vector2f circle1position;
	float arm1angle;
	float arm2angle;
	float gripperangle;
	float radius;

	//Variables
	const float pi = 3.1415926535897;
	float movementAngle;

	//Initialization functions
	void initVariables();
	void initWindow();
	void initTexture();
	void initArm();
public:
	//Constructor and destructor
	Robot();
	~Robot();

	const bool isrunning() const;

	//Functions
	void moveArm();
	void pollEvents();
	void updateArm();
	void update();
	void renderArm(sf::RenderTarget& target);
	void render();
};

