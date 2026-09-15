#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <SFML/Graphics.hpp>
#include "Scenes.hpp"
#include "Enums.hpp"
#include <optional>
#include <string>

namespace Component
{
	struct Position
	{
		double x = 0.0;
		double y = 0.0;
	};

	struct Transform
	{
		double width = 0.0;
		double height = 0.0;
	};

	struct Origin
	{
		double offsetX = 0.0;
		double offsetY = 0.0;
	};

	struct Button
	{
		double clickedDuration = 0.0;
		double enabled = true;
		double clickedTimer = 0.0;
		bool clicked = false;
	};

	struct Text
	{
		std::optional<sf::Text> box{};
		std::string string = "";
		int size = 12;
		sf::Color color = sf::Color::White;
		Enum::TextFormat format = Enum::TextFormat::MIDDLE;
	};

	struct NextScene
	{
		Scene next = Scene::MENU;
		bool active = false;
	};

	struct ZIndex
	{
		int index = 0;
		bool visible = true;
	};

	struct Velocity
	{
		double minX = 0.0;
		double minY = 0.0;
		double maxX = 0.0;
		double maxY = 0.0;
		double x = 0.0;
		double y = 0.0;
	};

	struct Speed
	{
		double x = 0.0;
		double y = 0.0;
	};

	struct PlayerController
	{
		bool enabled = false;
	};

	struct Drag
	{
		double x = 0.0;
		double y = 0.0;
	};

	struct Sprite
	{
		std::optional<sf::Sprite> body{};
	};

	struct Texture
	{
		Enum::Texture data;
	};

	struct TexturesContainer
	{
		std::unordered_map<Enum::Texture, sf::Texture> map;
	};

	struct Color
	{
		sf::Color col = sf::Color(sf::Color::White);
	};

	struct Camera
	{
		double min = 0.0;
		double max = 0.0;
		Entity target = NULL_ENTITY;
	};

	struct Inventory
	{
		std::vector<Enum::Item> items{};
	};

	struct Item
	{
		Enum::Item type = Enum::Item::NULL_ITEM;
		double pickupDistance = 0.0;
	};

	struct Delete
	{
		double timer = 0.0;
	};
}

#endif