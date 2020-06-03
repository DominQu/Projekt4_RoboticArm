#include "Robot.h"
//Private init functions
void Robot::initVariables()
{
	this->window = nullptr;

	this->armsize.x = 700.f;
	this->armsize.y = 100.f;
	this->armorigin.x = 0.f;
	this->armorigin.y = (this->armsize.y / 2);

	//arm1
	this->arm1angle = -22.5f;
	this->arm1position.x = 700.f;
	this->arm1position.y = 1078.f;

	//arm2
	this->arm2angle = 90.f;
	this->arm2position.x = this->arm1position.x + this->armsize.x * std::sinf(this->arm1angle*(this->pi/180.f));
	this->arm2position.y = this->arm1position.y - this->armsize.x * std::cosf(this->arm1angle * (this->pi / 180.f));
	
	//gripper
	this->grippersize.x = 200.f;
	this->grippersize.y = 100.f;
	this->gripperangle = 180.f;
	this->gripperposition.x = this->arm2position.x + this->armsize.x * std::sinf((this->arm2angle) * (this->pi / 180.f));
	this->gripperposition.y = this->arm2position.y - this->armsize.x * std::cosf((this->arm2angle) * (this->pi / 180.f));

	//circle1&2&3
	this->radius = (this->armsize.y / 2) + 20.f;

	//Variables
	this->movementAngle = 1.f;
}

void Robot::initWindow()
{
	this->videomode.height = 1080;
	this->videomode.width = 1920;
	
	this->window = new sf::RenderWindow(videomode, "Robotic Arm", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(60);
}
void Robot::initTexture()
{
	this->texture = new sf::Texture;
	this->texture->loadFromFile("metal6.png");
}
void Robot::initArm()
{
	//Drawing arm in default state
	//arm1
	this->arm1.setSize(armsize);
	this->arm1.setOrigin(armorigin);
	this->arm1.setPosition(arm1position);
	this->arm1.setRotation(arm1angle - 90.f);
	this->arm1.setTexture(this->texture);
	this->arm1.setOutlineColor(sf::Color::Black);
	this->arm1.setOutlineThickness(2.f);

	//arm2
	this->arm2.setSize(armsize);
	this->arm2.setOrigin(armorigin);
	this->arm2.setPosition(arm2position);
	this->arm2.setRotation(arm2angle - 90.f);
	this->arm2.setTexture(this->texture);
	this->arm2.setOutlineColor(sf::Color::Black);
	this->arm2.setOutlineThickness(2.f);

	//gripper
	this->gripper.setSize(grippersize);
	this->gripper.setOrigin(armorigin);
	this->gripper.setPosition(gripperposition);
	this->gripper.setRotation(gripperangle - 90.f);
	this->gripper.setTexture(this->texture);
	this->gripper.setOutlineColor(sf::Color::Black);
	this->gripper.setOutlineThickness(2.f);

	//other parts
	this->circle1.setRadius(this->radius);
	this->circle1.setTexture(this->texture);
	this->circle1.setOutlineColor(sf::Color::Black);
	this->circle1.setOutlineThickness(2.f);
	this->circle1.setOrigin(sf::Vector2f(radius, radius));
	this->circle1.setPosition(this->arm1position);

	this->circle2.setRadius(this->radius);
	this->circle2.setTexture(this->texture);
	this->circle2.setOutlineColor(sf::Color::Black);
	this->circle2.setOutlineThickness(2.f);
	this->circle2.setOrigin(sf::Vector2f(radius, radius));
	this->circle2.setPosition(this->arm2position);

	this->circle3.setRadius(this->radius);
	this->circle3.setTexture(this->texture);
	this->circle3.setOutlineColor(sf::Color::Black);
	this->circle3.setOutlineThickness(2.f);
	this->circle3.setOrigin(sf::Vector2f(radius, radius));
	this->circle3.setPosition(this->gripperposition);

}
//Constructor and destructor
Robot::Robot()
{
	this->initVariables();
	this->initWindow();
	this->initTexture();
	this->initArm();
}

Robot::~Robot()
{
	delete this->window;
	delete this->texture;
}
//running state function
const bool Robot::isrunning() const
{
	return this->window->isOpen();
}

//Functions
void Robot::pollEvents()
{
	while (this->window->pollEvent(this->event))
	{
		switch (this->event.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				this->window->close();
			break;
		}
	}
}

void Robot::moveArm()
{
	//Updating variables
	this->arm2position.x = this->arm1position.x + this->armsize.x * std::sinf(this->arm1angle * (this->pi / 180.f));
	this->arm2position.y = this->arm1position.y - this->armsize.x * std::cosf(this->arm1angle * (this->pi / 180.f));

	this->gripperposition.x = this->arm2position.x + this->armsize.x * std::sinf((this->arm2angle) * (this->pi / 180.f));
	this->gripperposition.y = this->arm2position.y - this->armsize.x * std::cosf((this->arm2angle) * (this->pi / 180.f));
	
	//Updating elements positions
	this->arm2.setPosition(arm2position);
	this->gripper.setPosition(gripperposition);
	this->circle2.setPosition(this->arm2position);
	this->circle3.setPosition(this->gripperposition);
}

void Robot::updateArm()
{
	/* 
		Inputs from keyboard
		Moving arm1, arm2
		Updating the rest of arm elements
	*/
	//Move arm2
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		//Upper movement bounf
		if (this->arm2angle >= 45.f) {
			this->arm2.rotate(-this->movementAngle);
			this->arm2angle -= this->movementAngle;
			this->moveArm();
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		//Bottom movement bound
		if (this->gripperposition.y <= this->videomode.height - 200.f) {
			this->arm2.rotate(this->movementAngle);
			this->arm2angle += this->movementAngle;
			this->moveArm();
		}
	}
	//Move arm1
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		//Left movement bound
		if (this->arm1angle >= -45.f && this->gripperposition.y <= this->videomode.height - 200.f) {
			this->arm1.rotate(-this->movementAngle);
			this->arm1angle -= this->movementAngle;
			this->moveArm();
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		//Right movement bound
		if (this->arm1angle <= 45.f && this->gripperposition.y <= this->videomode.height - 200.f) {
			this->arm1.rotate(this->movementAngle);
			this->arm1angle += this->movementAngle;
			this->moveArm();
		}
	}

}

void Robot::update()
{
	this->pollEvents();

	this->updateArm();

}

void Robot::renderArm(sf::RenderTarget& target)
{
	target.draw(this->arm1);
	target.draw(this->arm2);
	target.draw(this->gripper);
	target.draw(this->circle1);
	target.draw(this->circle2);
	target.draw(this->circle3);
	
}

void Robot::render()
{
	this->window->clear(sf::Color::White);

	//Draw everything here
	this->renderArm(*this->window);

	this->window->display();
}
