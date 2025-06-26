#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class PowerUp
{
private:
	Vector2f m_Position;
	CircleShape m_Shape;

	float m_Speed = 0.0f;
	float m_DirectionX = 0.0f;
	float m_DirectionY = 0.4f;

public:
	PowerUp(float startX, float startY, Color fillColor);

	FloatRect getPosition();

	void setPosition(float xPos, float yPos);

	CircleShape getShape();

	float getXVelocity();

	void update(Time dt);

	void setSpeed(float speed);

	float getXCord();

	float getYCord();

};