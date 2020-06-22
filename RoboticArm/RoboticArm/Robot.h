#pragma once

#include <iostream>
#include <cmath>
#include <queue>
#include <vector>

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
	sf::Font font;
	sf::Text text;

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
	sf::Vector2f gripperendposition;
	sf::Vector2f circle1position;
	float arm1angle;
	float arm2angle;
	float gripperangle;
	float radius;

	//Boxes
	sf::RectangleShape *box;
	std::vector <sf::RectangleShape> boxes;
	sf::Vector2f boxsize;
	sf::Vector2f boxstartposition;
	sf::Vector2f boxorigin;


	//Saving moves mode variables
	bool isbeingsaved;
	bool followsavedpath;
	std::queue<sf::Vector2f> queue;
	sf::Vector2f arm2savedposition;
	sf::Vector2f grippersavedposition;
	float arm1savedangle;
	float arm2savedangle;
	int onetimeloading;

	//Variables
	const float pi = 3.1415926535897;
	float movementAngle;
	bool magnetonoff;


	//Initialization functions
	void initVariables();
	void initWindow();
	void initTexture();
	void initText();
	void initArm();
public:
	//Constructor and destructor
	Robot();
	~Robot();

	const bool isrunning() const;

	//Functions
	void moveArm();
	void savePosition();
	void setSavedPosition();
	void createnewbox();
	bool istouching(sf::Vector2f);
	
	void pollEvents();

	void updateSavedMovement();
	void updateSaveEvents();
	void updateArm();
	void updateBoxes();
	void update();

	void renderText(sf::RenderTarget& target);
	void renderArm(sf::RenderTarget& target);
	void renderBoxes(sf::RenderTarget& target);
	void render();
};