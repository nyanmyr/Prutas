#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <iostream>

#include "../Engine/NacreCoordinator.hpp"

#include "Headers/EntityMaker.hpp"
#include "Headers/Components.hpp"
#include "Headers/Enums.hpp"

NacreCoordinator& entityMakerNC = NacreCoordinator::getInstance();

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
)
{
	Entity entity = entityMakerNC.createEntity();

	entityMakerNC.addComponent
	(
		entity,
		Component::Position
		{ 
			pos.x,
			pos.y
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::ZIndex
		{
			2,
			true
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::YAxisAdd
		{
			0,
			true
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Origin
		{
			size.x / 2.0,
			size.y / 2.0
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Velocity
		{
			minVelocity.x,
			minVelocity.y,
			maxVelocity.x,
			maxVelocity.y
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Speed
		{
			speed.x,
			speed.y
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Drag
		{
			drag.x,
			drag.y
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::PlayerController
		{
			true
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Transform
		{
			size.x,
			size.y
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Texture{ texture }
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Sprite{}
	);

	entityMakerNC.addComponent
	(
		entity,
		Component::Color{ col }
	);

	entityMakerNC.addComponent
	(
		entity,
		Component::Inventory{}
	);

	return entity;
}

Entity makeButton
(
	const Enum::Texture texture,
	const sf::Vector2f pos,
	const sf::Vector2f size,
	const Scene scene,
	const std::string str,
	const sf::Font& font,
	const sf::Color col
)
{
	Entity entity = entityMakerNC.createEntity();

	entityMakerNC.addComponent(
		entity,
		Component::Position
		{
			pos.x,
			pos.y
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Transform
		{
			size.x,
			size.y
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Origin
		{
			size.x / 2.0,
			size.y / 2.0
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Button
		{
			0.125f,
			true
		}
	);

	sf::Text text(font);
	entityMakerNC.addComponent
	(
		entity,
		Component::Text
		{
			text,
			str,
			64,
			sf::Color::Black,
			Enum::TextFormat::MIDDLE
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::NextScene
		{
			scene,
			false
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::ZIndex
		{
			1,
			true
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Transform
		{
			size.x,
			size.y
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Texture{ texture }
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Sprite{}
	);

	entityMakerNC.addComponent
	(
		entity,
		Component::Color{ col }
	);

	return entity;
}

Entity makeLoadedTexturesContainer()
{
	Entity entity = entityMakerNC.createEntity();

	entityMakerNC.addComponent
	(
		entity,
		Component::TexturesContainer{}
	);

	return entity;
}

Entity& makeObject
(
	const Enum::Texture texture,
	const sf::Vector2f pos,
	const sf::Vector2f size,
	const sf::Color col
)
{
	Entity entity = entityMakerNC.createEntity();

	entityMakerNC.addComponent
	(
		entity,
		Component::Position
		{
			pos.x,
			pos.y
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Transform
		{
			size.x,
			size.y
		}
	);

	entityMakerNC.addComponent
	(
		entity,
		Component::Texture{ texture }
	);

	entityMakerNC.addComponent
	(
		entity,
		Component::Sprite{}
	);

	entityMakerNC.addComponent
	(
		entity,
		Component::ZIndex
		{
			1,
			true
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Origin
		{
			size.x / 2.f,
			size.y / 2.f
		}
	);

	entityMakerNC.addComponent
	(
		entity,
		Component::Color{ col }
	);

	return entity;
}

Entity& makeItem
(
	const Enum::Texture texture,
	const sf::Vector2f pos,
	const sf::Vector2f size,
	const sf::Color col,
	const Enum::Item type,
	const double pickupDistance
)
{
	Entity entity = entityMakerNC.createEntity();

	entityMakerNC.addComponent
	(
		entity,
		Component::Position
		{
			pos.x,
			pos.y
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Transform
		{
			size.x,
			size.y
		}
	);

	entityMakerNC.addComponent
	(
		entity,
		Component::Texture{ texture }
	);

	entityMakerNC.addComponent
	(
		entity,
		Component::Sprite{}
	);

	entityMakerNC.addComponent
	(
		entity,
		Component::ZIndex
		{
			1,
			true
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::YAxisAdd
		{
			0,
			true
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Origin
		{
			size.x / 2.f,
			size.y / 2.f
		}
	);

	entityMakerNC.addComponent
	(
		entity,
		Component::Color{ col }
	);

	entityMakerNC.addComponent
	(
		entity,
		Component::Item
		{
			type,
			pickupDistance
		}
	);

	return entity;
}

Entity& makeCamera
(
	const double min,
	const double max,
	const Entity target
)
{
	Entity entity = entityMakerNC.createEntity();

	entityMakerNC.addComponent
	(
		entity,
		Component::Camera
		{
			min,
			max,
			target
		}
	);

	return entity;
}

Entity& makeForageSpot
(
	const Enum::Texture texture,
	const sf::Vector2f pos,
	const sf::Vector2f size,
	const sf::Color col,
	const double forageDistance,
	const std::unordered_map<Enum::Item, int>& itemTable
)
{
	Entity entity = entityMakerNC.createEntity();

	entityMakerNC.addComponent
	(
		entity,
		Component::Position
		{
			pos.x,
			pos.y
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Transform
		{
			size.x,
			size.y
		}
	);

	entityMakerNC.addComponent
	(
		entity,
		Component::Texture{ texture }
	);

	entityMakerNC.addComponent
	(
		entity,
		Component::Sprite{}
	);

	entityMakerNC.addComponent
	(
		entity,
		Component::ZIndex
		{
			1,
			true
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::YAxisAdd
		{
			0,
			true
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Origin
		{
			size.x / 2.f,
			size.y / 2.f
		}
	);

	entityMakerNC.addComponent
	(
		entity,
		Component::Color{ col }
	);

	int max = 0;

	for (const auto& [item, num] : itemTable)
	{
		max += num;
	}

	entityMakerNC.addComponent
	(
		entity,
		Component::ForageSpot
		{
			forageDistance,
			std::move(itemTable)
		}
	);

	return entity;
}