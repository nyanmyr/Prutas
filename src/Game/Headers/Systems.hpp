#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

#include <SFML/Graphics.hpp>
#include "../../Engine/NacreCoordinator.hpp"
#include "Components.hpp"

namespace Start
{
	void setText(sf::Font& font);
	void setTextOrigin();
	void setSpriteOrigin();
	void loadTextures(Entity loadedTextures);
	void loadSprites(Entity loadedTextures);
	void setColor();
}

namespace Control
{
	void buttonClicks
	(
		const sf::Vector2i mouseVector,
		const DeltaTime dt
	);
	void doPlayerControl
	(
		const Entity player,
		const DeltaTime dt
	);
	void pickup(const Entity player);
}

namespace Update
{
	void doButtons
	(
		const sf::Vector2i mouseVector,
		const DeltaTime dt
	);
	void doNextScene(sf::RenderWindow& window, sf::Font& font);
	void move(const DeltaTime dt);
	void drag(const DeltaTime dt);
	void doBounds
	(
		const double top,
		const double bottom,
		const double left,
		const double right
	);
	void followCamera
	(
		Entity camera,
		DeltaTime dt,
		sf::RenderWindow& window
	);
}

namespace Render
{
	void doZIndex(std::queue<Entity>& renderQueue);
	void render
	(
		sf::RenderWindow& window,
		std::queue<Entity>& renderQueue
	);
}

#endif
