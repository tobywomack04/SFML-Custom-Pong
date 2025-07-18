// Pong.cpp : Defines the entry point for the console application.

// C++ libraries here
#include "Bat.h"
#include "Ball.h"
#include "Box.h"
#include "PowerUp.h"
#include <sstream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <array>
#include <fstream>
#include <SFML/Audio.hpp>

// Make code easier to write with "using namespace"
using namespace std;

int main()
{
	// Create a video mode object
	VideoMode vm(1920, 1080);

	// Create and open a window for the game

	RenderWindow window(vm, "Pong", Style::Fullscreen);

	int score = 0; // Counter for the players score
	int lives = 3; // Number of lives the player has
	int powerUpSpawn = 0; // Random integer used to decide which powerup to use
	int xPos = 0; // Random X position of the powerups spawn location
	int yPos = 0; // Random Y position of the powerups spawn location
	int count = 0; // Used for executing freeze timer restart once
	int counter = 0; // Used to set the balls speed once at the start of the game
	int highScore = 0; //Integer used to store the current highest score
	int deadBoxes = 0; // Integer to count number of dead boxes 
	float speed = 0; // Random speed of the powerup

	// Array of boxes to store all the box objects
	Box boxesArray[] = { 
		Box (0, 70, Color(0, 182, 236), 50),
		Box (160, 70, Color(255, 184, 83), 50),
		Box (320, 70, Color(0, 182, 236), 50),
		Box (480, 70, Color(255, 184, 83), 50),
		Box (640, 70, Color(0, 182, 236), 50),
		Box (800, 70, Color(255, 184, 83), 50),
		Box (960, 70, Color(0, 182, 236), 50),
		Box (1120, 70, Color(255, 184, 83), 50),
		Box (1280, 70, Color(0, 182, 236), 50),
		Box (1440, 70, Color(255, 184, 83), 50),
		Box (1600, 70, Color(0, 182, 236), 50),
		Box (1760, 70, Color(255, 184, 83), 50),
		
		Box (0, 130, Color(255, 184, 83), 10),
		Box (160, 130, Color(0, 182, 236), 10),
		Box (320, 130, Color(255, 184, 83), 10),
		Box (480, 130, Color(0, 182, 236), 10),
		Box (640, 130, Color(255, 184, 83), 10),
		Box (800, 130, Color(0, 182, 236), 10),
		Box (960, 130, Color(255, 184, 83), 10),
		Box (1120, 130, Color(0, 182, 236), 10),
		Box (1280, 130, Color(255, 184, 83), 10),
		Box (1440, 130, Color(0, 182, 236), 10),
		Box (1600, 130, Color(255, 184, 83), 10),
		Box(1760, 130, Color(0, 182, 236), 10),

		Box(0, 190, Color(0, 182, 236), 1),
		Box(160, 190, Color(255, 184, 83), 1),
		Box(320, 190, Color(0, 182, 236), 1),
		Box(480, 190, Color(255, 184, 83), 1),
		Box(640, 190, Color(0, 182, 236), 1),
		Box(800, 190, Color(255, 184, 83), 1),
		Box(960, 190, Color(0, 182, 236), 1),
		Box(1120, 190, Color(255, 184, 83), 1),
		Box(1280, 190, Color(0, 182, 236), 1),
		Box(1440, 190, Color(255, 184, 83), 1),
		Box(1600, 190, Color(0, 182, 236), 1),
		Box(1760, 190, Color(255, 184, 83), 1),
	};

	// Arrays of booleans
	bool takeDamageArr[sizeof(boxesArray) / sizeof(boxesArray[0])];
	bool acceptScoreArr[sizeof(boxesArray) / sizeof(boxesArray[0])];

	// Setting all booleans in the array to true
	for (int i = 0; i < sizeof(boxesArray) / sizeof(boxesArray[0]); i++)
	{
		takeDamageArr[i] = true;
	}

	for (int i = 0; i < sizeof(boxesArray) / sizeof(boxesArray[0]); i++)
	{
		acceptScoreArr[i] = true;
	}

	bool ddPowerUp = false; // Boolean for whether the double damage powerup is active
	bool siPowerup = false; // Boolean for whether the speed increase powerup is active
	bool fePowerup = false; // Boolean for whether the freeze enemy powerup is active
	bool frozen = false; // Boolean for whether the enemy bat is frozen or not
	bool paused = true; // Boolean for whether the game is paused or not

	// Create two bat
	Bat playerBat(1920 / 2, 1080 - 20, Color::Green);
	Bat enemyBat(1920 / 2, 540, Color::Red);

	// Creating a ball object and setting its speed to zero
	Ball ball(1920 / 2, 400);
	ball.setSpeed(0.0f);

	// Creating Text objects
	Text hud;
	Text controls;

	// A retro-style font
	Font font;
	font.loadFromFile("fonts/DS-DIGI.ttf");

	// Setting text objects font to the retro-style font
	hud.setFont(font);
	controls.setFont(font);

	// Setting the chracter size of the text objects to 40
	hud.setCharacterSize(40);
	controls.setCharacterSize(60);

	// Setting a color for the text objects
	hud.setFillColor(Color::White);
	controls.setFillColor(Color::White);

	// Setting the position of the text objects
	hud.setPosition(20, 5);
	controls.setPosition(700, 400);

	// Making a rectangle shape to act as a top border
	sf::RectangleShape border(sf::Vector2f(1920, 5));
	border.setPosition(0, 65);

	//Powerup objects
	PowerUp doubleDamagePowerup(100, 20, Color(139, 0, 0));
	PowerUp speedIncreasePowerup(100, 20, Color(100, 250, 50));
	PowerUp freezeEnemyPowerup(100, 20, Color(173, 216, 230));

	// Here are our clocks for timing everything
	Clock clock; // Clock used to get delta time
	Clock enemyBatClock; // Clock for deciding when the enemy bat should move and for how long
	Clock powerUpClock; // Clock for deciding when to spawn a powerup
	Clock freezeClock; // Clock to count duration of freeze effect

	// Setting the text for the controls text object
	string cont = "Left arrow: move left \nRight arrow: move right \n\nPress space to start";
	controls.setString(cont);

	// Load the highscore from a text file
	ifstream inputFile("gameData/scores.txt");
	if (inputFile.is_open())
	{
		inputFile >> highScore;
		inputFile.close();
	}

	SoundBuffer bgMusicBuffer;
	bgMusicBuffer.loadFromFile("sound/theme.wav");
	Sound bgMusic;
	bgMusic.setBuffer(bgMusicBuffer);

	bgMusic.setLoop(true);

	while (window.isOpen())
	{
		/*
		*********************************************************************
		Handle the player input
		*********************************************************************
		*/

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{	
				// Quit the game when the window is closed
				window.close();
			}
		}

		// Handle the player quitting
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			// Write the highscore to the text file
			if (highScore < score)
			{
				highScore = score;
				std::ofstream outputFile("gamedata/scores.txt");
				outputFile << highScore;
				outputFile.close();
			}

			window.close();
		}

		// Handle the player starting the game
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			paused = false;
		}

		if (!paused)
		{
			// Handle the pressing and releasing of the left and right arrow keys
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				playerBat.moveLeft();
			}
			else
			{
				playerBat.stopLeft();
			}

			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				playerBat.moveRight();
			}
			else
			{
				playerBat.stopRight();
			}
		}

		/*
		*********************************************************************
		Update all elements of the game
		*********************************************************************
		*/

		// Making the ball move once the game has started
		if (!paused && counter == 0)
		{
			ball.setSpeed(1500.0f);
			bgMusic.play();
			counter = 1;
		}
		
		// Update the delta time
		Time dt = clock.restart();
		playerBat.update(dt);
		enemyBat.update(dt);
		ball.update(dt);
		doubleDamagePowerup.update(dt);
		speedIncreasePowerup.update(dt);
		freezeEnemyPowerup.update(dt);

		// Update the text elements
		stringstream ss;
		ss << "Score:" << score << "    Lives:" << lives << "    Highscore:" << highScore;
		hud.setString(ss.str());

		// Handle ball hitting the bottom
		if (ball.getPosition().top > window.getSize().y)
		{
			// reverse the ball direction
			ball.reboundBottom();

			// Remove a life
			lives--;

			// Reduce speed by 300
			playerBat.subSpeed(200.0f);

			// Check for zero lives
			if (lives < 1) {
				// Set highscore to score 
				if (highScore < score)
				{
					highScore = score;
				}
				
				// reset the score
				score = 0;

				// Reset the lives
				lives = 3;

				// Set the speed of the player's bat back to 1000
				playerBat.setSpeed(1500.0f);

				// Reset box health
				for (int i = 0; i < sizeof(boxesArray) / sizeof(boxesArray[0]); i++)
				{
					boxesArray[i].resetHealth();
				}

				// Reset the frozen boolean
				frozen = false;

				// Setting ball's damage back to 1
				ball.setDamage(1);
			}
		}

		// Handle ball hitting top
		if (ball.getPosition().intersects(border.getGlobalBounds()))
		{
			ball.reboundBatOrTop();
		}

		// Handle ball hitting sides
		if (ball.getPosition().left < 0 || ball.getPosition().left + 10 > window.getSize().x)
		{
			ball.reboundSides();
		}

		// Has the ball hit either bat or bat2?
		if (ball.getPosition().intersects(playerBat.getPosition()) || ball.getPosition().intersects(enemyBat.getPosition()))
		{
			// Hit detected so reverse the ball and score a point
			ball.reboundBatOrTop();
		}

		// Handles tracking of how long the enemy bat can move for and when it can move
		Time enemyTiming = enemyBatClock.getElapsedTime();

		if (!paused)
		{
			// Handle the enemy bat tracking the ball
			if (!frozen && (int)enemyTiming.asSeconds() % 5 == 0)
			{
				if (enemyBat.getXCord() + 50 > ball.getXCord() && enemyBat.getYCord() < ball.getYCord() && enemyBat.getXCord() > 1)
				{
					enemyBat.stopRight();
					enemyBat.moveLeft();
				}
				else if (enemyBat.getXCord() + 50 < ball.getXCord() && enemyBat.getYCord() < ball.getYCord() && enemyBat.getXCord() < 1820)
				{
					enemyBat.stopLeft();
					enemyBat.moveRight();
				}
				else if (ball.getXCord() + 50 >= enemyBat.getXCord())
				{
					enemyBat.stopLeft();
					enemyBat.stopRight();
				}
			}
			else if ((int)enemyTiming.asSeconds() % 6 == 0)
			{
				enemyBatClock.restart();
			}
			else if (frozen || (int)enemyTiming.asSeconds() % 5 != 0)
			{
				enemyBat.stopLeft();
				enemyBat.stopRight();
			}
		}

		// Stops enemy bat at the edge of the screen
		if (enemyBat.getXCord() < 1)
		{
			enemyBat.stopLeft();
		}

		if (enemyBat.getXCord() > 1820)
		{
			enemyBat.stopRight();
		}

		// Handle the enemy bat becoming frozen
		if (frozen)
		{
			enemyBat.setColor(Color::Cyan);
			if (count == 0)
			{
				freezeClock.restart();
			}
			count = 1;
			Time freezeTime = freezeClock.getElapsedTime();

			if (freezeTime.asSeconds() > 4)
			{
				frozen = false;
				count = 0;
			}
		}

		// Set colour of enemy bat to red if not frozen
		if (!frozen)
		{
			enemyBat.setColor(Color::Red);
		}

		// Handle the ball hitting a block
		for (int i = 0; i < sizeof(boxesArray) / sizeof(boxesArray[0]); i++)
		{
			if ((boxesArray[i].getHealth() > 0) && (ball.getPosition().intersects(boxesArray[i].getPosition())) && (takeDamageArr[i] = true)) // If ball has hit box don't allow it to damage the box again
			{
				takeDamageArr[i] = false;
				ball.reboundBatOrTop();
				boxesArray[i].onHit(ball.getDamage());
				if (acceptScoreArr[i])
				{
					score += ball.getDamage();
				}
				acceptScoreArr[i] = false;
			}
			else if (!ball.getPosition().intersects(boxesArray[i].getPosition())) // Allow the ball to damage the box once they're no longer colliding
			{
				takeDamageArr[i] = true;
				acceptScoreArr[i] = true;
			}
		}

		// Handles the amount of time between each powerup spawn
		Time powerUpTiming = powerUpClock.getElapsedTime();

		if (!paused)
		{
			if (powerUpTiming.asSeconds() > 7)
			{
				srand((int)time(0) * 20); // Seeding randomiser
				powerUpSpawn = rand() % 3; // Random number between 0 and 2
				xPos = 10 + (rand() % 1901); // Random number between 1910 and 10
				yPos = 300 + (rand() % 771); // Random number between 1070 and 300
				speed = 500 + (rand() % 301); // Random number between 800 and 300

				switch (powerUpSpawn)
				{
				case 0:
					if (!ddPowerUp) // Double damage powerup chosen
					{
						doubleDamagePowerup.setPosition(xPos, yPos);
						doubleDamagePowerup.setSpeed(speed);
						ddPowerUp = true;
					}
					break;

				case 1:
					if (!siPowerup) // Increase speed powerup chosen
					{
						speedIncreasePowerup.setPosition(xPos, yPos);
						speedIncreasePowerup.setSpeed(speed);
						siPowerup = true;
					}
					break;

				case 2:
					if (!fePowerup) // Freeze enemy powerup chosen
					{
						freezeEnemyPowerup.setPosition(xPos, yPos);
						freezeEnemyPowerup.setSpeed(speed);
						fePowerup = true;
					}
					break;
				}

				powerUpClock.restart(); // Restart the clock
			}
		}

		// Handling powerups falling off the screen & intersecting with player bat
		if (doubleDamagePowerup.getPosition().intersects(playerBat.getPosition()))
		{
			ball.setDamage(ball.getDamage() * 2);
			doubleDamagePowerup.setPosition(100, 20);
			doubleDamagePowerup.setSpeed(0);
			ddPowerUp = false;
		}
		else if (doubleDamagePowerup.getYCord() > 1080)
		{
			doubleDamagePowerup.setPosition(100, 20);
			doubleDamagePowerup.setSpeed(0);
			ddPowerUp = false;
		}

		if (speedIncreasePowerup.getPosition().intersects(playerBat.getPosition()))
		{
			ball.setSpeed(ball.getSpeed() + (ball.getSpeed() * 0.1));
			speedIncreasePowerup.setPosition(100, 20);
			speedIncreasePowerup.setSpeed(0);
			siPowerup = false;
		}
		else if (speedIncreasePowerup.getYCord() > 1080)
		{
			speedIncreasePowerup.setPosition(100, 20);
			speedIncreasePowerup.setSpeed(0);
			siPowerup = false;
		}

		if (freezeEnemyPowerup.getPosition().intersects(playerBat.getPosition()))
		{
			frozen = true;
			freezeEnemyPowerup.setPosition(100, 20);
			freezeEnemyPowerup.setSpeed(0);
			fePowerup = false;
		}
		else if (freezeEnemyPowerup.getYCord() > 1080)
		{
			freezeEnemyPowerup.setPosition(100, 20);
			freezeEnemyPowerup.setSpeed(0);
			fePowerup = false;
		}

		if (!paused)
		{
			for (int i = 0; i < sizeof(boxesArray) / sizeof(boxesArray[0]); i++)
			{
				if (boxesArray[i].getHealth() <= 0)
				{
					deadBoxes++;
				}
			}

			if (deadBoxes != 36)
			{
				deadBoxes = 0;
			}
			else if (deadBoxes == 36)
			{
				paused = true;
				string cont = "You Won!!";
				controls.setString(cont);
			}
		}

		/*
		*********************************************************************
		Drawing all the visual elements of the game
		*********************************************************************
		*/

		window.clear();
		if (!paused)
		{
			window.draw(playerBat.getShape());
			window.draw(enemyBat.getShape());
			window.draw(ball.getShape());
			// Draw every box with a health value greater than 0
			for (int i = 0; i < sizeof(boxesArray) / sizeof(boxesArray[0]); i++)
			{
				if (boxesArray[i].getHealth() > 0)
				{
					window.draw(boxesArray[i].getShape());
				}
			}
			window.draw(border);
			window.draw(hud);
		}
		if (ddPowerUp)
		{
			window.draw(doubleDamagePowerup.getShape());
		}
		if (siPowerup)
		{
			window.draw(speedIncreasePowerup.getShape());
		}
		if (fePowerup)
		{
			window.draw(freezeEnemyPowerup.getShape());
		}
		if (paused)
		{
			window.draw(controls);
		}
		window.display();
	}
	return 0;
}