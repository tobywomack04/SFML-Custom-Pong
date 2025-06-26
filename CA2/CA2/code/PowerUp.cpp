#include "PowerUp.h"

// This the constructor function
PowerUp::PowerUp(float startX, float startY, Color fillColor)
{
	m_Position.x = startX;
	m_Position.y = startY;

	m_Shape.setRadius(10);
	m_Shape.setPosition(m_Position);

	m_Shape.setFillColor(fillColor);
}

void PowerUp::update(Time dt)
{
	// Update the ball position variables
	m_Position.y += m_DirectionY * m_Speed * dt.asSeconds();
	m_Position.x += m_DirectionX * m_Speed * dt.asSeconds();

	// Move the ball and the bat
	m_Shape.setPosition(m_Position);
}

FloatRect PowerUp::getPosition()
{
	return m_Shape.getGlobalBounds();
}

void PowerUp::setPosition(float xPos, float yPos)
{
	m_Position.x = xPos;
	m_Position.y = yPos;
}

CircleShape PowerUp::getShape()
{
	return m_Shape;
}

float PowerUp::getXVelocity()
{
	return m_DirectionX;
}

void PowerUp::setSpeed(float speed)
{
	m_Speed = speed;
}

float PowerUp::getXCord()
{
	return m_Position.x;
}

float PowerUp::getYCord()
{
	return m_Position.y;
}