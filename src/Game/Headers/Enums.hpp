#ifndef ENUMS_HPP
#define ENUMS_HPP

namespace Enum
{
	enum class TextFormat
	{
		TOP,
		MIDDLE,
		BOTTOM
	};

	enum class Texture
	{
		TEXTURE_PLACEHOLDER
	};

	enum class Item
	{
		NULL_ITEM, // basically means it can't be picked up
		POTATO,
		CARROT,
		WHEAT_SEED,
		WHEAT,
		BARLEY_SEED,
		BARLEY,
		CORN_SEED,
		CORN,
		SUNFLOWER_SEED,
		SUNFLOWER
	};

	enum class PlayerState
	{
		IDLE,
		OPENED_INVENTORY
	};
}

#endif