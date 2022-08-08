#include "game.hpp"

#include <ctime>
#include <cstdlib>

Input input;

Game::Game(void)
{
	windowTitle = sf::String("Memory Game");
	window.create(sf::VideoMode(800, 600), windowTitle);
	window.setFramerateLimit(60);
	fullscreen = false;

	gameWidth = 256;
	gameHeight = 240;

	render.create(gameWidth, gameHeight);
	renderView = render.getDefaultView();
	//renderView.move(50, 20);
	render.setView(renderView);
	renderSprite.setTexture(render.getTexture());
	CalcScale();
	
	// Texture
	texture.loadFromFile("data/texture.png");
	
	sprFrame.setTexture(texture);
	sprFrame.setTextureRect(sf::IntRect(226, 28, 28, 40));

	// Audio
	bufferCorrect.loadFromFile("data/correct.wav");
	bufferWrong.loadFromFile("data/wrong.wav");
	bufferMove.loadFromFile("data/move.wav");
	bufferFlip.loadFromFile("data/flip.wav");

	sfxCorrect.setBuffer(bufferCorrect);
	sfxWrong.setBuffer(bufferWrong);
	sfxMove.setBuffer(bufferMove);
	sfxMove.setVolume(20);

	sfxFlip.setBuffer(bufferFlip);

	// Font
	score = 0;

	font.loadFromFile("data/PressStart2P.ttf");

	sf::Texture& txr = (sf::Texture&)font.getTexture(8);
	txr.setSmooth(false);

	txtScore.setFont(font);
	txtScore.setCharacterSize(8);
	txtScore.setScale(3, 3);
	txtScore.setFillColor(sf::Color::White);
	//txtScore.setString(stringScore);

	// Shader
	shaderInvert.loadFromFile("data/shader.frag", sf::Shader::Fragment);
	shaderEnable = false;
	shaderTime = 0;
	shaderBlinkTime = 0;

	// Random seed
	srand(time(nullptr));
	
	// Create cards
	cardSelected = 0;
	card1 = card2 = nullptr;
	card = (Card*)operator new(sizeof(Card) * cardsCount);

	unsigned int type;

	int sepX = 32;
	int x = (gameWidth - (sepX * 6)) / 2;

	int sepY = 40;
	int y = (gameHeight - (sepY * 3)) / 2;

	for (int i = 0; i < cardsCount; i += 2)
	{
		type = (rand() % 6) + 1;

		new(&card[i]) Card( ((i % cardsPerRow) * sepX) + x, ((i / cardsPerRow) * sepY) + y, type );
		new(&card[i + 1]) Card( (((i + 1) % cardsPerRow) * sepX) + x, (((i + 1)/cardsPerRow) * sepY) + y, type );
	}

	//* swap cards world position *//
	sf::Vector2f tempPos;
	sf::IntRect tempRect;
	unsigned int index;

	for (int i = 0; i < cardsCount; i++)
	{
		index = rand() % cardsCount;

		unsigned int tempType;
		tempType = card[i].type;
		card[i].type = card[index].type;
		card[index].type = tempType;
	}
}

Game::~Game(void)
{
	for (int i = 0; i < cardsCount; i++)
		card[i].~Card();
	operator delete(card);
}

void Game::Loop(void)
{
	while (window.isOpen())
	{	
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::Resized)
			{
				window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
				CalcScale();
			}

			if (event.type == sf::Event::JoystickMoved)
			{
				/*
				printf("AXIS: %u\n", event.joystickMove.axis);
				printf("AXIS POS: %.0f\n", event.joystickMove.position);
				*/
			}
		}
		
		input.Update();

		if (input.PRESSED[input.FULLSCREEN])
		{
			if (fullscreen)
				window.create(sf::VideoMode(gameWidth, gameHeight), windowTitle, sf::Style::Default);
			else
				window.create(sf::VideoMode::getFullscreenModes()[0], windowTitle, sf::Style::Fullscreen);

			CalcScale();
			fullscreen = !fullscreen;	
		}

		// Update
		render.setView(renderView);
		Update();

		// Draw
		render.clear(sf::Color(0, 100, 100, 255));
		Draw();
		render.display();

		window.clear(sf::Color(0, 0, 0, 255));
		window.draw(renderSprite, &shaderInvert);
		window.display();
	}
}

void Game::Update(void)
{
	// Card update / selection
	for (int i = 0; i < cardsCount; i++)
	{
		card[i].Update();

		if (card[i].rect.contains(input.MOUSE_POS))
		{
			/*
			if (cardSelected != i)
				sfxMove.play();
			*/

			cardSelected = i;
		}
	}

	if ((input.PRESSED[input.RIGHT]) and (cardSelected < cardsCount - 1))
		{cardSelected += 1; sfxMove.play();}
	else if ((input.PRESSED[input.LEFT]) and (cardSelected > 0))
		{cardSelected -= 1; sfxMove.play();}

	if ((input.PRESSED[input.UP]) and (cardSelected - cardsPerRow >= 0))
		{cardSelected -= cardsPerRow; sfxMove.play();}
	else if ((input.PRESSED[input.DOWN]) and (cardSelected + cardsPerRow <= cardsCount - 1))
		{cardSelected += cardsPerRow; sfxMove.play();}

	// Card clicked
	if (input.PRESSED[input.ACTION])
	if ((!card[cardSelected].flipped) and (!card[cardSelected].clicked))
	{
		if (!card1)
		{
			card1 = &card[cardSelected];
			card[cardSelected].clicked = true;

			sfxFlip.play();
		}
		else if (!card2)
		{
			card2 = &card[cardSelected];
			card[cardSelected].clicked = true;

			sfxFlip.play();
		}
	}

	sprintf(stringScore, "SCORE: %u", score);
	txtScore.setString(stringScore);

	//renderView.move(1, 0);

	// Shader blink
	if (shaderTime > 0)
	{
		if (shaderBlinkTime == 0)
		{
			shaderBlinkTime = 5;
			shaderEnable = !shaderEnable;
		}
		else
			shaderBlinkTime--;

		shaderTime--;
	}
	else
	{
		shaderEnable = false;
		shaderBlinkTime = 0;
	}

	shaderInvert.setUniform("enable", shaderEnable);
}

void Game::Draw(void)
{
	for (int i = 0; i < cardsCount; i++)
		render.draw(card[i]);

	// Card frame
	sf::Vector2f pos = card[cardSelected].spr.getPosition();
	pos.x += 2;
	pos.y += -4;

	sprFrame.setPosition(pos);
	render.draw(sprFrame);

	render.draw(txtScore);
}

void Game::CalcScale(void)
{
	sf::Vector2u size = window.getSize();

	unsigned int scaleX = size.x / gameWidth;
	unsigned int scaleY = size.y / gameHeight;
	scale = (scaleX <= scaleY) ? scaleX : scaleY;

	if (scale == 0)
	{
		scale = 1;
		offsetX = 0;
		offsetY = 0;

		window.setSize(sf::Vector2u(gameWidth, gameHeight));
		renderSprite.setScale(scale, scale);

		return;
	}
	else
		renderSprite.setScale(scale, scale);

	offsetX = (size.x - (scale * gameWidth)) / 2;
	offsetY = (size.y - (scale * gameHeight)) / 2;
	renderSprite.setPosition(offsetX, offsetY);
}
