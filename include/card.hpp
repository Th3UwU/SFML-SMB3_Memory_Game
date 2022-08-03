#ifndef CARD_HPP
#define CARD_HPP

#include <SFML/Graphics.hpp>

class Card : public sf::Drawable
{
public:
	Card(int x, int y, int type);
	~Card(void) = default;

	void Update(void);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Sprite spr;
	sf::IntRect rect;

	unsigned int type;

	bool clicked;
	bool flipped;
	unsigned int time;
	unsigned int frame;

private:
	static inline unsigned int animationSpeed = 4;
};

#endif
