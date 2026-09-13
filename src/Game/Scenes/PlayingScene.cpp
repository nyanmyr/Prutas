#include <SFML/Graphics.hpp>
#include "../src/Game/Headers/GameManager.hpp"
#include "../src/Game/Headers/Scenes.hpp"

void playingScene
(
	sf::RenderWindow& window,
	sf::Font& font
)
{
	NacreCoordinator& nc = NacreCoordinator::getInstance();

	const double PLAYER_SIZE = 40.0;

	const double Y_BOUNDS_TOP = window.getDefaultView().getSize().y / 2.0;
	const double Y_BOUNDS_BOTTOM = window.getDefaultView().getSize().y - (PLAYER_SIZE / 2.0);

	// game state variables
	sf::Clock clock;
	std::queue<Entity> renderQueue;

	Entity loadedTextures = makeLoadedTexturesContainer();

	// entity instantiation
	Entity player = makePlayer
	(
		Enum::Texture::TEXTURE_PLACEHOLDER,
		sf::Vector2f(
			window.getDefaultView().getSize().x / 2.0,
			window.getDefaultView().getSize().y / 2.0
		),
		{
			PLAYER_SIZE,
			PLAYER_SIZE
		},
		{
			-300.0,
			-300.0
		},
		{
			300.0,
			300.0
		},
		{
			500.0,
			500.0
		},
		{
			50.0,
			50.0
		},
		sf::Color::White
	);

	Entity background = makeObject
	(
		Enum::Texture::TEXTURE_PLACEHOLDER,
		sf::Vector2f(
			window.getDefaultView().getSize().x / 2.0,
			window.getDefaultView().getSize().y * 0.75
		),
		sf::Vector2f(
			window.getDefaultView().getSize().x,
			window.getDefaultView().getSize().y / 2.0
		),
		sf::Color::Green
	);

	// onstart systems
	Start::setText(font); // font system is limited to one font
	Start::setTextOrigin();

	Start::loadTextures(loadedTextures);
	Start::loadSprites(loadedTextures);
	Start::setSpriteOrigin();
	Start::setColor();

	while (window.isOpen())
	{
		DeltaTime dt = clock.restart().asSeconds();

		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}

		// systems
		Control::doPlayerControl
		(
			player,
			dt
		);
		Update::move(dt);
		Update::drag(dt);
		Update::doYBounds
		(
			Y_BOUNDS_TOP,
			Y_BOUNDS_BOTTOM
		);

		window.clear();
		// render systems
		Render::doZIndex(renderQueue);
		Render::render
		(
			window,
			renderQueue
		);
		window.display();
	}
}