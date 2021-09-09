#pragma once

#include "BigNumber.h"

// Stores game data for Hexii (aka interactable hexagons)
struct HexiiData {
	inline HexiiData(BigNumber power = 2.5) :
		power(power)
	{}

	BigNumber power;
};