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
		BARLEY_SEED,
		CORN_SEED,
		SUNFLOWER_SEED
	};
}

#endif