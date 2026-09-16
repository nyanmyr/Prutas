#ifndef ENUMS_HPP
#define ENUMS_HPP

namespace Enum
{
	enum TextFormat
	{
		TOP,
		MIDDLE,
		BOTTOM
	};

	enum Texture
	{
		TEXTURE_PLACEHOLDER
	};

	enum Item
	{
		NULL_ITEM, // basically means it can't be picked up
		POTATO_SEED,
		WHEAT_SEED,
		BARLEY_SEED,
		CARROT_SEED,
		CORN_SEED,
		SUNFLOWER_SEED
	};
}

#endif