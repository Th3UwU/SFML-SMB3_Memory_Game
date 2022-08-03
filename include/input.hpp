#ifndef INPUT_HPP
#define INPUT_HPP

#include <SFML/System.hpp>

enum class JOYSTICK_TYPE : unsigned char
{
	NONE = 0,
	XBOX,
	PS4
};

class Input
{
private:
	static const unsigned int IN_COUNT = 6;

public:
	Input(void);
	void Update(void);

	sf::Vector2i MOUSE_POS;
	
	enum INPUT : unsigned int
	{
		ACTION = 0,
		UP,
		DOWN,
		RIGHT,
		LEFT,
		FULLSCREEN
	};

	bool HOLD_OLD[IN_COUNT];
	
	bool HOLD[IN_COUNT];
	bool PRESSED[IN_COUNT];
	bool RELEASED[IN_COUNT];

	JOYSTICK_TYPE joystickType;
};

extern Input input;

#endif
