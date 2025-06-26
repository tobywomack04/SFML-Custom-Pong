#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>

using namespace sf;

class Bat
{
private:
	Vector2f m_Position;

	// A RectangleShape object
	RectangleShape m_Shape;

	float m_Speed = 1000.0f;

	bool m_MovingRight = false;
	bool m_MovingLeft = false;

public:
	Bat(float startX, float startY, Color fillColor);

	FloatRect getPosition();

	RectangleShape getShape();

	void moveLeft();

	void moveRight();

	void stopLeft();

	void stopRight();

	void update(Time dt);

	void setSpeed(float speed);

	void subSpeed(float speed);

	float getXCord();

	float getYCord();

	void setColor(Color fillColor);
};