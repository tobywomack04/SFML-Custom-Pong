#include "Ball.h"

// This the constructor function
Ball::Ball(float startX, float startY)
{
	m_Position.x = startX;
	m_Position.y = startY;

	m_Shape.setSize(sf::Vector2f(10, 10));
	m_Shape.setPosition(m_Position);

	m_Shape.setRotation(45.f);
}

FloatRect Ball::getPosition()
{
	return m_Shape.getGlobalBounds();
}

RectangleShape Ball::getShape()
{
	return m_Shape;
}

float Ball::getXVelocity()
{
	return m_DirectionX;
}

void Ball::reboundSides()
{
	m_DirectionX = -m_DirectionX;
}

void Ball::reboundBatOrTop()
{
	m_DirectionY = -m_DirectionY;
}

void Ball::reboundBottom()
{
	m_Position.y = 400;
	m_Position.x = 500;
}

void Ball::update(Time dt)
{
	// Update the ball position variables
	m_Position.y += m_DirectionY * m_Speed * dt.asSeconds();
	m_Position.x += m_DirectionX * m_Speed * dt.asSeconds();

	// Move the ball and the bat
	m_Shape.setPosition(m_Position);
}

float Ball::getXCord()
{
	return m_Position.x;
}

float Ball::getYCord()
{
	return m_Position.y;
}

int Ball::getDamage()
{
	return damage;
}

void Ball::setDamage(int dam)
{
	damage = dam;
}

float Ball::getSpeed()
{
	return m_Speed;
}

void Ball::setSpeed(float speed)
{
	m_Speed = speed;
}