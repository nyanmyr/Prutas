#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <SFML/Graphics.hpp>

#include "Scenes.hpp"
#include "Enums.hpp"

#include <optional>
#include <string>
#include <vector>

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
		int yAxisAdd = 0;
		bool visible = true;
	};

	struct YAxisAdd
	{
		int amount = 0;
		bool enabled = true;
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

	struct PlayerAction
	{
		Enum::PlayerState state = Enum::PlayerState::IDLE;
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
		uint8_t current = 0;
	};

	struct Shillings
	{
		int amount = 0;
	};

	struct Item
	{
		Enum::Item type = Enum::Item::NULL_ITEM;
		double pickupDistance = 0.0;
	};

	struct ForageSpot
	{
		double forageDistance = 0.0;
		std::vector<Enum::Item> items{};
		std::vector<int> probabilityWeights{};
	};

	struct PlantTimes
	{
		double seed = 0.0;
		double seedling = 0.0;
		double flowering = 0.0;
	};

	struct PlantSizes
	{
		double seedX = 0.0;
		double seedY = 0.0;

		double seedlingX = 0.0;
		double seedlingY = 0.0;

		double floweringX = 0.0;
		double floweringY = 0.0;

		double matureX = 0.0;
		double matureY = 0.0;
	};

	struct PlantTextures
	{
		Enum::Texture seed = Enum::Texture::TEXTURE_PLACEHOLDER;
		Enum::Texture seedling = Enum::Texture::TEXTURE_PLACEHOLDER;
		Enum::Texture flowering = Enum::Texture::TEXTURE_PLACEHOLDER;
		Enum::Texture mature = Enum::Texture::TEXTURE_PLACEHOLDER;
	};

	struct PlantColors
	{
		sf::Color seed = sf::Color::White;
		sf::Color seedling = sf::Color::White;
		sf::Color flowering = sf::Color::White;
		sf::Color mature = sf::Color::White;
	};

	struct PlantHarvest
	{
		double harvestDistance = 0.0;
		std::vector<Enum::Item> items{};
		// x: min, y: max
		std::vector<sf::Vector2i> amounts{};
	};

	struct PlantGrowthRate
	{
		double spring = 0.0;
		double summer = 0.0;
		double fall = 0.0;
		double winter = 0.0;
	};

	struct Delete
	{
		double timer = 0.0;
	};

	struct SellArea
	{
		double distance = 0.0;
	};

	struct TimeCycle
	{
		double time = 0.0;
		uint8_t days = 0;
		uint8_t weeks = 0;
		uint8_t months = 0;
	};

	struct TimePassed
	{
		// can get overflow
		double time = 0.0;
		unsigned int days = 0;
		unsigned int weeks = 0;
		unsigned int months = 0;
		unsigned int seasons = 0;
		unsigned int years = 0;
	};

	struct TimeDefaults
	{
		double time = 0.0; // in seconds
		uint8_t days = 0;
		uint8_t weeks = 0;
		uint8_t months = 0; // per season
	};

	struct Season
	{
		Enum::Season current = Enum::Season::SPRING;
	};
}

#endif