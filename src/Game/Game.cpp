#include <SFML/Graphics.hpp>

#include "Headers/GameManager.hpp"
#include "Headers/Scenes.hpp"

#include <iostream>

using sf::RenderWindow;
using sf::VideoMode;

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

constexpr int MAX_FPS = 60;

const std::string FONT_FILEPATH = RESOURCES_PATH "arial.ttf";

void main()
{
	RenderWindow window(VideoMode({ SCREEN_WIDTH, SCREEN_HEIGHT }), "Nacre Engine", sf::Style::Close);
	window.setFramerateLimit(MAX_FPS);

	NacreCoordinator& nc = NacreCoordinator::getInstance();

	// components registration
	nc.registerComponent<Component::Position>();
	nc.registerComponent<Component::Transform>();
	nc.registerComponent<Component::Origin>();
	nc.registerComponent<Component::Button>();
	nc.registerComponent<Component::Text>();
	nc.registerComponent<Component::NextScene>();
	nc.registerComponent<Component::ZIndex>();
	nc.registerComponent<Component::YAxisAdd>();
	nc.registerComponent<Component::Velocity>();
	nc.registerComponent<Component::Speed>();
	nc.registerComponent<Component::PlayerController>();
	nc.registerComponent<Component::PlayerAction>();
	nc.registerComponent<Component::Drag>();
	nc.registerComponent<Component::Sprite>();
	nc.registerComponent<Component::Texture>();
	nc.registerComponent<Component::TexturesContainer>();
	nc.registerComponent<Component::Color>();
	nc.registerComponent<Component::Camera>();
	nc.registerComponent<Component::Inventory>();
	nc.registerComponent<Component::Shillings>();
	nc.registerComponent<Component::Item>();
	nc.registerComponent<Component::ForageSpot>();
	nc.registerComponent<Component::PlantTimes>();
	nc.registerComponent<Component::PlantSizes>();
	nc.registerComponent<Component::PlantTextures>();
	nc.registerComponent<Component::PlantColors>();
	nc.registerComponent<Component::PlantHarvest>();
	nc.registerComponent<Component::PlantGrowthRate>();
	nc.registerComponent<Component::PlantHealth>();
	nc.registerComponent<Component::PlantHealthGrowth>();
	nc.registerComponent<Component::Delete>();
	nc.registerComponent<Component::SellArea>();

	sf::Font font;
	if (!font.openFromFile(FONT_FILEPATH))
	{
		throw std::runtime_error("Font not found.");
	}

	playScene
	(
		window,
		Scene::PLAYING,
		font
	);
}