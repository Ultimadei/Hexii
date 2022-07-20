#pragma once

#include <mpir/mpirxx.h>
#include <memory>

typedef mpz_class BigInt;
typedef mpf_class BigNumber;
typedef std::shared_ptr<BigNumber> SPBigNumber;
typedef std::weak_ptr<BigNumber> WPBigNumber;

extern std::string formatScientificString(const std::string& string);
// Display number as a string with `sf` significant figures
extern std::string bigNumberToString(const BigNumber& number, unsigned int sf = 3, bool fixed = true);