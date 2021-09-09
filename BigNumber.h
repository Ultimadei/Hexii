#pragma once

#include <mpir/mpirxx.h>

typedef mpz_class BigInt;
typedef mpf_class BigNumber;

extern std::string formatScientificString(const std::string& string);
// Display number as a string with 'sf' significant figures
extern std::string bigNumberToString(const BigNumber& number, unsigned int sf = 3, bool fixed = true);