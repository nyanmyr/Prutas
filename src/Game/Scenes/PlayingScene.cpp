#include <SFML/Graphics.hpp>
#include "../src/Game/Headers/GameManager.hpp"
#include "../src/Game/Headers/Scenes.hpp"

void playingScene
(
	sf::RenderWindow& window,
	sf::Font& font
)
{
	window.setView
	(
		sf::View
		(
			sf::Vector2f
			(
				0.f,
				0.f
			),
			sf::Vector2f
			(
				window.getDefaultView().getSize().x,
				window.getDefaultView().getSize().y
			)
		)
	);

	NacreCoordinator& nc = NacreCoordinator::getInstance();

	const double PLAYER_SIZE = 40.0;

	const double Y_BOUNDS_TOP = -(PLAYER_SIZE / 2.0);
	const double Y_BOUNDS_BOTTOM = window.getDefaultView().getSize().y / 2 - (PLAYER_SIZE / 2.0);

	const double X_BOUNDS_LEFT = -500.0;
	const double X_BOUNDS_RIGHT = 500.0;

	// game state variables
	sf::Clock clock;
	std::queue<Entity> renderQueue;

	Entity loadedTextures = makeLoadedTexturesContainer();

	// entity instantiation
	Entity player = makePlayer
	(
		Enum::Texture::TEXTURE_PLACEHOLDER,
		sf::Vector2f
		(
			0,
			0
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
		sf::Vector2f
		(
			0.0,
			150.0
		),
		sf::Vector2f
		(
			window.getDefaultView().getSize().x,
			window.getDefaultView().getSize().y / 2.0
		),
		sf::Color::Green
	);

	Entity camera = makeCamera
	(
		X_BOUNDS_LEFT,
		X_BOUNDS_RIGHT,
		player
	);

	Entity potatoSeed = makeItem
	(
		Enum::Texture::TEXTURE_PLACEHOLDER,
		sf::Vector2f
		(
			0.0,
			150.0
		),
		sf::Vector2f
		(
			30.0,
			30.0
		),
		sf::Color
		(
			150,
			100,
			0
		),
		Enum::POTATO_SEED,
		50.0
	);

	const std::unordered_map<Enum::Item, int> forageSpot1ItemTable
	{
		{Enum::POTATO_SEED, 3},
		{Enum::WHEAT_SEED, 4},
		{Enum::BARLEY_SEED, 4},
		{Enum::CARROT_SEED, 2},
		{Enum::CORN_SEED, 1},
		{Enum::SUNFLOWER_SEED, 2},
	};

	Entity forageSpot1 = makeForageSpot
	(
		Enum::Texture::TEXTURE_PLACEHOLDER,
		sf::Vector2f
		(
			0.0,
			150.0
		),
		sf::Vector2f
		(
			30.0,
			30.0
		),
		sf::Color
		(
			150,
			100,
			0
		),
		50.0,
		forageSpot1ItemTable
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

			if (const auto& keyReleased = event->getIf<sf::Event::KeyReleased>())
			{
				if (keyReleased->scancode == sf::Keyboard::Scancode::Space)
				{
					Control::pickup(player);
					
				}
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
		Update::doBounds
		(
			Y_BOUNDS_TOP,
			Y_BOUNDS_BOTTOM,
			X_BOUNDS_LEFT,
			X_BOUNDS_RIGHT
		);
		Update::followCamera
		(
			camera,
			dt,
			window
		);
		Update::deleteEntities(dt);

		window.clear();
		// render systems
		Render::alignZIndexWithYAxis
		(
			Y_BOUNDS_TOP,
			Y_BOUNDS_BOTTOM,
			player
		);
		Render::doZIndex(renderQueue);
		Render::render
		(
			window,
			renderQueue
		);
		window.display();
	}
}