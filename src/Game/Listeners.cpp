#include "Headers/Listeners.hpp"
#include <iostream>

namespace Listener
{
	void test(Event::ButtonReleased event)
	{
		std::cout << "test" << "\n";
	}
}