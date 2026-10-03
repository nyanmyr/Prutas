#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <SFML/Graphics.hpp>

namespace Event
{
	struct ButtonReleased
	{
		sf::Keyboard::Scancode key;
	};
}

#endif