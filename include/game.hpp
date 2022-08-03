#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "card.hpp"
#include "input.hpp"

class Game
{
public:
	Game(void);
	~Game(void);

	void Loop(void);

	void Update(void);
	void Draw(void);

	void CalcScale(void);

	sf::RenderWindow window;
	bool fullscreen;
	sf::String windowTitle;
	sf::Event event;

	sf::RenderTexture render;
	sf::View renderView;
	sf::Sprite renderSprite;

	sf::Texture texture;

	sf::Sprite sprFrame;

	sf::SoundBuffer bufferCorrect;
	sf::SoundBuffer bufferWrong;
	sf::SoundBuffer bufferMove;
	sf::SoundBuffer bufferFlip;

	sf::Sound sfxCorrect;
	sf::Sound sfxWrong;
	sf::Sound sfxMove;
	sf::Sound sfxFlip;
	
	sf::Font font;
	sf::Text txtScore;
	char stringScore[32];
	unsigned int score;

	sf::Shader shaderInvert;
	bool shaderEnable;
	unsigned int shaderTime;
	unsigned int shaderBlinkTime;

	int gameWidth, gameHeight;
	int scale;
	int offsetX, offsetY;

	int cardSelected;

	Card *card1, *card2;
	Card* card;

private:
	static inline int cardsCount = 18;
	static inline int cardsPerRow = 6;
};

extern Game game;

#endif