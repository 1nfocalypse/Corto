#include "BinNum.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <unordered_map>

BinNum::BinNum(std::string bitstr) : myStr(bitstr), size(bitstr.size()) {
	if (bitstr.size() == 0) {
		throw std::invalid_argument("Failed to pass a string.");
	}
	if (bitstr.find_first_not_of("01") != std::string::npos) {
		throw std::invalid_argument("Invalid binary string");
	}
}

BinNum::BinNum(std::string bitstr, uint32_t bitSize) : myStr(bitstr), size(bitSize) {
	if (bitSize == 0) {
		throw std::invalid_argument("Bitstring must have at least one bit.");
	}
	if (myStr.size() > size) {
		throw std::invalid_argument("Bitstring size exceeds declared size.");
	}
	if (bitstr.find_first_not_of("01") != std::string::npos) {
		throw std::invalid_argument("Invalid binary string.");
	}
	if (myStr.length() < size) {
		std::string padStr(size - myStr.length(), '0');
		myStr = padStr + bitstr;
	}
}

//specify bitSize 
BinNum::BinNum(std::string number, uint32_t bitSize, uint32_t base) : myStr(number), size(bitSize), base(base) {
	if (base != 2 && base != 10 && base != 16) {
		throw std::invalid_argument("Only base 2, 10, and 16 supported.");
	}
	if (base == 2 && (number.find_first_not_of("01") != std::string::npos)) {
		throw std::invalid_argument("Invalid binary string.");
	}
	if (base == 10 && (number.find_first_not_of("0123456789") != std::string::npos)) {
		throw std::invalid_argument("Invalid decimal string");
	}
	if (base == 16 && (number.find_first_not_of("0123456789ABCDEFabcdef") != std::string::npos)) {
		throw std::invalid_argument("Invalid hexadecimal string");
	}
	if (base == 10) {
		myStr = decToBin(myStr);
		// size = myStr.size();
		base = 2;
	}
	if (base == 16) {
		myStr = hexToBin(myStr);
		// size = myStr.size();
		base = 2;
	}
	if (size > bitSize && bitSize != 0) {
		throw std::length_error("Insufficient bits provided to hold number " + number + ".");
	}
	if (myStr.length() < bitSize) {
		std::string padStr(bitSize - myStr.length(), '0');
		myStr = padStr + myStr;
	}
}

BinNum::BinNum(const BinNum& other) : myStr(other.getVal()), size(other.getSize()) {}

BinNum& BinNum::operator=(const BinNum& other) {
	if (this != &other) {
		myStr = other.getVal();
		size = other.getSize();
	}
	return *this;
}

BinNum BinNum::AND(const BinNum& other) const {
	std::string otherStr = other.getVal();
	std::string tmpStr = this->myStr;
	std::string retStr;
	if (tmpStr.size() != otherStr.size()) {
		throw std::length_error("Bitstring lengths must be the same.");
	}
	for (uint32_t i = 0; i < this->size; ++i) {
		if (otherStr[i] == '1' && tmpStr[i] == '1') {
			retStr.push_back('1');
		}
		else {
			retStr.push_back('0');
		}
	}
	BinNum retNum(retStr);
	return retNum;
}

BinNum BinNum::OR(const BinNum& other) const {
	std::string otherStr = other.getVal();
	std::string tmpStr = this->myStr;
	std::string retStr;
	if (tmpStr.size() != otherStr.size()) {
		throw std::length_error("Bitstring lengths must be the same.");
	}
	for (uint32_t i = 0; i < this->size; ++i) {
		if (otherStr[i] == '1' || tmpStr[i] == '1') {
			retStr.push_back('1');
		}
		else {
			retStr.push_back('0');
		}
	}
	BinNum retNum(retStr);
	return retNum;
}

BinNum BinNum::XOR(const BinNum& other) const {
	std::string otherStr = other.getVal();
	std::string tmpStr = this->myStr;
	std::string retStr;
	if (tmpStr.size() != otherStr.size()) {
		throw std::length_error("Bitstring lengths must be the same.");
	}
	for (uint32_t i = 0; i < this->size; ++i) {
		if (otherStr[i] == '1' || tmpStr[i] == '1') {
			if (otherStr[i] == '1' && tmpStr[i] == '1') {
				retStr.append("0");
			}
			else {
				retStr.append("1");
			}
		}
		else {
			retStr.append("0");
		}
	}
	BinNum retNum(retStr);
	return retNum;
}

BinNum BinNum::shiftLeft(unsigned int shift) const {
	std::string tmpStr = this->myStr;
	if (shift >= tmpStr.size()) {
		return BinNum(std::string(tmpStr.size(), '0'));
	}
	if (shift == 0) {
		return *this;
	}
	// hold size constant
	std::string retStr = myStr + std::string(shift, '0');
	if (retStr.size() > size) {
		retStr = retStr.substr(shift, size);
	}
	return BinNum(retStr);
}

BinNum BinNum::shiftRight(unsigned int shift) const {
	std::string tmpStr = this->myStr;
	if (shift >= tmpStr.size()) {
		return BinNum(std::string(tmpStr.size(), '0'));
	}
	// hold size constant
	std::string retStr = std::string(shift, '0') + myStr.substr(0, myStr.length() - shift);
	return BinNum(retStr);
}

BinNum BinNum::ADD(const BinNum& other) const {
	std::string tmpStr = this->myStr;
	std::string otherStr = other.getVal();
	std::string retStr;
	bool carryFlag = 0;
	if (tmpStr.size() != otherStr.size()) {
		throw std::length_error("Bitstring lengths must be the same.");
	}
	for (int32_t i = static_cast<int32_t>(tmpStr.length()) - 1; i >= 0; --i) {
		bool a = tmpStr[i] == '1';
		bool b = otherStr[i] == '1';
		retStr.insert(0, (a ^ b ^ carryFlag) ? "1" : "0");
		carryFlag = (a && b) || (carryFlag && (a ^ b));
	}
	if (carryFlag) {
		throw std::overflow_error("Integers added together resulted in carry out past the given size.");
	}
	return BinNum(retStr);
}

BinNum BinNum::SUB(const BinNum& other) const {
	std::string tmpStr = this->myStr;
	std::string otherStr = other.getVal();
	std::string retStr;
	bool borrow = false;
	if (tmpStr.size() != otherStr.size()) {
		throw std::length_error("Bitstring lengths must be the same.");
	}
	if (!this->GEQ(other)) {
		throw std::underflow_error("Passed number is larger than the number it is being subtracted from.");
	}
	for (int32_t i = tmpStr.size() - 1; i >= 0; --i) {
		bool a = (tmpStr[i] == '1');
		bool b = (otherStr[i] == '1');
		if (a < b + borrow) {
			retStr.insert(0, (a + 2 - b - borrow) ? "1" : "0");
			borrow = true;
		}
		else {
			retStr.insert(0, (a - b - borrow) ? "1" : "0");
			borrow = false;
		}
	}
	if (borrow) {
		throw std::underflow_error("Substraction resulted in negative value.");
	}
	return BinNum(retStr);
}

BinNum BinNum::MULT(const BinNum& other) const {
	std::string tmpStr = this->myStr;
	std::string otherStr = other.getVal();
	std::string retStr;
	if (tmpStr.size() != otherStr.size()) {
		throw std::length_error("Bitstring lengths must be the same.");
	}
	std::reverse(otherStr.begin(), otherStr.end());
	BinNum runningNum(std::string(tmpStr.size(), '0'));
	for (int i = 0; i < tmpStr.length(); ++i) {
		if (otherStr[i] == '1') {
			BinNum tmp(tmpStr);
			tmp = tmp.shiftLeft(i);
			try {
				runningNum.set((runningNum.ADD(tmp)).getVal());
			}
			catch (std::overflow_error& e) {
				std::cout << "Error: Addition resulted in a carry-out past allocated size." << std::endl;
			}
		}
	}
	return runningNum;
}

BinNum BinNum::DIV(const BinNum& other) const {
	std::string tmpStr = this->myStr;
	std::string otherStr = other.getVal();
	std::string retStr;
	// generate zero string for comparison
	std::string zero(tmpStr.size(), '0');
	if (other.getVal() == zero) {
		throw std::invalid_argument("Error: Division by zero");
	}
	if (tmpStr.size() != otherStr.size()) {
		throw std::length_error("Bitstring lengths must be the same.");
	}
	BinNum tmp(tmpStr);
	BinNum cnt(std::string(tmpStr.size(), '0'));
	std::string oneStr(tmpStr.size(), '0');
	oneStr[tmpStr.size() - 1] = '1';
	BinNum one(oneStr);
	while (other.LEQ(tmp)) {
		try {
			tmp.set(((tmp.SUB(other)).getVal())); 
			cnt.set((cnt.ADD(one)).getVal());
		}
		catch (std::overflow_error& e) {
			std::cout << "Overflow occured during division." << std::endl;
		}
	}
	return cnt;
}

BinNum BinNum::MOD(const BinNum& other) const {
	std::string tmpStr = this->myStr;
	std::string otherStr = other.getVal();
	std::string retStr;
	if (tmpStr.size() != otherStr.size()) {
		throw std::length_error("Bitstring lengths must be the same.");
	}
	BinNum thisNum(tmpStr);
	BinNum otherNum(otherStr);
	BinNum multiple = thisNum / otherNum;
	return (thisNum - (otherNum * multiple));
}

bool BinNum::GT(const BinNum& other) const {
	std::string tmpStr = this->myStr;
	std::string otherStr = other.getVal();
	if (tmpStr.size() != otherStr.size()) {
		throw std::length_error("Bitstring lengths must be the same.");
	}
	for (size_t i = 0; i < tmpStr.size(); ++i) {
		if (tmpStr[i] > otherStr[i]) {
			return true;
		}
		if (tmpStr[i] < otherStr[i]) {
			return false;
		}
	}
	return false;
}

bool BinNum::LT(const BinNum& other) const {
	std::string tmpStr = this->myStr;
	std::string otherStr = other.getVal();
	if (tmpStr.size() != otherStr.size()) {
		throw std::length_error("Bitstring lengths must be the same.");
	}
	for (uint32_t i = 0; i < tmpStr.size(); ++i) {
		if (tmpStr[i] < otherStr[i]) {
			return true;
		}
		if (tmpStr[i] > otherStr[i]) {
			return false;
		}
	}
	return false;
}

bool BinNum::GEQ(const BinNum& other) const {
	if (this->GT(other) || this->EQ(other)) {
		return true;
	}
	return false;
}

bool BinNum::LEQ(const BinNum& other) const {
	if (this->LT(other) || this->EQ(other)) {
		return true;
	}
	return false;
}

bool BinNum::EQ(const BinNum& other) const {
	return this->myStr == other.myStr;
}

std::string BinNum::decToBin(std::string decStr) const {
	std::string retNum;
	std::string workingNum = decStr;
	std::string binString;

	while (!workingNum.empty()) {
		uint32_t carry = 0;
		// enumerate remainder, append
		if (static_cast<uint32_t>(workingNum[workingNum.length() - 1] - '0') % 2 == 0) {
			binString.push_back('0');
		}
		else {
			binString.push_back('1');
		}
		// divide by two, floored
		std::string divNum = "";
		for (uint32_t i = 0; i < workingNum.length(); ++i) {
			uint32_t current = carry * 10 + (workingNum[i] - '0');
			divNum += (current / 2) + '0';
			carry = current % 2;
		}
		divNum.erase(0, divNum.find_first_not_of('0'));
		workingNum = divNum;
	}
	std::reverse(binString.begin(), binString.end());
	return binString;
}

std::string BinNum::hexToBin(std::string hexStr) const {
	std::string retNum;
	std::vector<std::string> translate = {
		"0000", "0001", "0010", "0011",
		"0100", "0101", "0110", "0111",
		"1000", "1001", "1010", "1011",
		"1100", "1101", "1110", "1111"
	};
	std::unordered_map<char, int> hexToIndex = {
		{'0', 0},{'1', 1},{'2', 2},{'3', 3},
		{'4', 4},{'5', 5},{'6', 6},{'7', 7},
		{'8', 8},{'9', 9},{'A', 10},{'B', 11},
		{'C', 12},{'D', 13},{'E', 14},{'F', 15},
		{'a', 10},{'b', 11},{'c', 12},{'d', 13},
		{'e', 14},{'f', 15}
	};
	for (uint32_t i = 0; i < hexStr.size(); ++i) {
		retNum += translate[hexToIndex[hexStr[i]]];
	}
	return retNum;
}

std::string BinNum::binToDec(std::string bitStr) const {
	std::reverse(bitStr.begin(), bitStr.end());
	std::string retNum = "0";
	std::string curMultiplier = "1";
	for (uint32_t i = 0; i < bitStr.length(); ++i) {
		if (bitStr[i] == '1') {
			std::string addResult;
			uint32_t addCarry = 0;
			uint32_t num1Size = retNum.size();
			uint32_t num2Size = curMultiplier.size();
			int32_t ctr1 = num1Size - 1, ctr2 = num2Size - 1;
			while (ctr1 >= 0 || ctr2 >= 0 || addCarry) {
				uint32_t curSum = addCarry;
				if (ctr1 >= 0) {
					curSum += retNum[ctr1--] - '0';
				}
				if (ctr2 >= 0) {
					curSum += curMultiplier[ctr2--] - '0';
				}
				addResult += static_cast<char>((curSum % 10) + '0');
				addCarry = curSum / 10;
			}
			std::reverse(addResult.begin(), addResult.end());
			retNum = addResult;
		}
		std::string result;
		uint32_t carry = 0;
		for (int32_t j = curMultiplier.length()-1; j >= 0; --j) {
			uint32_t digit = curMultiplier[j] - '0';
			uint32_t prod = digit * 2 + carry;
			result += (prod % 10) + '0';
			carry = prod / 10;
		}
		if (carry) {
			result += carry + '0';
		}
		std::reverse(result.begin(), result.end());
		curMultiplier = result;
	}
	return retNum;
}

std::string BinNum::binToHex(std::string bitStr) const {
	if (bitStr.length() % 4 != 0) {
		throw std::invalid_argument("Provided bitstring must have a length divisible by 4 to convert to base 16.");
	}
	std::string retNum;
	std::vector<std::string> nibbles;
	uint32_t i = 0;
	while (bitStr.size() != i) {
		nibbles.push_back(bitStr.substr(i, 4));
		i += 4;
	}
	std::unordered_map<std::string, char> binToHex = {
		{"0000", '0'},{"0001", '1'},{"0010", '2'},{"0011", '3'},
		{"0100", '4'},{"0101", '5'},{"0110", '6'},{"0111", '7'},
		{"1000", '8'},{"1001", '9'},{"1010", 'A'},{"1011", 'B'},
		{"1100", 'C'},{"1101", 'D'},{"1110", 'E'},{"1111", 'F'}
	};
	for (uint32_t i = 0; i < nibbles.size(); ++i) {
		retNum += binToHex[nibbles[i]];
	}
	return retNum;
}

BinNum BinNum::operator<<(unsigned int shift) const {
	return shiftLeft(shift);
}

BinNum BinNum::operator>>(unsigned int shift) const {
	return shiftRight(shift);
}

BinNum BinNum::operator^(const BinNum& other) const {
	return XOR(other);
}

BinNum BinNum::operator&(const BinNum& other) const {
	return AND(other);
}

BinNum BinNum::operator|(const BinNum& other) const {
	return OR(other);
}

BinNum BinNum::operator+(const BinNum& other) const {
	return ADD(other);
}

BinNum BinNum::operator-(const BinNum& other) const {
	return SUB(other);
}

BinNum BinNum::operator*(const BinNum& other) const {
	return MULT(other);
}

BinNum BinNum::operator/(const BinNum& other) const {
	return DIV(other);
}

BinNum BinNum::operator%(const BinNum& other) const {
	return MOD(other);
}

bool BinNum::operator>(const BinNum& other) const {
	return GT(other);
}

bool BinNum::operator<(const BinNum& other) const {
	return LT(other);
}

bool BinNum::operator>=(const BinNum& other) const {
	return GEQ(other);
}

bool BinNum::operator<=(const BinNum& other) const {
	return LEQ(other);
}

bool BinNum::operator!=(const BinNum& other) const {
	return !(this->EQ(other));
}

bool BinNum::operator==(const BinNum& other) const {
	return this->EQ(other);
}

// this needs to be refined so it can truncate in a defined way
void BinNum::setBitSize(uint32_t bitSize) {
	if (bitSize == 0) {
		throw std::invalid_argument("Bitstring must be more than 0 bits.");
	}
	if (this->getVal().length() < bitSize) {
		std::string padStr(bitSize - this->getVal().length(), '0');
		std::string newBitStr = padStr + this->getVal();
		this->set(newBitStr);
	}
	else {
		throw std::invalid_argument("Cannot truncate strings in this way.");
	}
}

void BinNum::set(std::string newStr) {
	if (newStr.size() == 0) {
		throw std::invalid_argument("Failed to pass a string.");
	}
	if (newStr.find_first_not_of("01") != std::string::npos) {
		throw std::invalid_argument("Invalid binary string");
	}
	this->myStr = newStr;
	this->size = newStr.size(); // think about if this is the behavior you want. resizing may not be the best idea. it may be best to just check if newSize > old size, and reject the changes if newSize is bigger.
}

void BinNum::set(std::string other, uint32_t base) {
	if (other.empty()) {
		throw std::invalid_argument("Faled to pass a string.");
	}
	if (base == 2 && other.find_first_not_of("01") != std::string::npos) {
		throw std::invalid_argument("Invalid binary string.");
	}
	if (base == 10 && other.find_first_not_of("0123456789") != std::string::npos) {
		throw std::invalid_argument("Invalid decimal string.");
	}
	if (base == 16 && other.find_first_not_of("0123456789abdefABCDEF") != std::string::npos) {
		throw std::invalid_argument("Invalid hexadecimal string.");
	}
	if (base == 10) {
		this->myStr = this->decToBin(other);
		this->size = this->myStr.size();
	}
	if (base == 16) {
		this->myStr = this->hexToBin(other);
		this->size = this->myStr.size();
	}
}

// need to add additional support for setters to support hexadecimal and decimal strings

std::string BinNum::getVal() const {
	return this->myStr;
}

std::string BinNum::getVal(uint32_t base) const {
	if (base != 2 && base != 10 && base != 16) {
		throw std::invalid_argument("Base must be 2, 10, or 16.");
	}
	if (base == 2) {
		return this->myStr;
	}
	if (base == 10) {
		return this->binToDec(myStr);
	}
	if (base == 16) {
		return this->binToHex(myStr);
	}
}

uint32_t BinNum::getSize() const {
	return this->size;
}

void BinNum::print() const {
	std::cout << myStr;
}