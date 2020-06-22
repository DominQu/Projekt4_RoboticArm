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

	//Boxes
	this->boxstartposition.x = 900.f;
	this->boxstartposition.y = 980.f;
	this->boxsize.x = 100.f;
	this->boxsize.y = 100.f;
	this->boxorigin.x = this->boxsize.x / 2;
	this->boxorigin.y = 0.f;

	//Variables
	this->movementAngle = 0.3f;
	this->magnetonoff = false;

	//Saving moves mode variables
	this->isbeingsaved = false;
	this->followsavedpath = false;
	this->onetimeloading = 0;
}
void Robot::initWindow()
{
	this->videomode.height = 1080;
	this->videomode.width = 1920;
	
	this->window = new sf::RenderWindow(videomode, "Robotic Arm", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(144);
}
void Robot::initTexture()
{
	this->texture = new sf::Texture;
	this->texture->loadFromFile("metal6.png");
	
}
void Robot::initText()
{
	this->font.loadFromFile("bahnschrift.ttf");

	this->text.setFont(this->font);
	this->text.setString("Sterowanie:\nWyjscie z aplikacji - Escape\nRuchy ramieniem - strzalki\nPoczatek zapisu ruchow - S\nKoniec zapisu ruchow - E\nWczytaj zapisane ruchy - R\nStworz nowy klocek - W\nWlacz/wylacz magnes - Spacja");
	this->text.setCharacterSize(24);
	this->text.setFillColor(sf::Color::Black);
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
	this->gripperendposition.x = this->gripperposition.x;
	this->gripperendposition.y = this->gripperposition.y + this->grippersize.x;

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
	this->initText();
	this->initArm();
}
Robot::~Robot()
{
	delete this->window;
	delete this->texture;

	for (int i = this->boxes.size()-1; i >= 0; i--) 
	{
		this->boxes.erase(this->boxes.begin() + i);
	}
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
		case sf::Event::KeyReleased:
			if (this->event.key.code == sf::Keyboard::W)
			{
				this->createnewbox();
			}
			else if (this->event.key.code == sf::Keyboard::Space)
			{
				this->magnetonoff = !this->magnetonoff;
			}
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

	this->gripperendposition.x = this->gripperposition.x;
	this->gripperendposition.y = this->gripperposition.y + this->grippersize.x;
	
	//Updating elements positions
	this->arm2.setPosition(arm2position);
	this->gripper.setPosition(gripperposition);
	this->circle2.setPosition(this->arm2position);
	this->circle3.setPosition(this->gripperposition);
}

void Robot::savePosition()
{
	//saving current position and angles
	this->arm2savedposition.x = this->arm2position.x;
	this->arm2savedposition.y = this->arm2position.y;
	this->grippersavedposition.x = this->gripperposition.x;
	this->grippersavedposition.y = this->gripperposition.y;
	this->arm1savedangle = this->arm1angle;
	this->arm2savedangle = this->arm2angle;
}

void Robot::setSavedPosition()
{
	//Loading saved position, saved position is loaded only once
	if (this->onetimeloading == 1)
	{
		this->arm2position.x = this->arm2savedposition.x;
		this->arm2position.y = this->arm2savedposition.y;

		this->gripperposition.x = this->grippersavedposition.x;
		this->gripperposition.y = this->grippersavedposition.y;

		this->arm1angle = this->arm1savedangle;
		this->arm2angle = this->arm2savedangle;

		this->arm2.setPosition(arm2position);
		this->gripper.setPosition(gripperposition);

		this->arm1.setRotation(this->arm1angle - 90.f);
		this->arm2.setRotation(this->arm2angle - 90.f);

		this->circle1.setPosition(this->arm1position);
		this->circle2.setPosition(this->arm2position);
		this->circle3.setPosition(this->gripperposition);
	}
}

void Robot::createnewbox()
{
		this->box = new sf::RectangleShape;

		this->box->setSize(this->boxsize);
		this->box->setFillColor(sf::Color::Magenta);
		this->box->setOutlineThickness(2.f);
		this->box->setOutlineColor(sf::Color::Green);
		this->box->setOrigin(this->boxorigin);
		this->box->setPosition(this->boxstartposition);
		

		this->boxes.push_back(*this->box);
}

bool Robot::istouching(sf::Vector2f object)
{
	//Bool checking if an object is touching a box
	if (!this->magnetonoff)
	{
		for (auto& i : this->boxes)
		{
			if (i.getGlobalBounds().contains(object))
			{
				return true;
			}
		}
	}
	return false;
}

void Robot::updateSavedMovement()
{
	//Following saved moves
	if (this->followsavedpath && !this->queue.empty()) 
	{
		if (this->queue.front().x == 1.f)
		{
			this->arm1.rotate(this->queue.front().y);
			this->arm1angle += this->queue.front().y;
			this->moveArm();
		}
		else if (this->queue.front().x == 2.f)
		{
			this->arm2.rotate(this->queue.front().y);
			this->arm2angle += this->queue.front().y;
			this->moveArm();
		}
		
		this->queue.pop();

		if (this->queue.empty())
		{
			this->followsavedpath = false;
			this->onetimeloading = 0;
		}
	}
}

void Robot::updateSaveEvents()
{
	/*Inputs from keyboard:
		S (start saving), E (end saving), R (follow saved path) keys 
	*/
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
			this->isbeingsaved = true;
			this->savePosition();
			}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			{
			this->isbeingsaved = false;
			}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
			if (!this->queue.empty())
				{
				this->onetimeloading += 1;
				this->setSavedPosition();
				this->followsavedpath = true;
				}
			}
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
		//Upper movement bound
		if (this->arm2angle >= 45.f && this->gripperposition.y >= 75.f && this->gripperposition.x <= this->videomode.width - 75.f) {
			this->arm2.rotate(-this->movementAngle);
			this->arm2angle -= this->movementAngle;
			this->moveArm();

			//Saving moves
			if (this->isbeingsaved)
			{
				this->queue.push(sf::Vector2f(2.f, -this->movementAngle));
			}
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		//Bottom movement bound
		if (this->gripperposition.y <= this->videomode.height - 200.f && this->gripperposition.x <= this->videomode.width - 75.f && !this->istouching(this->gripperendposition)) {
			this->arm2.rotate(this->movementAngle);
			this->arm2angle += this->movementAngle;
			this->moveArm();

			//Saving moves
			if (this->isbeingsaved)
			{
				this->queue.push(sf::Vector2f(2.f, this->movementAngle));
			}
		}
	}
	//Move arm1
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		//Left movement bound
		if (this->arm1angle >= -45.f && this->gripperposition.y <= this->videomode.height - 200.f && this->gripperposition.y >= 75.f) {
			this->arm1.rotate(-this->movementAngle);
			this->arm1angle -= this->movementAngle;
			this->moveArm();

			//Saving moves
			if (this->isbeingsaved)
			{
				this->queue.push(sf::Vector2f(1.f, -this->movementAngle));
			}
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		//Right movement bound
		if (this->arm1angle <= 45.f && this->gripperposition.y <= this->videomode.height - 200.f && this->gripperposition.y >= 75.f && this->gripperposition.x <= this->videomode.width - 75.f && !this->istouching(this->gripperendposition)) {
			this->arm1.rotate(this->movementAngle);
			this->arm1angle += this->movementAngle;
			this->moveArm();

			//Saving moves
			if (this->isbeingsaved)
			{
				this->queue.push(sf::Vector2f(1.f, this->movementAngle));
			}
		}
	}
}

void Robot::updateBoxes()
{
	//Moving catched boxes
		for (int i = 0; i < this->boxes.size(); i++)
		{
			if (this->magnetonoff)
			{
				if (this->boxes[i].getGlobalBounds().intersects(this->gripper.getGlobalBounds()))
				{
					this->boxes[i].setPosition(this->gripperendposition);
				}
			}
			else
			{
				if (this->boxes[i].getPosition().y < this->videomode.height - this->boxsize.y && !this->istouching(sf::Vector2f(this->boxes[i].getPosition().x, this->boxes[i].getPosition().y + this->boxsize.y + 3.f)))
					this->boxes[i].move(sf::Vector2f(0.f, 5.f));
			}
		}
	

}

void Robot::update()
{
	this->pollEvents();

	this->updateSaveEvents();
	this->updateArm();
	this->updateSavedMovement();
	this->updateBoxes();
}

void Robot::renderText(sf::RenderTarget& target)
{
	target.draw(text);
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

void Robot::renderBoxes(sf::RenderTarget& target)
{
	for (auto& i : this->boxes)
	{
		target.draw(i);
	}

}

void Robot::render()
{
	this->window->clear(sf::Color::White);

	//Drawing everything here
	this->renderText(*this->window);
	this->renderArm(*this->window);
	this->renderBoxes(*this->window);

	this->window->display();
}
