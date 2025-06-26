#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Box
{
private:
	Vector2f m_Position;

	// A RectangleShape object
	RectangleShape m_Shape;

	// Box health
	int currentHp;
	int maxHp;

	// Box status
	bool dead;

public:
	Box(float startX, float startY, Color fillColor, int health);

	FloatRect getPosition();

	RectangleShape getShape();

	void onHit(int damage);

	int getHealth();

	void resetHealth();
};