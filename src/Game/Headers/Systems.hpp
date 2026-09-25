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
	void loadTextures(const Entity loadedTextures);
	void loadSprites(const Entity loadedTextures);
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
	void forage(const Entity player);
	void harvest(const Entity player);
	void openInventory(const Entity player);
	void inventorySelectLeft(const Entity player);
	void inventorySelectRight(const Entity player);
	void plant(const Entity player);
	void sellAllItems(const Entity player);
}

namespace Update
{
	void timeCycle
	(
		const DeltaTime dt,
		Component::TimeCycle& timeCycle,
		Component::TimePassed& timePassed,
		Component::TimeDefaults& timeDefaults,
		Component::Season& season
	);
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
		const Entity camera,
		const DeltaTime dt,
		sf::RenderWindow& window
	);
	void grow
	(
		const Entity loadedTextures,
		const DeltaTime dt,
		const Component::Season& season
	);
	void handlePlantHealth
	(
		const DeltaTime dt,
		const Component::Season& season,
		const Component::TimeDefaults& timeDefaults
	);
	void deleteEntities(DeltaTime dt);
}

namespace Render
{
	void alignZIndexWithYAxis
	(
		const double top,
		const double bottom,
		const Entity player
	);
	void doZIndex(std::queue<Entity>& renderQueue);
	void render
	(
		sf::RenderWindow& window,
		std::queue<Entity>& renderQueue
	);
}

#endif
