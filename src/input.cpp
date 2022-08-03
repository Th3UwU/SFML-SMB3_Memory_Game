#include "input.hpp"
#include "game.hpp"

Input::Input(void)
{
	// Joystick
	joystickType = JOYSTICK_TYPE::NONE;
	if (sf::Joystick::isConnected(0))
	{
		unsigned int buttonCount = sf::Joystick::getButtonCount(0);

		if (buttonCount == 10)
			joystickType = JOYSTICK_TYPE::XBOX;
		else if (buttonCount == 14)
			joystickType = JOYSTICK_TYPE::PS4;
	}
}

void Input::Update(void)
{
	//sf::Vector2f worldPos = game.render.mapPixelToCoords(sf::Mouse::getPosition(game.window));
	//MOUSE_POS = sf::Vector2i((worldPos.x - game.offsetX) / game.scale, (worldPos.y - game.offsetY) / game.scale);

	// Get mouse position and convert coords relative to render (scale, view, offset)
	MOUSE_POS = sf::Mouse::getPosition(game.window);
	MOUSE_POS.x -= game.offsetX; MOUSE_POS.x /= game.scale;
	MOUSE_POS.y -= game.offsetY; MOUSE_POS.y /= game.scale;

	sf::Vector2f viewPos = game.renderView.getCenter();
	MOUSE_POS.x += viewPos.x - (game.gameWidth / 2);
	MOUSE_POS.y += viewPos.y - (game.gameHeight / 2);

	HOLD[ACTION] = sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
	HOLD[UP] = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
	HOLD[DOWN] = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
	HOLD[RIGHT] = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
	HOLD[LEFT] = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
	HOLD[FULLSCREEN] = sf::Keyboard::isKeyPressed(sf::Keyboard::F11);

	// Joystick

	/** Same for both controller types **/
	HOLD[UP] |= (int(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY)) == 100);
	HOLD[DOWN] |= (int(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY)) == -100);
	HOLD[RIGHT] |= (int(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX)) == 100);
	HOLD[LEFT] |= (int(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX)) == -100);


	if (joystickType == JOYSTICK_TYPE::XBOX)
		HOLD[ACTION] |= sf::Joystick::isButtonPressed(0, 0);
	else if (joystickType == JOYSTICK_TYPE::PS4)
		HOLD[ACTION] |= sf::Joystick::isButtonPressed(0, 1);

	/*
	if (HOLD[UP]) printf("UP\n");
	if (HOLD[LEFT]) printf("LEFT\n");
	if (HOLD[RIGHT]) printf("RIGHT\n");
	if (HOLD[DOWN]) printf("DOWN\n");
	if (HOLD[ACTION]) printf("ACTION\n");
	*/

	for (unsigned int i = 0; i < IN_COUNT; i++)
	{
		// Key pressed
		if ((!HOLD_OLD[i]) and (HOLD[i]))
			PRESSED[i] = true;
		else
			PRESSED[i] = false;

		// Key released
		if ((HOLD_OLD[i]) and (!HOLD[i]))
			RELEASED[i] = true;
		else
			RELEASED[i] = false;
		
		HOLD_OLD[i] = HOLD[i];
	}
}