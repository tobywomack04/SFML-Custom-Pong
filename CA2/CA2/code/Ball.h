#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Ball
{
private:
	Vector2f m_Position;	
	RectangleShape m_Shape;

	float m_Speed = 1500.0f;
	float m_DirectionX = .2f;
	float m_DirectionY = .2f;

	int damage = 1;

public:
	Ball(float startX, float startY);

	FloatRect getPosition();

	RectangleShape getShape();

	float getXVelocity();

	void reboundSides();

	void reboundBatOrTop();

	void reboundBottom();

	void update(Time dt);

	float getXCord();

	float getYCord();

	int getDamage();
	
	void setDamage(int damage);

	float getSpeed();

	void setSpeed(float speed);
};