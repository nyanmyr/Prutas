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

Entity makeObject
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

Entity makeItem
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

Entity makeCamera
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

Entity makeForageSpot
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

	std::vector<Enum::Item> items{};
	std::vector<int> probabilityWeights{};

	for (auto& [item, probabilityWeight] : itemTable)
	{
		items.push_back(item);
		probabilityWeights.push_back(probabilityWeight);
	}

	entityMakerNC.addComponent
	(
		entity,
		Component::ForageSpot
		{
			forageDistance,
			std::move(items),
			std::move(probabilityWeights)
		}
	);

	return entity;
}

Entity makePlant
(
	const sf::Vector2f pos,
	const std::vector<double> plantTimes,
	const std::vector<sf::Color> plantColors,
	const std::vector<sf::Vector2f> plantSizes,
	const std::vector<Enum::Texture> plantTextures
)
{
	// 4 corresponds to the plant stages
	if ((plantTimes.size() < 3 || plantTimes.size() > 3) ||
		(plantColors.size() < 4 || plantColors.size() > 4) ||
		(plantSizes.size() < 4 || plantSizes.size() > 4) ||
		(plantTextures.size() < 4 || plantTextures.size() > 4))
		throw std::runtime_error("Error: Unequal sized vectors.");

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
			plantSizes[0].x,
			plantSizes[0].y
		}
	);

	entityMakerNC.addComponent
	(
		entity,
		Component::Texture{ plantTextures[0] }
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
			plantSizes[0].x / 2.f,
			plantSizes[0].y / 2.f
		}
	);

	entityMakerNC.addComponent
	(
		entity,
		Component::Color{ plantColors[0] }
	);

	entityMakerNC.addComponent
	(
		entity,
		Component::PlantTimes
		{
			plantTimes[0],
			plantTimes[1],
			plantTimes[2]
		}
	);

	entityMakerNC.addComponent
	(
		entity,
		Component::PlantSizes
		{
			plantSizes[0].x,
			plantSizes[0].y,

			plantSizes[1].x,
			plantSizes[1].y,

			plantSizes[2].x,
			plantSizes[2].y,

			plantSizes[3].x,
			plantSizes[3].y
		}
	);

	entityMakerNC.addComponent
	(
		entity,
		Component::PlantTextures
		{
			plantTextures[0],
			plantTextures[1],
			plantTextures[2],
			plantTextures[3]
		}
	);

	entityMakerNC.addComponent
	(
		entity,
		Component::PlantColors
		{
			plantColors[0],
			plantColors[1],
			plantColors[2],
			plantColors[3]
		}
	);

	return entity;
}