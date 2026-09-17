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
Entity& makeObject
(
	const Enum::Texture texture,
	const sf::Vector2f pos,
	const sf::Vector2f size,
	const sf::Color col
);

Entity& makeItem
(
	const Enum::Texture texture,
	const sf::Vector2f pos,
	const sf::Vector2f size,
	const sf::Color col,
	const Enum::Item type,
	const double pickupDistance
);

Entity& makeCamera
(
	const double min,
	const double max,
	const Entity target
);

Entity& makeForageSpot
(
	const Enum::Texture texture,
	const sf::Vector2f pos,
	const sf::Vector2f size,
	const sf::Color col,
	const double forageDistance,
	// pass in int values because this func also handles conversion of those values to probability percentages
	const std::unordered_map<Enum::Item, int>& itemTable
);
#endif