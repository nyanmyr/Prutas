#ifndef ENTITY_MAKER_HPP
#define ENTITY_MAKER_HPP

#include <SFML/Graphics.hpp>
#include "Scenes.hpp"
#include "Components.hpp"
#include "Enums.hpp"

Entity makePlayer
(
	const Enum::Texture texture,
	const sf::Vector2f pos,
	const sf::Vector2f size,
	const sf::Vector2f minVelocity,
	const sf::Vector2f maxVelocity,
	const sf::Vector2f speed,
	const sf::Vector2f drag,
	const sf::Color col
);
Entity makeButton
(
	const Enum::Texture texture,
	const sf::Vector2f pos,
	const sf::Vector2f size,
	const Scene scene,
	const std::string str,
	const sf::Font& font,
	const sf::Color col
);
Entity makeLoadedTexturesContainer();
Entity makeObject
(
	const Enum::Texture texture,
	const sf::Vector2f pos,
	const sf::Vector2f size,
	const sf::Color col
);

Entity makeItem
(
	const Enum::Texture texture,
	const sf::Vector2f pos,
	const sf::Vector2f size,
	const sf::Color col,
	const Enum::Item type,
	const double pickupDistance
);

Entity makeCamera
(
	const double min,
	const double max,
	const Entity target
);

Entity makeForageSpot
(
	const Enum::Texture texture,
	const sf::Vector2f pos,
	const sf::Vector2f size,
	const sf::Color col,
	const double forageDistance,
	const std::unordered_map<Enum::Item, int>& itemTable
);

// passing an vector argument with unequal sizes will throw an error
// while passing vector arguments with exceeding sizes, the exceeding elements will be ignored
Entity makePlant
(
	const sf::Vector2f pos,
	const std::vector<double> plantTimes,
	const std::vector<sf::Color> plantColors,
	const std::vector<sf::Vector2f> plantSizes,
	const std::vector<Enum::Texture> plantTextures,
	const std::unordered_map<Enum::Item, sf::Vector2i>& dropTable,
	const double harvestDistance,
	const double springGrowthRate,
	const double summerGrowthRate,
	const double fallGrowthRate,
	const double winterGrowthRate,
	const double plantHeathCurrent,
	const double plantHeathMax
);

Entity makeSeller
(
	const Enum::Texture texture,
	const sf::Vector2f pos,
	const sf::Vector2f size,
	const sf::Color col,
	const double sellDistance
);

#endif