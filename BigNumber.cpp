#include <sstream>
#include <iomanip>

#include "BigNumber.h"

std::string formatScientificString(const std::string& string) {
	std::string result = string;

	std::size_t symbolPos = result.find_first_of('+', 0);
	if (symbolPos == -1) {
		// Not in scientific form, do nothing
		return result;
	}

	// Erase the symbol
	result.erase(symbolPos, 1);

	// Erase all consecutive trailing 0s
	while (result.length() > symbolPos && result[symbolPos] == '0') {
		result.erase(symbolPos, 1);
	}

	return result;
}

std::string bigNumberToString(const BigNumber& number, unsigned int sf, bool fixed) {
	std::stringstream ss;

	if (fixed) { ss << std::scientific; }
	ss << std::setprecision(sf) << number;

	return formatScientificString(ss.str());
}
