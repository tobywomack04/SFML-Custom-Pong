#include "Box.h"

// This the constructor and it is called when we create an object
Box::Box(float startX, float startY, Color fillColor, int health)
{
	m_Position.x = startX;
	m_Position.y = startY;

	m_Shape.setSize(sf::Vector2f(160, 60));
	m_Shape.setPosition(m_Position);

	m_Shape.setFillColor(fillColor);

	currentHp = health;
	maxHp = health;

}

FloatRect Box::getPosition()
{
	return m_Shape.getGlobalBounds();
}

RectangleShape Box::getShape()
{
	return m_Shape;
}

void Box::onHit(int damage)
{
	currentHp -= damage;
}

int Box::getHealth()
{
	return currentHp;
}

void Box::resetHealth()
{
	currentHp = maxHp;
}