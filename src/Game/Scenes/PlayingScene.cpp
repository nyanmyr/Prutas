#include <SFML/Graphics.hpp>
#include "../src/Game/Headers/GameManager.hpp"
#include "../src/Game/Headers/Scenes.hpp"
#include <iostream>

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
		Enum::Item::POTATO,
		50.0
	);

	Entity forageSpot1 = makeForageSpot
	(
		Enum::Texture::TEXTURE_PLACEHOLDER,
		sf::Vector2f
		(
			150.0,
			150.0
		),
		sf::Vector2f
		(
			30.0,
			30.0
		),
		sf::Color
		(
			200,
			100,
			0
		),
		50.0,
		{
			{Enum::Item::POTATO, 3},
			{Enum::Item::WHEAT_SEED, 4},
			{Enum::Item::BARLEY_SEED, 4},
			{Enum::Item::CARROT, 2},
			{Enum::Item::CORN_SEED, 1},
			{Enum::Item::SUNFLOWER_SEED, 2},
		}
	);

	Entity seller = makeSeller
	(
		Enum::Texture::TEXTURE_PLACEHOLDER,
		sf::Vector2f
		(
			-100.0,
			25.0
		),
		sf::Vector2f
		(
			30.0,
			40.0
		),
		sf::Color
		(
			3,
			86,
			252
		),
		50.0
	);
	
	// per second
	const double TIMECYCLE_MINUTES = 60.0;
	const double TIMECYCLE_HOURS = 12.0; // whole day

	const double STARTING_TIME = (TIMECYCLE_MINUTES * 3); // equal to 6 AM
	const Enum::Season STARTING_SEASON = Enum::Season::SPRING;

	const double DEFAULT_TIME = (TIMECYCLE_MINUTES * TIMECYCLE_HOURS);
	const uint8_t DEFAULT_DAYS = 7; // per week
	const uint8_t DEFAULT_WEEKS = 4; // per month
	const uint8_t DEFAULT_MONTHS = 3; // per season

	// 720 seconds per day
	// 4 weeks per month = 28 days per month
	// 4 seasons * 3 months per season = 12 months per year

	// universal entity
	Component::TimeCycle timeCycle
	{
		DEFAULT_TIME, // time (in seconds)
		DEFAULT_DAYS, // days
		DEFAULT_WEEKS, // weeks
		DEFAULT_MONTHS // months
	};
	Component::TimePassed timePassed{};
	Component::TimeDefaults timeDefaults
	{
		DEFAULT_TIME, // time (in seconds)
		DEFAULT_DAYS, // days
		DEFAULT_WEEKS, // weeks
		DEFAULT_MONTHS // months
	};
	Component::Season season{ STARTING_SEASON };



	Entity potatoPlant = makePlant
	(
		sf::Vector2f
		(
			150.0,
			100.0
		),
		{ // plantTimes
			1.0,
			1.0,
			1.0
		},
		{ // plantColors
			sf::Color(51, 43, 6),
			sf::Color(77, 43, 6),
			sf::Color(171, 43, 6),
			sf::Color(234, 43, 6),
		},
		{ // plantSizes
			sf::Vector2f(5.0, 5.0),
			sf::Vector2f(10.0, 10.0),
			sf::Vector2f(20.0, 20.0),
			sf::Vector2f(30.0, 30.0)
		},
		{ // plantTextures
			Enum::Texture::TEXTURE_PLACEHOLDER,
			Enum::Texture::TEXTURE_PLACEHOLDER,
			Enum::Texture::TEXTURE_PLACEHOLDER,
			Enum::Texture::TEXTURE_PLACEHOLDER,
		},
		{
			{
				Enum::Item::POTATO,
				{2, 3}
			}
		},
		50.0,
		1.0,
		1.0,
		1.0,
		1.0,
		100.0,
		10 / DEFAULT_TIME,
		10 / DEFAULT_TIME,
		10 / DEFAULT_TIME,
		-(10 / DEFAULT_TIME)
	);

	Entity cornPlant = makePlant
	(
		sf::Vector2f
		(
			-150.0,
			100.0
		),
		{ // plantTimes
			5.0,
			5.0,
			5.0
		},
		{ // plantColors
			sf::Color(51, 252, 3),
			sf::Color(77, 252, 3),
			sf::Color(171, 252, 3),
			sf::Color(234, 252, 3),
		},
		{ // plantSizes
			sf::Vector2f(5.0, 5.0),
			sf::Vector2f(10.0, 10.0),
			sf::Vector2f(20.0, 30.0),
			sf::Vector2f(30.0, 50.0)
		},
		{ // plantTextures
			Enum::Texture::TEXTURE_PLACEHOLDER,
			Enum::Texture::TEXTURE_PLACEHOLDER,
			Enum::Texture::TEXTURE_PLACEHOLDER,
			Enum::Texture::TEXTURE_PLACEHOLDER,
		},
		{
			{
				Enum::Item::CORN_SEED,
				{1, 3}
			},
			{
				Enum::Item::CORN,
				{1, 1}
			}
		},
		50.0,
		1.0,
		1.0,
		1.0,
		1.0,
		100.0,
		10 / DEFAULT_TIME,
		10 / DEFAULT_TIME,
		10 / DEFAULT_TIME,
		-(10 / DEFAULT_TIME)
	);

	std::unordered_map<Enum::Item, Component::Good> goods
	{
		{
			Enum::Item::POTATO,
			Component::Good
			{
				Component::Price
				{
					50, // starting
					50, // current
				},
				Component::Stock
				{
					10, // starting
					10 // current
				},
				Component::PriceGrowth
				{
					0.1
				}
			}
		}
	};

	// universal entity
	Component::Economy economy
	{
		std::move(goods)
	};

	// onstart systems
	Start::setText(font); // font system is limited to one font
	Start::setTextOrigin();

	while (window.isOpen())
	{
		DeltaTime dt = clock.restart().asSeconds();

		Update::timeCycle
		(
			dt,
			timeCycle,
			timePassed,
			timeDefaults,
			season
		);

		// convert to Update methods
		Start::loadTextures(loadedTextures);
		Start::loadSprites(loadedTextures);
		Start::setSpriteOrigin();
		Start::setColor();

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
					Control::forage(player);
					Control::harvest(player);
				}

				if (keyReleased->scancode == sf::Keyboard::Scancode::R)
				{
					Control::openInventory(player);
				}

				if (keyReleased->scancode == sf::Keyboard::Scancode::Q)
				{
					Control::inventorySelectLeft(player);
				}

				if (keyReleased->scancode == sf::Keyboard::Scancode::E)
				{
					Control::inventorySelectRight(player);
				}

				if (keyReleased->scancode == sf::Keyboard::Scancode::F)
				{
					Control::plant(player);
				}

				if (keyReleased->scancode == sf::Keyboard::Scancode::Num1)
				{
					Control::sellAllItems
					(
						player,
						economy
					);
				}

				// for debugging
				if (keyReleased->scancode == sf::Keyboard::Scancode::X)
				{
					std::cout << "Shillings: " <<
						nc.getComponentArray<Component::Shillings>()->getData(player).amount << "\n";
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
		Update::grow
		(
			loadedTextures,
			dt,
			season
		);
		Update::handlePlantHealth
		(
			dt,
			season,
			timeDefaults
		);
		Update::handlePlantDeath();
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