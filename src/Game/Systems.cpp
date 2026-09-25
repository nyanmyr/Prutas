#include "Headers/Systems.hpp"
#include "Headers/EntityMaker.hpp"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include <chrono>

NacreCoordinator& systemsNC = NacreCoordinator::getInstance();

// -------------------------------------------------------
// start systems
// -------------------------------------------------------
void Start::setText(sf::Font& font)
{
	auto& textArray = systemsNC.getComponentArray<Component::Text>();

	for (auto& [entity, textObj] : textArray->getAll())
	{
		textObj.box.value().setString(textObj.string);
		textObj.box.value().setCharacterSize(textObj.size);
		textObj.box.value().setFillColor(textObj.color);
	}
}
void Start::setTextOrigin()
{
	auto& textArray = systemsNC.getComponentArray<Component::Text>();
	auto& transformArray = systemsNC.getComponentArray<Component::Transform>();

	double offsetX;
	double offsetY;

	for (auto& [entity, textObj] : textArray->getAll())
	{
		if (!transformArray->hasData(entity))
		{
			continue;
		}

		Component::Transform& transform = transformArray->getData(entity);

		switch (textObj.format)
		{
		case Enum::TextFormat::TOP:
			offsetX = textObj.box.value().getLocalBounds().size.x / 2;
			offsetY = textObj.box.value().getLocalBounds().size.y;
			break;
		case Enum::TextFormat::BOTTOM:
			offsetX = textObj.box.value().getLocalBounds().size.x / 2;
			offsetY = textObj.box.value().getLocalBounds().size.y / 2;
			break;
		case Enum::TextFormat::MIDDLE:
		default:
			offsetX = textObj.box.value().getLocalBounds().size.x / 2;
			offsetY = (textObj.box.value().getLocalBounds().size.y / 2) + (textObj.box.value().getLocalBounds().size.y / 4);
			break;
		}

		textObj.box.value().setOrigin
		(
			sf::Vector2f
			(
				offsetX,
				offsetY
			)
		);
	}
}
void Start::setSpriteOrigin()
{
	auto& originArray = systemsNC.getComponentArray<Component::Origin>();
	auto& spriteArray = systemsNC.getComponentArray<Component::Sprite>();

	for (auto& [entity, spriteObj] : spriteArray->getAll())
	{
		if (!originArray->hasData(entity))
		{
			continue;
		}

		Component::Origin& originObj = originArray->getData(entity);

		spriteObj.body->setOrigin
		(
			sf::Vector2f
			(
				originObj.offsetX / spriteObj.body->getScale().x,
				originObj.offsetY / spriteObj.body->getScale().y
			)
		);
	}
}
void Start::loadTextures(const Entity loadedTextures)
{
	auto& texturesContainerArray = systemsNC.getComponentArray<Component::TexturesContainer>();

	if (!texturesContainerArray->hasData(loadedTextures))
	{
		return;
	}

	Component::TexturesContainer& containerObj = texturesContainerArray->getData(loadedTextures);

	containerObj.map.emplace(Enum::Texture::TEXTURE_PLACEHOLDER, sf::Texture(SPRITES_PATH "placeholder_texture.jpeg"));
}
void Start::loadSprites(const Entity loadedTextures)
{
	auto& spriteArray = systemsNC.getComponentArray<Component::Sprite>();
	auto& transformArray = systemsNC.getComponentArray<Component::Transform>();
	auto& textureArray = systemsNC.getComponentArray<Component::Texture>();
	auto& texturesContainerArray = systemsNC.getComponentArray<Component::TexturesContainer>();

	if (!texturesContainerArray->hasData(loadedTextures))
	{
		return;
	}

	Component::TexturesContainer& containerObj = texturesContainerArray->getData(loadedTextures);

	for (auto& [entity, spriteObj] : spriteArray->getAll())
	{
		if (!transformArray->hasData(entity) ||
			!textureArray->hasData(entity))
		{
			continue;
		}

		Component::Texture& textureObj = textureArray->getData(entity);
		Component::Transform& transformObj = transformArray->getData(entity);

		spriteObj.body.emplace(containerObj.map[textureObj.data]);
		spriteObj.body->setScale
		(
			sf::Vector2f
			(
				transformObj.width / spriteObj.body->getGlobalBounds().size.x,
				transformObj.height / spriteObj.body->getGlobalBounds().size.y
			)
		);
	}
}
void Start::setColor()
{
	auto& colorArray = systemsNC.getComponentArray<Component::Color>();
	auto& spriteArray = systemsNC.getComponentArray<Component::Sprite>();

	for (auto& [entity, colorObj] : colorArray->getAll())
	{
		if (!spriteArray->hasData(entity))
		{
			continue;
		}

		Component::Sprite& spriteObj = spriteArray->getData(entity);

		spriteObj.body->setColor(colorObj.col);
	}
}

// -------------------------------------------------------
// control systems
// -------------------------------------------------------
const double DEFAULT_SCALE_X = 1.0;
const double DEFAULT_SCALE_Y = 1.0;

const double HOVER_SCALE_X = 1.1;
const double HOVER_SCALE_Y = 1.1;

const double CLICKED_SCALE_X = 0.9;
const double CLICKED_SCALE_Y = 0.9;

void Control::buttonClicks
(
	const sf::Vector2i mouseVector,
	const DeltaTime dt
)
{
	auto& buttonArray = systemsNC.getComponentArray<Component::Button>();
	auto& originArray = systemsNC.getComponentArray<Component::Origin>();
	auto& transformArray = systemsNC.getComponentArray<Component::Transform>();
	auto& positionArray = systemsNC.getComponentArray<Component::Position>();

	for (auto& [entity, buttonObj] : buttonArray->getAll())
	{
		if (!buttonObj.enabled ||
			!originArray->hasData(entity) ||
			!positionArray->hasData(entity) ||
			!transformArray->hasData(entity))
		{
			continue;
		}

		// buttonArray must have a shape, origin, and text
		////std::cout << "button.top: " << button.top << "\n";
		////std::cout << "button.left: " << button.left << "\n";
		const Component::Origin& originObj = originArray->getData(entity);
		const Component::Transform& transformObj = transformArray->getData(entity);
		const Component::Position& posObj = positionArray->getData(entity);

		if (mouseVector.x > posObj.x - originObj.offsetX &&
			mouseVector.x < posObj.x + transformObj.width - originObj.offsetX &&
			mouseVector.y > posObj.y - originObj.offsetY &&
			mouseVector.y < posObj.y + transformObj.height - originObj.offsetY)
		{
			buttonObj.clicked = true;
			buttonObj.clickedTimer = buttonObj.clickedDuration;
		}
	}
}
void Control::doPlayerControl
(
	const Entity player,
	const DeltaTime dt
)
{
	auto& velocityArray = systemsNC.getComponentArray<Component::Velocity>();
	auto& speedArray = systemsNC.getComponentArray<Component::Speed>();
	auto& playerControllerArray = systemsNC.getComponentArray<Component::PlayerController>();

	if (!velocityArray->hasData(player) ||
		!speedArray->hasData(player) ||
		!playerControllerArray->hasData(player))
	{
		return;
	}

	const Component::PlayerController playerControllerObj = playerControllerArray->getData(player);
	const Component::Speed speedObj = speedArray->getData(player);
	Component::Velocity& velocityObj = velocityArray->getData(player);

	if (!playerControllerObj.enabled)
	{
		return;
	}

	double newSpeedX = 0.0;
	double newSpeedY = 0.0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		newSpeedY = -speedObj.y;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		newSpeedY = speedObj.y;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		newSpeedX = -speedObj.x;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		newSpeedX = speedObj.x;
	}

	// applies the speed (even if there aren't any changes)
	velocityObj.x += (newSpeedX * dt);
	velocityObj.y += (newSpeedY * dt);

	if (velocityObj.x > velocityObj.maxX)
	{
		velocityObj.x = velocityObj.maxX;
	}
	else if (velocityObj.x < velocityObj.minX)
	{
		velocityObj.x = velocityObj.minX;
	}

	if (velocityObj.y > velocityObj.maxY)
	{
		velocityObj.y = velocityObj.maxY;
	}
	else if (velocityObj.y < velocityObj.minY)
	{
		velocityObj.y = velocityObj.minY;
	}

}

void Control::pickup(const Entity player)
{
	auto& positionArray = systemsNC.getComponentArray<Component::Position>();
	auto& itemArray = systemsNC.getComponentArray<Component::Item>();
	auto& inventoryArray = systemsNC.getComponentArray<Component::Inventory>();

	if (!positionArray->hasData(player) ||
		!inventoryArray->hasData(player)) return;

	const Component::Position playerPos = positionArray->getData(player);
	Component::Inventory& playerInventory = inventoryArray->getData(player);

	for (auto& [entity, item] : itemArray->getAll())
	{
		if (!positionArray->hasData(entity) ||
			systemsNC.getComponentArray<Component::Delete>()->hasData(entity)) continue;

		const Component::Position itemPos = positionArray->getData(entity);

		double distance = [](Component::Position a, Component::Position b)
			{
				return std::sqrt(std::pow(a.x - b.x, 2.0) + std::pow(a.y - b.y, 2.0));
			}
		(playerPos, itemPos);

		//std::cout << "distance: " << distance << "\n";
		//std::cout << "item.pickupdistance: " << item.pickupDistance << "\n";

		if (distance > item.pickupDistance) continue;
		

		playerInventory.items.push_back(item.type);
		systemsNC.addComponent
		(
			entity,
			Component::Delete{ 0.0 }
		);
	}
}
void Control::forage(const Entity player)
{
	auto& positionArray = systemsNC.getComponentArray<Component::Position>();
	auto& forageSpotArray = systemsNC.getComponentArray<Component::ForageSpot>();
	auto& inventoryArray = systemsNC.getComponentArray<Component::Inventory>();

	if (!positionArray->hasData(player) ||
		!inventoryArray->hasData(player)) return;

	const Component::Position playerPos = positionArray->getData(player);
	Component::Inventory& playerInventory = inventoryArray->getData(player);

	for (auto& [entity, forageSpot] : forageSpotArray->getAll())
	{
		if (!positionArray->hasData(entity) ||
			systemsNC.getComponentArray<Component::Delete>()->hasData(entity)) continue;

		const Component::Position itemPos = positionArray->getData(entity);

		double distance = [](Component::Position a, Component::Position b)
			{
				return std::sqrt(std::pow(a.x - b.x, 2.0) + std::pow(a.y - b.y, 2.0));
			}
		(playerPos, itemPos);

		//std::cout << "distance: " << distance << "\n";
		//std::cout << "item.pickupdistance: " << item.pickupDistance << "\n";

		if (distance > forageSpot.forageDistance) continue;
		

		auto now = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());

		uint64_t ms_unsigned = static_cast<uint64_t>(duration.count());

		uint64_t seed = static_cast<uint64_t>(ms_unsigned);
		std::mt19937 generator(seed);

		std::discrete_distribution<int> distrib
		(
			forageSpot.probabilityWeights.begin(),
			forageSpot.probabilityWeights.end()
		);

		int randomWeight = distrib(generator);
		Enum::Item randomItem = forageSpot.items[randomWeight];

		playerInventory.items.push_back(randomItem);
		//std::cout << "foraged item: " << static_cast<int>(randomItem) << "\n";
	}
}
void Control::harvest(const Entity player)
{
	auto& positionArray = systemsNC.getComponentArray<Component::Position>();
	auto& plantHarvestArray = systemsNC.getComponentArray<Component::PlantHarvest>();
	auto& plantTimesArray = systemsNC.getComponentArray<Component::PlantTimes>();
	auto& inventoryArray = systemsNC.getComponentArray<Component::Inventory>();

	if (!positionArray->hasData(player) ||
		!inventoryArray->hasData(player)) return;

	const Component::Position playerPos = positionArray->getData(player);
	Component::Inventory& playerInventory = inventoryArray->getData(player);

	for (auto& [entity, plantHarvest] : plantHarvestArray->getAll())
	{
		if (!positionArray->hasData(entity) ||
			!plantTimesArray->hasData(entity) ||
			systemsNC.getComponentArray<Component::Delete>()->hasData(entity)) continue;

		const Component::Position& itemPos = positionArray->getData(entity);
		const Component::PlantTimes& plantTimes = plantTimesArray->getData(entity);

		if (plantTimes.seed > 0.0 ||
			plantTimes.seedling > 0.0 ||
			plantTimes.flowering > 0.0) continue;

		double distance = [](Component::Position a, Component::Position b)
			{
				return std::sqrt(std::pow(a.x - b.x, 2.0) + std::pow(a.y - b.y, 2.0));
			}
		(playerPos, itemPos);

		//std::cout << "distance: " << distance << "\n";
		//std::cout << "item.pickupdistance: " << item.pickupDistance << "\n";

		if (distance > plantHarvest.harvestDistance) continue;

		auto now = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());

		uint64_t ms_unsigned = static_cast<uint64_t>(duration.count());

		uint64_t seed = static_cast<uint64_t>(ms_unsigned);
		std::mt19937 generator(seed);

		for (size_t i = 0; i < plantHarvest.items.size(); ++i)
		{
			//std::cout << "x: " << plantHarvest.amounts[i].x << "\n";
			//std::cout << "y: " << plantHarvest.amounts[i].y << "\n";
			std::uniform_int_distribution<int> amountsDistrib
			(
				plantHarvest.amounts[i].x,
				plantHarvest.amounts[i].y
			);

			int randomAmount = amountsDistrib(generator);
			//std::cout << "randomAmount: " << randomAmount << "\n";

			while (randomAmount > 0)
			{
				playerInventory.items.push_back(plantHarvest.items[i]);
				randomAmount--;
			}
		}

		//std::cout << "inventory size: " << static_cast<int>(playerInventory.items.size()) << "\n";

		systemsNC.addComponent
		(
			entity,
			Component::Delete{ 0.0 }
		);
	}
}
void Control::openInventory(const Entity player)
{
	if (!systemsNC.getComponentArray<Component::PlayerController>()->hasData(player) ||
		!systemsNC.getComponentArray<Component::PlayerAction>()->hasData(player) ||
		!systemsNC.getComponentArray<Component::Velocity>()->hasData(player) ||
		!systemsNC.getComponentArray<Component::Inventory>()->hasData(player)) return;

	Component::PlayerController& playerController = systemsNC.getComponentArray<Component::PlayerController>()->getData(player);
	Component::PlayerAction& playerAction = systemsNC.getComponentArray<Component::PlayerAction>()->getData(player);
	Component::Velocity& velocity = systemsNC.getComponentArray<Component::Velocity>()->getData(player);
	Component::Inventory& inventory = systemsNC.getComponentArray<Component::Inventory>()->getData(player);

	if (inventory.items.empty())
	{
		//std::cout << "Inventory is empty!" << "\n";
		return;
	}

	// toggles it on or off
	playerController.enabled = !playerController.enabled;
	// std::cout << "Opened inventory: " << (playerController.enabled ? "True" : "False") << "\n";

	playerAction.state = playerController.enabled ?
		Enum::PlayerState::IDLE : Enum::PlayerState::OPENED_INVENTORY;

	if (!playerController.enabled)
	{
		velocity.x = velocity.x * 0.1;
		velocity.y = velocity.y * 0.1;
	}

	//std::cout << "Currently Selected: " << static_cast<int>(inventory.items[inventory.current]) << "\n";
	//std::cout << "Inventory Size: " << static_cast<int>(inventory.items.size()) << "\n";
}
void Control::inventorySelectLeft(const Entity player)
{
	if (!systemsNC.getComponentArray<Component::PlayerController>()->hasData(player) ||
		!systemsNC.getComponentArray<Component::PlayerAction>()->hasData(player) ||
		!systemsNC.getComponentArray<Component::Inventory>()->hasData(player)) return;

	Component::PlayerController& playerController = systemsNC.getComponentArray<Component::PlayerController>()->getData(player);
	const Component::PlayerAction& playerAction = systemsNC.getComponentArray<Component::PlayerAction>()->getData(player);
	Component::Inventory& inventory = systemsNC.getComponentArray<Component::Inventory>()->getData(player);

	if (playerAction.state != Enum::PlayerState::OPENED_INVENTORY) return;

	inventory.current = inventory.current == 0 ? 0 : inventory.current - 1;
	// std::cout << "New current: " << static_cast<int>(inventory.current) << "\n";
}
void Control::inventorySelectRight(const Entity player)
{
	if (!systemsNC.getComponentArray<Component::PlayerController>()->hasData(player) ||
		!systemsNC.getComponentArray<Component::PlayerAction>()->hasData(player) ||
		!systemsNC.getComponentArray<Component::Inventory>()->hasData(player)) return;

	Component::PlayerController& playerController = systemsNC.getComponentArray<Component::PlayerController>()->getData(player);
	const Component::PlayerAction& playerAction = systemsNC.getComponentArray<Component::PlayerAction>()->getData(player);
	Component::Inventory& inventory = systemsNC.getComponentArray<Component::Inventory>()->getData(player);

	if (playerAction.state != Enum::PlayerState::OPENED_INVENTORY) return;

	inventory.current = inventory.current + 1 > inventory.items.size() - 1 ?
		0 : inventory.current + 1;
	// std::cout << "New current: " << static_cast<int>(inventory.current) << "\n";
}

#pragma region ITEM_SELL_PRICES
const int POTATO_SELL_PRICE = 10;
const int CARROT_SELL_PRICE = 10;
const int WHEAT_SEED_SELL_PRICE = 10;
const int WHEAT_SELL_PRICE = 10;
const int BARLEY_SEED_SELL_PRICE = 10;
const int BARLEY_SELL_PRICE = 10;
const int CORN_SEED_SELL_PRICE = 10;
const int CORN_SELL_PRICE = 10;
const int SUNFLOWER_SEED_SELL_PRICE = 10;
const int SUNFLOWER_SELL_PRICE = 10;
#pragma endregion

void Control::sellAllItems(const Entity player)
{
	auto& positionArray = systemsNC.getComponentArray<Component::Position>();
	auto& inventoryArray = systemsNC.getComponentArray<Component::Inventory>();
	auto& shillingsArray = systemsNC.getComponentArray<Component::Shillings>();
	auto& sellAreaArray = systemsNC.getComponentArray<Component::SellArea>();

	if (!positionArray->hasData(player) ||
		!inventoryArray->hasData(player) ||
		!shillingsArray->hasData(player)) return;

	const Component::Position playerPos = positionArray->getData(player);
	Component::Inventory& playerInventory = inventoryArray->getData(player);
	Component::Shillings& shillings = shillingsArray->getData(player);

	if (playerInventory.items.empty()) return;

	for (auto& [entity, sellArea] : sellAreaArray->getAll())
	{
		if (!positionArray->hasData(entity)) continue;

		const Component::Position& sellAreaPos = positionArray->getData(entity);

		double distance = [](Component::Position a, Component::Position b)
			{
				return std::sqrt(std::pow(a.x - b.x, 2.0) + std::pow(a.y - b.y, 2.0));
			}
		(playerPos, sellAreaPos);

		//std::cout << "distance: " << distance << "\n";
		//std::cout << "sellArea.distance: " << sellArea.distance << "\n";

		if (distance > sellArea.distance) continue;

		playerInventory.current = 0;

		for (Enum::Item item : playerInventory.items)
		{
			switch (item)
			{
				case Enum::Item::POTATO:
					shillings.amount += POTATO_SELL_PRICE;
					break;
				case Enum::Item::CARROT:
					shillings.amount += CARROT_SELL_PRICE;
					break;
				case Enum::Item::WHEAT_SEED:
					shillings.amount += WHEAT_SEED_SELL_PRICE;
					break;
				case Enum::Item::WHEAT:
					shillings.amount += WHEAT_SELL_PRICE;
					break;
				case Enum::Item::BARLEY_SEED:
					shillings.amount += BARLEY_SEED_SELL_PRICE;
					break;
				case Enum::Item::BARLEY:
					shillings.amount += BARLEY_SELL_PRICE;
					break;
				case Enum::Item::CORN_SEED:
					shillings.amount += CORN_SEED_SELL_PRICE;
					break;
				case Enum::Item::CORN:
					shillings.amount += CORN_SELL_PRICE;
					break;
				case Enum::Item::SUNFLOWER_SEED:
					shillings.amount += SUNFLOWER_SEED_SELL_PRICE;
					break;
				case Enum::Item::SUNFLOWER:
					shillings.amount += SUNFLOWER_SELL_PRICE;
					break;
				default:
					shillings.amount += 0;
					break;
			}
		}

		playerInventory.items.clear();
	}
}

#pragma region POTATO_PLANT_CONSTANTS
const double POTATO_SEED_PLANT_TIME = 1.0;
const double POTATO_SEEDLING_PLANT_TIME = 1.0;
const double POTATO_FLOWERING_PLANT_TIME = 1.0;

const sf::Color POTATO_SEED_COLOR = sf::Color(51, 43, 6);
const sf::Color POTATO_SEEDLING_COLOR = sf::Color(77, 43, 6);
const sf::Color POTATO_FLOWERING_COLOR = sf::Color(171, 43, 6);
const sf::Color POTATO_MATURE_COLOR = sf::Color(234, 43, 6);

const double POTATO_SEED_X = 5.0;
const double POTATO_SEED_Y = 5.0;

const double POTATO_SEEDLING_X = 10.0;
const double POTATO_SEEDLING_Y = 10.0;

const double POTATO_FLOWERING_X = 20.0;
const double POTATO_FLOWERING_Y = 20.0;

const double POTATO_MATURE_X = 30.0;
const double POTATO_MATURE_Y = 30.0;

const Enum::Texture POTATO_SEED_TEXTURE = Enum::Texture::TEXTURE_PLACEHOLDER;
const Enum::Texture POTATO_SEEDLING_TEXTURE = Enum::Texture::TEXTURE_PLACEHOLDER;
const Enum::Texture POTATO_FLOWERING_TEXTURE = Enum::Texture::TEXTURE_PLACEHOLDER;
const Enum::Texture POTATO_MATURE_TEXTURE = Enum::Texture::TEXTURE_PLACEHOLDER;

std::unordered_map<Enum::Item, sf::Vector2i> POTATO_DROP_TABLE
{
	{
		Enum::Item::POTATO,
		{1, 2}
	}
};
const double POTATO_HARVEST_DISTANCE = 30.0;

const double POTATO_SPRING_GROWTH_RATE = 1.0;
const double POTATO_SUMMER_GROWTH_RATE = 1.5;
const double POTATO_FALL_GROWTH_RATE = 1.0;
const double POTATO_WINTER_GROWTH_RATE = 0.0;

const double POTATO_STARTING_HEALTH = 100.0;

// every day
const double POTATO_SPRING_HEALTH_GROWTH = 1.0;
const double POTATO_SUMMER_HEALTH_GROWTH = 1.0;
const double POTATO_FALL_HEALTH_GROWTH = 1.0;
const double POTATO_WINTER_HEALTH_GROWTH = 0.0;

#pragma endregion

void Control::plant(const Entity player)
{
	if (!systemsNC.getComponentArray<Component::PlayerController>()->hasData(player) ||
		!systemsNC.getComponentArray<Component::PlayerAction>()->hasData(player) ||
		!systemsNC.getComponentArray<Component::Inventory>()->hasData(player) ||
		!systemsNC.getComponentArray<Component::Transform>()->hasData(player) ||
		!systemsNC.getComponentArray<Component::Position>()->hasData(player)) return;

	const Component::Transform transform = systemsNC.getComponentArray<Component::Transform>()->getData(player);
	const Component::Position position = systemsNC.getComponentArray<Component::Position>()->getData(player);
	Component::PlayerController& playerController = systemsNC.getComponentArray<Component::PlayerController>()->getData(player);
	const Component::PlayerAction& playerAction = systemsNC.getComponentArray<Component::PlayerAction>()->getData(player);
	Component::Inventory& inventory = systemsNC.getComponentArray<Component::Inventory>()->getData(player);

	if (inventory.items.empty() ||
		playerAction.state != Enum::PlayerState::IDLE) return;

	bool planted = false;

	double seedPlantTime = 0.0;
	double seedlingPlantTime = 0.0;
	double floweringPlantTime = 0.0;

	sf::Color seedColor = sf::Color::White;
	sf::Color seedlingColor = sf::Color::White;
	sf::Color floweringColor = sf::Color::White;
	sf::Color matureColor = sf::Color::White;

	double seedX = 0.0;
	double seedY = 0.0;

	double seedlingX = 0.0;
	double seedlingY = 0.0;

	double floweringX = 0.0;
	double floweringY = 0.0;

	double matureX = 0.0;
	double matureY = 0.0;

	Enum::Texture seedTexture = Enum::Texture::TEXTURE_PLACEHOLDER;
	Enum::Texture seedlingTexture = Enum::Texture::TEXTURE_PLACEHOLDER;
	Enum::Texture floweringTexture = Enum::Texture::TEXTURE_PLACEHOLDER;
	Enum::Texture matureTexture = Enum::Texture::TEXTURE_PLACEHOLDER;

	std::unordered_map<Enum::Item, sf::Vector2i> dropTable{};
	double harvestDistance = 0.0;

	double springGrowthRate = 0.0;
	double summerGrowthRate = 0.0;
	double fallGrowthRate = 0.0;
	double winterGrowthRate = 0.0;

	double health = 0.0;

	double springHealthGrowth = 0.0;
	double summerHealthGrowth = 0.0;
	double fallHealthGrowth = 0.0;
	double winterHealthGrowth = 0.0;

	if (inventory.items[inventory.current] == Enum::Item::POTATO)
	{
		seedPlantTime = POTATO_SEED_PLANT_TIME;
		seedlingPlantTime = POTATO_SEEDLING_PLANT_TIME;
		floweringPlantTime = POTATO_FLOWERING_PLANT_TIME;

		seedColor = POTATO_SEED_COLOR;
		seedlingColor = POTATO_SEEDLING_COLOR;
		floweringColor = POTATO_FLOWERING_COLOR;
		matureColor = POTATO_MATURE_COLOR;

		seedX = POTATO_SEED_X;
		seedY = POTATO_SEED_Y;

		seedlingX = POTATO_SEEDLING_X;
		seedlingY = POTATO_SEEDLING_Y;

		floweringX = POTATO_FLOWERING_X;
		floweringY = POTATO_FLOWERING_Y;

		matureX = POTATO_MATURE_X;
		matureY = POTATO_MATURE_Y;

		seedTexture = POTATO_SEED_TEXTURE;
		seedlingTexture = POTATO_SEEDLING_TEXTURE;
		floweringTexture = POTATO_FLOWERING_TEXTURE;
		matureTexture = POTATO_MATURE_TEXTURE;

		dropTable = POTATO_DROP_TABLE;
		harvestDistance = POTATO_HARVEST_DISTANCE;

		springGrowthRate = POTATO_SPRING_GROWTH_RATE;
		summerGrowthRate = POTATO_SUMMER_GROWTH_RATE;
		fallGrowthRate = POTATO_FALL_GROWTH_RATE;
		winterGrowthRate = POTATO_WINTER_GROWTH_RATE;

		health = POTATO_STARTING_HEALTH;

		springHealthGrowth = POTATO_SPRING_HEALTH_GROWTH;
		summerHealthGrowth = POTATO_SUMMER_HEALTH_GROWTH;
		fallHealthGrowth = POTATO_FALL_HEALTH_GROWTH;
		winterHealthGrowth = POTATO_WINTER_HEALTH_GROWTH;

		planted = true;
	}

	if (!planted) return;

	// removes the planted item
	inventory.items.erase(inventory.items.begin() + inventory.current);

	// decrements current if it is not 0
	inventory.current = inventory.current == 0 ? 0 : inventory.current - 1;

	makePlant
	(
		sf::Vector2f
		(
			position.x,
			position.y + (transform.height / 2.0)
		),
		{ // plantTimes
			seedPlantTime,
			seedlingPlantTime,
			floweringPlantTime
		},
		{ // plantColors
			seedColor,
			seedlingColor,
			floweringColor,
			matureColor
		},
		{ // plantSizes
			sf::Vector2f(seedX, seedY),
			sf::Vector2f(seedlingX, seedlingY),
			sf::Vector2f(floweringX, floweringY),
			sf::Vector2f(matureX, matureY)
		},
		{ // plantTextures
			seedTexture,
			seedlingTexture,
			floweringTexture,
			matureTexture
		},
		std::move(dropTable),
		harvestDistance,
		springGrowthRate,
		summerGrowthRate,
		fallGrowthRate,
		winterGrowthRate,
		health, // current / starting
		health,
		springHealthGrowth,
		summerHealthGrowth,
		fallHealthGrowth,
		winterHealthGrowth
	);
}

// -------------------------------------------------------
// update systems
// -------------------------------------------------------
const double TIME_SPEED_MULTIPLIER_DEBUG = 2.0;

void Update::timeCycle
(
	const DeltaTime dt,
	Component::TimeCycle& timeCycle,
	Component::TimePassed& timePassed,
	Component::TimeDefaults& timeDefaults,
	Component::Season& season
)
{	
	//std::cout << "time: " << timeCycle.time << "\n";

	timeCycle.time -= (dt * TIME_SPEED_MULTIPLIER_DEBUG);
	timePassed.time += (dt * TIME_SPEED_MULTIPLIER_DEBUG);

	if (timeCycle.time > 0) return;
	//std::cout << "A day has passed!" << "\n";
	timeCycle.time = timeDefaults.time;
	timeCycle.days--;
	timePassed.days++;

	if (timeCycle.days > 0) return;
	//std::cout << "A week has passed!" << "\n";
	timeCycle.days = timeDefaults.days;
	timeCycle.weeks--;
	timePassed.weeks++;

	if (timeCycle.weeks > 0) return;
	//std::cout << "A month has passed!" << "\n";
	timeCycle.weeks = timeDefaults.weeks;
	timeCycle.months--;
	timePassed.months++;

	if (timeCycle.months > 0) return;
	//std::cout << "A new season has come!" << "\n";
	timeCycle.months = timeDefaults.months;
	timePassed.seasons++;
	
	switch (season.current)
	{
	case Enum::Season::SPRING:
		season.current = Enum::Season::SUMMER;
		break;
	case Enum::Season::SUMMER:
		season.current = Enum::Season::FALL;
		break;
	case Enum::Season::FALL:
		season.current = Enum::Season::WINTER;
		break;
	case Enum::Season::WINTER:
		//std::cout << "A year has passed!" << "\n";
		season.current = Enum::Season::SPRING;
		timePassed.years++;
		break;
	}

	//std::cout << "timePassed: " << timePassed.time << "\n";
	//std::cout << "days: " << static_cast<int>(timePassed.days) << "\n";
	//std::cout << "weeks: " << static_cast<int>(timePassed.weeks) << "\n";
	//std::cout << "months: " << static_cast<int>(timePassed.months) << "\n";
	//std::cout << "seasons: " << static_cast<int>(timePassed.seasons) << "\n";
	//std::cout << "years: " << static_cast<int>(timePassed.years) << "\n";
}
void Update::doButtons
(
	const sf::Vector2i mouseVector,
	const DeltaTime dt
) {
	auto& spriteArray = systemsNC.getComponentArray<Component::Sprite>();
	auto& buttonArray = systemsNC.getComponentArray<Component::Button>();
	auto& originArray = systemsNC.getComponentArray<Component::Origin>();
	auto& textArray = systemsNC.getComponentArray<Component::Text>();
	auto& nextSceneArray = systemsNC.getComponentArray<Component::NextScene>();
	auto& transformArray = systemsNC.getComponentArray<Component::Transform>();
	auto& positionArray = systemsNC.getComponentArray<Component::Position>();

	for (auto& [entity, buttonObj] : buttonArray->getAll())
	{
		if (!buttonObj.enabled)
		{
			continue;
		}

		// buttonArray must have a shape, origin, and text
		if (!originArray->hasData(entity) ||
			!positionArray->hasData(entity) ||
			!spriteArray->hasData(entity))
		{
			continue;
		}

		////std::cout << "button.top: " << button.top << "\n";
		////std::cout << "button.left: " << button.left << "\n";
		Component::Origin& originObj = originArray->getData(entity);
		Component::Transform& transformObj = transformArray->getData(entity);
		Component::Position& posObj = positionArray->getData(entity);
		Component::Sprite& spriteObj = spriteArray->getData(entity);

		if (buttonObj.clickedTimer <= 0)
		{
			spriteObj.body->setScale
			(
				sf::Vector2f
				(
					DEFAULT_SCALE_X * (transformObj.width / spriteObj.body->getTexture().getSize().x),
					DEFAULT_SCALE_Y * (transformObj.height / spriteObj.body->getTexture().getSize().y)
				)
			);

			if (textArray->hasData(entity))
			{
				Component::Text& text = textArray->getData(entity);
				text.box->setScale
				(
					sf::Vector2f
					(
						DEFAULT_SCALE_X,
						DEFAULT_SCALE_Y
					)
				);
			}
		}
		else
		{
			buttonObj.clickedTimer -= dt;
			if (buttonObj.clickedTimer <= 0)
			{
				buttonObj.clicked = true;

				if (nextSceneArray->hasData(entity))
				{
					////std::cout << "starting next scene." << "\n";
					Component::NextScene& nextScene = nextSceneArray->getData(entity);
					nextScene.active = true;
				}
			}
		}

		// button hovering
		if (mouseVector.x > posObj.x - originObj.offsetX &&
			mouseVector.x < posObj.x + transformObj.width - originObj.offsetX &&
			mouseVector.y > posObj.y - originObj.offsetY &&
			mouseVector.y < posObj.y + transformObj.height - originObj.offsetY &&
			buttonObj.clickedTimer <= 0)
		{
			spriteObj.body->setScale
			(
				sf::Vector2f
				(
					HOVER_SCALE_X * (transformObj.width / spriteObj.body->getTexture().getSize().x),
					HOVER_SCALE_Y * (transformObj.height / spriteObj.body->getTexture().getSize().y)
				)
			);

			if (textArray->hasData(entity))
			{
				Component::Text& text = textArray->getData(entity);
				text.box->setScale
				(
					sf::Vector2f
					(
						HOVER_SCALE_X,
						HOVER_SCALE_Y
					)
				);
			}
		}

		// button clicking
		if (buttonObj.clickedTimer > 0)
		{
			spriteObj.body->setScale
			(
				sf::Vector2f
				(
					CLICKED_SCALE_X * (transformObj.width / spriteObj.body->getTexture().getSize().x),
					CLICKED_SCALE_Y * (transformObj.height / spriteObj.body->getTexture().getSize().y)
				)
			);

			if (textArray->hasData(entity))
			{
				Component::Text& text = textArray->getData(entity);
				text.box->setScale
				(
					sf::Vector2f
					(
						CLICKED_SCALE_X,
						CLICKED_SCALE_Y
					)
				);
			}
		}
	}
}
void Update::doNextScene
(
	sf::RenderWindow& window,
	sf::Font& font
)
{
	auto& nextSceneArray = systemsNC.getComponentArray<Component::NextScene>();

	bool playNext = false;
	Scene playNextScene;

	for (auto& [entity, nextSceneObj] : nextSceneArray->getAll())
	{
		// buttons must have a shape, origin, and text
		if (nextSceneObj.active)
		{
			//std::cout << "active: " << nextScene.next << "\n";
			playNext = true;
			playNextScene = nextSceneObj.next;
			break;
		}
	}

	if (playNext)
	{
		systemsNC.destroyAll();
		playScene
		(
			window,
			playNextScene,
			font
		);
		window.close();
	}
}
void Update::move(const DeltaTime dt)
{
	auto& velocityArray = systemsNC.getComponentArray<Component::Velocity>();
	auto& positionArray = systemsNC.getComponentArray<Component::Position>();

	for (auto& [entity, velocityObj] : velocityArray->getAll())
	{
		if (!positionArray->hasData(entity))
		{
			continue;
		}

		Component::Position& posObj = positionArray->getData(entity);
		posObj.x += (velocityObj.x * dt);
		posObj.y += (velocityObj.y * dt);
	}
}
void Update::drag(const DeltaTime dt)
{
	auto& velocityArray = systemsNC.getComponentArray<Component::Velocity>();
	auto& dragArray = systemsNC.getComponentArray<Component::Drag>();

	for (auto& [entity, velocityObj] : velocityArray->getAll())
	{
		if (!dragArray->hasData(entity))
		{
			continue;
		}

		Component::Drag dragObj = dragArray->getData(entity);

		// can't be exactly 0.0 because it will drift aimlessly
		velocityObj.x = velocityObj.x < -0.1 ? velocityObj.x + (dragObj.x * dt) :
			velocityObj.x > 0.1f ? velocityObj.x - (dragObj.x * dt) : 0.0;
		velocityObj.y = velocityObj.y < -0.1 ? velocityObj.y + (dragObj.y * dt) :
			velocityObj.y > 0.1f ? velocityObj.y - (dragObj.y * dt) : 0.0;
	}
}
void Update::doBounds
(
	const double top,
	const double bottom,
	const double left,
	const double right
)
{
	auto& positionArray = systemsNC.getComponentArray<Component::Position>();
	auto& velocityArray = systemsNC.getComponentArray<Component::Velocity>();

	for (auto& [entity, pos] : positionArray->getAll())
	{
		if (!velocityArray->hasData(entity)) continue;

		Component::Velocity& velocity = velocityArray->getData(entity);

		if (pos.y < top || pos.y > bottom)
		{
			velocity.y = -velocity.y / 2.0;
		}

		if (pos.x < left || pos.x > right)
		{
			velocity.x = -velocity.x / 2.0;
		}

		pos.y = std::clamp(pos.y, top, bottom);
		pos.x = std::clamp(pos.x, left, right);
	}
}

const double SIDE_MOVE_SPEED = 200.0;
const double SMOOTHNESS_MIDPOINT = 50.0;
const double SMOOTHNESS_GROWTH = 1.0;

void Update::followCamera
(
	const Entity camera,
	const DeltaTime dt,
	sf::RenderWindow& window
)
{
	auto& positionArray = systemsNC.getComponentArray<Component::Position>();
	auto& cameraArray = systemsNC.getComponentArray<Component::Camera>();

	if (!systemsNC.getComponentArray<Component::Camera>()->hasData(camera)) return;

	Component::Camera cameraObj = systemsNC.getComponentArray<Component::Camera>()->getData(camera);

	if (cameraObj.target == NULL_ENTITY) return;

	sf::View view = window.getView();

	if (!systemsNC.getComponentArray<Component::Position>()->hasData(cameraObj.target)) return;

	const Component::Position& targetPos = positionArray->getData(cameraObj.target);

	double moveAmount = 0.f;

	if (targetPos.x < window.getView().getCenter().x)
	{
		moveAmount = -SIDE_MOVE_SPEED;
	}
	else if (targetPos.x > window.getView().getCenter().x)
	{
		moveAmount = SIDE_MOVE_SPEED;
	}

	//std::cout << "distance: " << (targetPos.x - view.getCenter().x) << "\n";

	float smoothness = 1 / (1 + std::exp(-SMOOTHNESS_GROWTH * (std::abs(targetPos.x - view.getCenter().x) - SMOOTHNESS_MIDPOINT)));

	//std::cout << "smoothness: " << smoothness << "\n";

	view.move
	(
		sf::Vector2f
		( 
			(moveAmount * smoothness) * dt,
			0.0
		)
	);

	//std::cout << "currentDist: " << std::clamp(static_cast<double>(view.getCenter().x), cameraObj.min, cameraObj.max) << "\n";

	view.setCenter
	(
		sf::Vector2f
		(
			std::clamp(static_cast<double>(view.getCenter().x), cameraObj.min, cameraObj.max),
			view.getCenter().y
		)
	);

	window.setView(view);
}

void Update::grow
(
	const Entity loadedTextures,
	const DeltaTime dt,
	const Component::Season& season
)
{
	auto& plantTimesArray = systemsNC.getComponentArray<Component::PlantTimes>();
	auto& plantSizesArray = systemsNC.getComponentArray<Component::PlantSizes>();
	auto& plantTexturesArray = systemsNC.getComponentArray<Component::PlantTextures>();
	auto& plantColorsArray = systemsNC.getComponentArray<Component::PlantColors>();
	auto& plantGrowthRateArray = systemsNC.getComponentArray<Component::PlantGrowthRate>();

	auto& textureArray = systemsNC.getComponentArray<Component::Texture>();
	auto& spriteArray = systemsNC.getComponentArray<Component::Sprite>();
	auto& transformArray = systemsNC.getComponentArray<Component::Transform>();
	auto& colorArray = systemsNC.getComponentArray<Component::Color>();
	auto& originArray = systemsNC.getComponentArray<Component::Origin>();
	auto& positionArray = systemsNC.getComponentArray<Component::Position>();

	auto& texturesContainerArray = systemsNC.getComponentArray<Component::TexturesContainer>();

	if (!texturesContainerArray->hasData(loadedTextures))
	{
		return;
	}

	Component::TexturesContainer& containerObj = texturesContainerArray->getData(loadedTextures);

	double growthRate = 0.0;

	for (auto& [entity, plantTimes] : plantTimesArray->getAll())
	{
		if (!plantSizesArray->hasData(entity) ||
			!plantTexturesArray->hasData(entity) ||
			!plantColorsArray->hasData(entity) ||
			!plantGrowthRateArray->hasData(entity) ||
			!textureArray->hasData(entity) ||
			!spriteArray->hasData(entity) ||
			!transformArray->hasData(entity) ||
			!colorArray->hasData(entity) ||
			!originArray->hasData(entity) ||
			!positionArray->hasData(entity)) continue;

		Component::PlantSizes& plantSizes = plantSizesArray->getData(entity);
		Component::PlantTextures& plantTextures = plantTexturesArray->getData(entity);
		Component::PlantColors& plantColors = plantColorsArray->getData(entity);
		const Component::PlantGrowthRate& plantGrowthRate = plantGrowthRateArray->getData(entity);

		switch (season.current)
		{
		case Enum::Season::SPRING:
			growthRate = plantGrowthRate.spring;
			break;
		case Enum::Season::SUMMER:
			growthRate = plantGrowthRate.summer;
			break;
		case Enum::Season::FALL:
			growthRate = plantGrowthRate.fall;
			break;
		case Enum::Season::WINTER:
			growthRate = plantGrowthRate.winter;
			break;
		}

		//std::cout << "growthRate: " << growthRate << "\n";

		Component::Texture& texture = textureArray->getData(entity);
		Component::Sprite& sprite = spriteArray->getData(entity);
		Component::Transform& transform = transformArray->getData(entity);
		Component::Color& color = colorArray->getData(entity);
		Component::Origin& origin = originArray->getData(entity);
		Component::Position& position = positionArray->getData(entity);

		bool grew = false;
		double growthMoveY = 0.0;

		if (plantTimes.seed > 0)
		{
			if (plantTimes.seed - dt <= 0)
			{
				texture.data = plantTextures.seedling;

				growthMoveY = transform.height / 2.0;

				transform.width = plantSizes.seedlingX;
				transform.height = plantSizes.seedlingY;

				color.col = plantColors.seedling;

				grew = true;
				//std::cout << "Plant #" << static_cast<int>(entity) << " has grown to seedling stage." << "\n";
			}

			plantTimes.seed -= (dt * growthRate);
		}
		else if (plantTimes.seedling > 0)
		{
			if (plantTimes.seedling - dt <= 0)
			{
				texture.data = plantTextures.flowering;

				growthMoveY = transform.height / 2.0;

				transform.width = plantSizes.floweringX;
				transform.height = plantSizes.floweringY;

				color.col = plantColors.flowering;

				grew = true;
				//std::cout << "Plant #" << static_cast<int>(entity) << " has grown to flowering stage." << "\n";
			}

			plantTimes.seedling -= (dt * growthRate);
		}
		else if (plantTimes.flowering > 0)
		{
			if (plantTimes.flowering - dt <= 0)
			{
				texture.data = plantTextures.mature;

				growthMoveY = transform.height / 2.0;

				transform.width = plantSizes.matureX;
				transform.height = plantSizes.matureY;

				color.col = plantColors.mature;

				grew = true;
				//std::cout << "Plant #" << static_cast<int>(entity) << " has grown to mature stage." << "\n";
			}

			plantTimes.flowering -= (dt * growthRate);
		}

		if (grew)
		{
			sprite.body.emplace(containerObj.map[texture.data]);

			position.y -= growthMoveY;

			sprite.body->setScale
			(
				sf::Vector2f
				(
					transform.width / sprite.body->getGlobalBounds().size.x,
					transform.height / sprite.body->getGlobalBounds().size.y
				)
			);

			origin.offsetX = transform.width / 2.0;
			origin.offsetY = transform.height / 2.0;

			sprite.body->setOrigin
			(
				sf::Vector2f
				(
					origin.offsetX / sprite.body->getScale().x,
					origin.offsetY / sprite.body->getScale().y
				)
			);

			sprite.body->setColor(color.col);
		}
	}
}

void Update::deleteEntities(DeltaTime dt)
{
	std::vector<Entity> deleteQueue{};

	for (auto& [entity, deleteObj] : systemsNC.getComponentArray<Component::Delete>()->getAll())
	{
		if (deleteObj.timer <= 0.0)
		{
			deleteQueue.push_back(entity);
			continue;
		}

		deleteObj.timer -= dt;
	}

	for (Entity entity : deleteQueue)
	{
		systemsNC.deleteEntity(entity);
	}
}

// -------------------------------------------------------
// rendering systems
// -------------------------------------------------------
void Render::alignZIndexWithYAxis
(
	const double top,
	const double bottom,
	const Entity player
)
{
	auto& positionArray = systemsNC.getComponentArray<Component::Position>();
	auto& yAxisAddArray = systemsNC.getComponentArray<Component::YAxisAdd>();

	for (const auto& [entity, zIndex] : systemsNC.getComponentArray<Component::ZIndex>()->getAll())
	{
		if (!positionArray->hasData(entity) ||
			!yAxisAddArray->hasData(entity)) continue;

		Component::YAxisAdd& yAxisAdd = yAxisAddArray->getData(entity);
		const Component::Position pos = positionArray->getData(entity);
		const Component::Transform trans = systemsNC.getComponentArray<Component::Transform>()->getData(entity);

		// inverse lerp/ min-max normalization
		double progressRatio = [](double target, double min, double max)
			{
				return (target - min) / (max - min);
			}
		(pos.y + (trans.height / 2.0), top, bottom);

		yAxisAdd.amount = static_cast<int>(progressRatio * (bottom - top));

		//std::cout << "posY: " << pos.y << "\n";
		//std::cout << "progressRatio: " << progressRatio << "\n";
		//std::cout << "yAxisAdd: " << zIndex.yAxisAdd << "\n";
	}
}
void Render::doZIndex(std::queue<Entity>& renderQueue)
{
	auto& zIndexArray = systemsNC.getComponentArray<Component::ZIndex>();
	auto& yAxisAddArray = systemsNC.getComponentArray<Component::YAxisAdd>();

	std::vector<std::pair<int, Entity>> renderVector;
	for (auto& [entity, zIndexObj] : zIndexArray->getAll())
	{
		if (!zIndexObj.visible) continue;

		int finalIndex = zIndexObj.index;

		if (yAxisAddArray->hasData(entity) &&
			yAxisAddArray->getData(entity).enabled)
		{
			Component::YAxisAdd yAxisAdd = yAxisAddArray->getData(entity);
			finalIndex += yAxisAdd.amount;
		}

		renderVector.emplace_back(finalIndex, entity);
	}
	std::sort(renderVector.begin(), renderVector.end());

	for (auto& [zIndex, entity] : renderVector)
	{
		renderQueue.push(entity);
	}
}
void Render::render
(
	sf::RenderWindow& window,
	std::queue<Entity>& renderQueue
)
{
	auto& spriteArray = systemsNC.getComponentArray<Component::Sprite>();
	auto& positionArray = systemsNC.getComponentArray<Component::Position>();
	auto& textArray = systemsNC.getComponentArray<Component::Text>();

	while (!renderQueue.empty())
	{
		Entity& popped = renderQueue.front();
		////std::cout << "popped: " << popped << "\n";

		if (!positionArray->hasData(popped))
		{
			// this means the entity does not have a position component
			continue;
		}

		Component::Position& posObj = positionArray->getData(popped);

		if (spriteArray->hasData(popped))
		{
			Component::Sprite& spriteObj = spriteArray->getData(popped);

			if (spriteObj.body.has_value())
			{
				spriteObj.body->setPosition
				(
					sf::Vector2f
					(
						posObj.x,
						posObj.y
					)
				);
				window.draw(spriteObj.body.value());
			}
		}

		if (textArray->hasData(popped))
		{
			Component::Text& textObj = textArray->getData(popped);

			textObj.box->setPosition
			(
				sf::Vector2f
				(
					posObj.x,
					posObj.y
				)
			);
			window.draw(textObj.box.value());
		}

		renderQueue.pop();
	}
}