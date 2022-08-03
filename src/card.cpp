#include "card.hpp"
#include "game.hpp"

#include <cstdio>

//unsigned int Card::animationSpeed = 4;

Card::Card(int x, int y, int type)
{
	spr.setTexture(game.texture);
	spr.setPosition(x, y);
	spr.setTextureRect(sf::IntRect(0, 0, 32, 32));

	rect = sf::IntRect(x + 5, y, 22, 32);

	this->type = type;

	clicked = false;
	flipped = false;
	frame = 0;
	time = animationSpeed;
}

void Card::Update(void)
{
	if ((clicked) and (!flipped))
	{
		if (time > 0)
			time--;
		else
		{
			time = animationSpeed;
			frame++;
			spr.setTextureRect(sf::IntRect(frame * 32, 0, 32, 32));
		}

		if (frame > 4)
		{
			flipped = true;
			time = animationSpeed;

			spr.setTextureRect(sf::IntRect(type * 32, 32, 32, 32));

			if ((game.card1 and game.card2) and (game.card2 == this))
			{
				if (game.card1->type == this->type)
				{
					game.card1 = nullptr;
					game.card2 = nullptr;

					game.score++;
					game.sfxCorrect.play();
					game.shaderTime = 60;
				}
				else
				{
					game.card1->time *= 8;
					game.card2->time *= 8;

					game.card1->clicked = false;
					game.card2->clicked = false;

					game.sfxWrong.play();
				}
			}
		}
	}
	else if ((!clicked) and (flipped))
	{
		if (time > 0)
			time--;
		else
		{
			time = animationSpeed;
			frame--;
			spr.setTextureRect(sf::IntRect(frame * 32, 0, 32, 32));
		}

		if (frame == 0)
		{
			flipped = false;
			time = animationSpeed;

			spr.setTextureRect(sf::IntRect(0, 0, 32, 32));

			if (game.card1 == this)
				game.card1 = nullptr;
			else
				game.card2 = nullptr;
		}
	}
}

void Card::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(spr, states);
}