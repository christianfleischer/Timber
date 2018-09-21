// Include C++ libraries here
#include "pch.h"
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// test2


// namespace here
using namespace sf;

// Function declaration
void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
// Where is the player/branch?
// Left or Right
enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];

int main()
{
    // Create video mode object
	VideoMode vm(1920, 1080);

	// Create and open the game window
	RenderWindow window(vm, "Timber", Style::Fullscreen);

	// Create a texture to hold a graphic on the GPU
	Texture textureBackground;

	// Load a graphic into the texture
	textureBackground.loadFromFile("graphics/background.png");

	// Create a sprite
	Sprite spriteBackground;

	// Attach the texturre to the sprite
	spriteBackground.setTexture(textureBackground);

	// Set the spriteBackground to cover the screen
	spriteBackground.setPosition(0, 0);

	// Make a tree sprite
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	// Enhanced version
	Texture textureTree2;
	textureTree2.loadFromFile("graphics/tree2.png");
	Sprite spriteTree2;
	spriteTree2.setTexture(textureTree2);
	Sprite spriteTree3;
	spriteTree3.setTexture(textureTree2);
	Sprite spriteTree4;
	spriteTree4.setTexture(textureTree2);
	Sprite spriteTree5;
	spriteTree5.setTexture(textureTree2);
	Sprite spriteTree6;
	spriteTree6.setTexture(textureTree2);


	spriteTree2.setPosition(20, 0);
	spriteTree3.setPosition(300, -400);
	spriteTree4.setPosition(1300, -400);
	spriteTree5.setPosition(1500, -500);
	spriteTree6.setPosition(1900, 0);

	// Prepare the bee
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(2000, 800);
	// Is the bee currently moving?
	bool beeActive = false;
	// How fast can the bee fly?
	float beeSpeed = 0.0f;

	// Make 3 cloud sprites from 1 texture
	Texture textureCloud;
	// Load 1 new texture
	textureCloud.loadFromFile("graphics/cloud.png");

	// Make the clouds with arrays
	const int NUM_CLOUDS = 6;
	Sprite clouds[NUM_CLOUDS];
	int cloudSpeeds[NUM_CLOUDS];
	bool cloudsActive[NUM_CLOUDS];

	for (int i = 0; i < NUM_CLOUDS; i++)
	{
		clouds[i].setTexture(textureCloud);
		clouds[i].setPosition(-300, i * 150);
		cloudsActive[i] = false;
		cloudSpeeds[i] = 0;
	}

	// 3 New sprites with the same texture
	//sprite spritecloud1;
	//sprite spritecloud2;
	//sprite spritecloud3;
	//spritecloud1.settexture(texturecloud);
	//spritecloud2.settexture(texturecloud);
	//spritecloud3.settexture(texturecloud);
	//// position the clouds off screen
	//spritecloud1.setposition(2000, 0);
	//spritecloud2.setposition(2000, 250);
	//spritecloud3.setposition(2000, 500);
	//// are the clouds currently on screen?
	//bool cloud1active = false;
	//bool cloud2active = false;
	//bool cloud3active = false;
	//// how fast is each cloud?
	//float cloud1speed = 0.0f;
	//float cloud2speed = 0.0f;
	//float cloud3speed = 0.0f;

	// Variables to control time
	Clock clock;

	// Time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	FloatRect timeBarRect = timeBar.getLocalBounds();
	timeBar.setOrigin(timeBarRect.left +
		timeBarRect.width / 2.0f,
		timeBarRect.top +
		timeBarRect.height / 2.0f);
	timeBar.setPosition((1920 / 2), 980);
	//timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 5.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	// Track whether the game is running
	bool paused = true;

	// Track whether the player is squished
	bool squished = true;

	// Draw some text
	int score = 0;
	sf::Text messageText;
	sf::Text scoreText;
	sf::Text fpsText;

	// Choose a font
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	// Set the font to our message
	messageText.setFont(font);
	scoreText.setFont(font);
	fpsText.setFont(font);

	// Set up the fps text
	fpsText.setFillColor(Color::White);
	fpsText.setCharacterSize(100);
	fpsText.setPosition(1200, 20);

	// Assign the actual message
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");

	// Make it really big
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	// Choose a color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	// Position the text
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left + 
		textRect.width / 2.0f,
		textRect.top + 
		textRect.height / 2.0f);
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreText.setPosition(20, 20);

	// Backgrounds for the text
	RectangleShape rect1;
	rect1.setFillColor(sf::Color(0, 0, 0, 150));
	rect1.setSize(Vector2f(600, 105));
	rect1.setPosition(0, 30);

	RectangleShape rect2;
	rect2.setFillColor(sf::Color(0, 0, 0, 150));
	rect2.setSize(Vector2f(1000, 105));
	rect2.setPosition(1150, 30);

	// Prepare 6 branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	// Set the texture for each branch sprite
	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		// Set the sprite's origin to dead center
		// We can then spin it round without changing its position
		branches[i].setOrigin(220, 20);
	}

	// Prepare the player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(655, 816);
	spritePlayer.setOrigin(75, 96);
	spritePlayer.setTextureRect(IntRect(150, 0, -150, 192));

	// The player starts on the left
	side playerSide = side::LEFT;

	// Prepare the gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(675, 2000);

	// Prepare the axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(776, 846);
	spriteAxe.setOrigin(76, 16);
	spriteAxe.setTextureRect(IntRect(152, 0, -152, 32));

	// Line the axe up with the tree
	const float AXE_POSITION_LEFT = 776;
	const float AXE_POSITION_RIGHT = 1151;

	// Prepare the flying log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	// Some other useful log related variables
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	// Control the player input
	bool acceptInput = false;
	bool acceptPause = true;

	// Prepare the sound
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	// Out of time
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);

	// Control the drawing of the score
	int lastDrawn = 0;

	while (window.isOpen())
	{
		/*
		***************************************
		Handle player input
		***************************************
		*/

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased)
			{
				// Listen for key presses again
				if (!paused)
				{
					acceptInput = true;
				}				
				acceptPause = true;

				// Hide the axe
				spriteAxe.setPosition(2000,
					spriteAxe.getPosition().y);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// Start the game
		if (Keyboard::isKeyPressed(Keyboard::Return) && paused && acceptPause)
		{
			paused = false;
			acceptPause = false;

			if (timeRemaining <= 0 || squished)
			{
				// Reset the time, the score and the branches
				score = 0;
				timeRemaining = 5;
				squished = false;

				// Make all the branches disappear
				for (int i = 1; i < NUM_BRANCHES; i++)
				{
					branchPositions[i] = side::NONE;
				}

				// Make sure the gravstone is hidden
				spriteRIP.setPosition(675, 2000);

				// Move the player into position
				spritePlayer.setPosition(655, 816);
				acceptInput = true;
			}
			
			//sf::sleep(sf::milliseconds(300));
			clock.restart();
		}

		// Wrap the player controls to
		// Make sure we are accepting input
		if (acceptInput)
		{
			// First handle pressing the right cursor key
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				// Make sure the player is on the right
				playerSide = side::RIGHT;
				score++;

				// Add to the amount of time remaining
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_RIGHT,
					spriteAxe.getPosition().y);
				spriteAxe.setTextureRect(IntRect(0, 0, 152, 32));

				spritePlayer.setPosition(1275, 816);
				spritePlayer.setTextureRect(IntRect(0, 0, 150, 192));

				// Update the braches
				updateBranches(score);

				// Set the log flying to the left
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;

				// Play a chop sound
				chop.play();
			}

			// Handle the left cursor key
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				// Make sure the player is on the left
				playerSide = side::LEFT;
				score++;

				// Add to the amount of time remaining
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_LEFT,
					spriteAxe.getPosition().y);
				spriteAxe.setTextureRect(IntRect(152, 0, -152, 32));

				spritePlayer.setPosition(655, 816);
				spritePlayer.setTextureRect(IntRect(150, 0, -150, 192));

				// Update the branches
				updateBranches(score);

				// Set the log flying
				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;

				// Play a chop sound
				chop.play();
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Return) && !paused && acceptPause)
		{
			paused = true;
			acceptInput = false;
			acceptPause = false;

			// Change the message shown to the player
			messageText.setString("Paused");

			// Reposition the text based on its new size
			FloatRect textRect = messageText.getLocalBounds();
			messageText.setOrigin(textRect.left +
				textRect.width / 2.0f,
				textRect.top +
				textRect.height / 2.0f);
			messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

			//sf::sleep(sf::milliseconds(300));
			clock.restart();
		}

		/*
		***************************************
		Update the scene
		***************************************
		*/

		if (!paused)
		{
			// Measure time
			Time dt = clock.restart();

			// Subtract from the amount of time remaining
			timeRemaining -= dt.asSeconds();
			// Size up the time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * 
				timeRemaining, timeBarHeight));
			FloatRect timeBarRect = timeBar.getLocalBounds();
			timeBar.setOrigin(timeBarRect.left +
				timeBarRect.width / 2.0f,
				timeBarRect.top +
				timeBarRect.height / 2.0f);
			timeBar.setPosition((1920 / 2), 980);

			if (timeRemaining <= 0.0f)
			{
				// Pause the game
				paused = true;

				// Change the message shown to the player
				messageText.setString("Out of time!");

				// Reposition the text based on its new size
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top +
					textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				// Play the out of time sound
				outOfTime.play();
			}

			// Setup the bee
			if (!beeActive)
			{
				// How fast is the bee
				srand((int)time(0) * 10);
				beeSpeed = (rand() % 200) + 200;

				// How high is the bee
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);
				beeActive = true;
			}

			else
			{
				// Move the bee
				spriteBee.setPosition(
					spriteBee.getPosition().x -
					(beeSpeed * dt.asSeconds()),
					spriteBee.getPosition().y);

				// Has the bee reached the right hand edge of the screen?
				if (spriteBee.getPosition().x < -100)
				{
					// Set it up ready to be a whole new bee next frame
					beeActive = false;
				}
			}

			// Manage the clouds
			for (int i = 0; i < NUM_CLOUDS; i++)
			{
				if (!cloudsActive[i])
				{
					// How fast is the cloud
					srand((int)time(0) * i);
					cloudSpeeds[i] = (rand() % 200);

					// How high is the clouds
					srand((int)time(0) * i);
					float height = (rand() % 150);
					clouds[i].setPosition(-200, height);
					cloudsActive[i] = true;
				}

				else
				{
					// Set the new position
					clouds[i].setPosition(
						clouds[i].getPosition().x +
						(cloudSpeeds[i] * dt.asSeconds()),
						clouds[i].getPosition().y);

					// Has the cloud reached the right hand edge of the screen?
					if (clouds[i].getPosition().x > 1920)
					{
						// Set it up ready to be a whole new cloud next frame
						cloudsActive[i] = false;
					}
				}
			}


			//// Cloud 1
			//if (!cloud1Active)
			//{
			//	// How fast is the cloud
			//	srand((int)time(0) * 10);
			//	cloud1Speed = (rand() % 200);

			//	// How high is the cloud
			//	srand((int)time(0) * 10);
			//	float height = (rand() % 150);
			//	spriteCloud1.setPosition(-200, height);
			//	cloud1Active = true;
			//}

			//else
			//{
			//	spriteCloud1.setPosition(
			//		spriteCloud1.getPosition().x +
			//		(cloud1Speed * dt.asSeconds()),
			//		spriteCloud1.getPosition().y);

			//	// Has the cloud reached the right hand edge of the screen?
			//	if (spriteCloud1.getPosition().x > 1920)
			//	{
			//		// Set it up ready to be a whole new cloud next frame
			//		cloud1Active = false;
			//	}
			//}

			//// Cloud 2
			//if (!cloud2Active)
			//{
			//	// How fast is the cloud
			//	srand((int)time(0) * 20);
			//	cloud2Speed = (rand() % 200);

			//	// How high is the cloud
			//	srand((int)time(0) * 20);
			//	float height = (rand() % 300) - 150;
			//	spriteCloud2.setPosition(-200, height);
			//	cloud2Active = true;
			//}

			//else
			//{
			//	spriteCloud2.setPosition(
			//		spriteCloud2.getPosition().x +
			//		(cloud2Speed * dt.asSeconds()),
			//		spriteCloud2.getPosition().y);

			//	// Has the cloud reached the right hand edge of the screen?
			//	if (spriteCloud2.getPosition().x > 1920)
			//	{
			//		// Set it up ready to be a whole new cloud next frame
			//		cloud2Active = false;
			//	}
			//}

			//// Cloud 3
			//if (!cloud3Active)
			//{
			//	// How fast is the cloud
			//	srand((int)time(0) * 30);
			//	cloud3Speed = (rand() % 200);

			//	// How high is the cloud
			//	srand((int)time(0) * 30);
			//	float height = (rand() % 450) - 150;
			//	spriteCloud3.setPosition(-200, height);
			//	cloud3Active = true;
			//}

			//else
			//{
			//	spriteCloud3.setPosition(
			//		spriteCloud3.getPosition().x +
			//		(cloud3Speed * dt.asSeconds()),
			//		spriteCloud3.getPosition().y);

			//	// Has the cloud reached the right hand edge of the screen?
			//	if (spriteCloud3.getPosition().x > 1920)
			//	{
			//		// Set it up ready to be a whole new cloud next frame
			//		cloud3Active = false;
			//	}
			//}

			// Draw the score and the frame rate once every 100 frames
			lastDrawn++;
			if (lastDrawn == 100)
			{
				// Update the score text
				std::stringstream ss;
				ss << "Score = " << score;
				scoreText.setString(ss.str());

				// Draw the fps
				std::stringstream ss2;
				ss2 << "FPS = " << 1 / dt.asSeconds();
				fpsText.setString(ss2.str());

				lastDrawn = 0;
			}

			// Update the branch sprites
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;
				if (branchPositions[i] == side::LEFT)
				{
					// Move the sprite to the left side
					branches[i].setPosition(610, height);
					// Flip the sprite round the other way
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					// Move the sprite to the right side
					branches[i].setPosition(1330, height);
					// Set the sprite rotation to normal
					branches[i].setRotation(0);
				}
				else
				{
					// Hide the branch
					branches[i].setPosition(3000, height);
				}
			}

			// Handle a flying log
			if (logActive)
			{
				spriteLog.setPosition(
					spriteLog.getPosition().x +
					(logSpeedX * dt.asSeconds()),
					spriteLog.getPosition().y +
					(logSpeedY * dt.asSeconds()));

				// Has the log reached the edge?
				if (spriteLog.getPosition().x < -100 ||
					spriteLog.getPosition().x > 2000)
				{
					// Set it up ready to be a whole new log next frame
					logActive = false;
					spriteLog.setPosition(810, 720);
				}
			}

			// Has the player been squished by a branch?
			if (branchPositions[5] == playerSide)
			{
				// Death
				paused = true;
				acceptInput = false;
				squished = true;

				// Draw the gravestone
				spriteRIP.setPosition(525, 760);

				// Hide the player
				spritePlayer.setPosition(2000, 660);

				// Change the text of the message
				messageText.setString("SQUISHED");

				// Center it on the screen
				FloatRect textRect = messageText.getLocalBounds();

				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f,
					1080 / 2.0f);

				// Play the death sound
				death.play();
			}

		} // End if(!paused)
		else
		{
			clock.restart();
		}
		

		/*
		***************************************
		Draw the scene
		***************************************
		*/

		// Clear everything from the last frame
		window.clear();

		// Draw the game scene here
		window.draw(spriteBackground);

		// Draw the clouds
		//window.draw(spriteCloud1);
		//window.draw(spriteCloud2);
		//window.draw(spriteCloud3);
		for (int i = 0; i < NUM_CLOUDS; i++)
		{
			window.draw(clouds[i]);
		}

		// Enhanced version
		window.draw(spriteTree2);
		window.draw(spriteTree3);
		window.draw(spriteTree4);
		window.draw(spriteTree5);
		window.draw(spriteTree6);

		// Draw the branches
		for (int i = 0; i < NUM_BRANCHES; i++)
		{
			window.draw(branches[i]);
		}

		// Draw the tree
		window.draw(spriteTree);

		// Draw the player
		window.draw(spritePlayer);

		// Draw the axe
		window.draw(spriteAxe);

		// Draw the flying log
		window.draw(spriteLog);

		// Draw the gravestone
		window.draw(spriteRIP);

		// Draw backgrounds for the text
		window.draw(rect1);
		window.draw(rect2);

		// Draw the bee
		window.draw(spriteBee);

		// Draw the score
		window.draw(scoreText);

		// Draw the FPS
		window.draw(fpsText);

		// Draw the time bar
		window.draw(timeBar);

		if (paused)
		{
			// Draw our message
			window.draw(messageText);
		}

		// Show everything we just drew
		window.display();

	}

	return 0;
}

// Function definition
void updateBranches(int seed)
{
	// Move all the branches down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		branchPositions[j] = branchPositions[j - 1];
	}
	// Spawn a new branch at position 0
	// LEFT, RIGHT or NONE
	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r)
	{
	case 0:
		branchPositions[0] = side::LEFT;
		break;

	case 1:
		branchPositions[0] = side::RIGHT;
		break;

	default:
		branchPositions[0] = side::NONE;
		break;
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
