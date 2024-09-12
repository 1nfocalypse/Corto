#include "Streebog.h"
#include <string>
#include <bitset>
#include <stdexcept>
#include <vector>

// 1 is 512, 0 is 256
BinNum Streebog::hash(BinNum number, int size) const {
	if (size != 512 && size != 256) {
		throw std::invalid_argument("Passed invalid hash size. Must be 512 or 256.");
	}
	BinNum hotel("0", 512, 2);
	if (size == 256) {
		hotel = this->initNum256;
	}
	else {
		hotel = this->initNum512;
	}
	BinNum november("0", 512, 2);
	BinNum sigma("0", 512, 2);
	BinNum message("0", 512, 2);
	if (number.getSize() >= 512) {
		// implement stage 3
	}
	// key generation
	std::vector<BinNum> keys;
	BinNum key1 = precompShift(pBox(sBox(hotel ^ november)));
	keys.push_back(key1);
	for (uint32_t i = 0; i < 12; ++i) {
		BinNum key = precompShift(pBox(sBox(keys[i] ^ BinNum(std::bitset<8>(this->C[i]).to_string(), 512, 2))));
		keys.push_back(key);
	}
	// calculate |M|
	BinNum messageCpy = message;
	uint32_t numBits;
	BinNum zero512("0", 512);
	while (messageCpy != zero512) {
		++numBits;
		messageCpy = messageCpy >> 1;
	}
	uint32_t initLen = 511 - numBits;
	std::string preText(initLen, '0');
	preText += '1';
	std::string postText(message.getVal().substr(512 - numBits, numBits));
	std::string messageStr = preText + postText;
	messageCpy.set(messageStr);
	hotel = roundFunct(N, h, m); // must implement later
	november = BinNum(n, 512, 10) + BinNum(std::bitset<8>(numBits).to_string(), 512, 2);
	sigma = sigma + m;
	hotel = roundFunct(0, hotel, N);
	BinNum endVal(roundFunc(0, h, sigma));
	if (size == 256) {
		std::string hash256 = endVal.getVal().substr(0, 256);
		endVal.set(hash256);
		endVal.setBitSize(256);
	}
	return endVal;
}

// seems correct
BinNum Streebog::sBox(BinNum number) const { // S in Stree spec
	BinNum mask("FF", 512, 16);
	mask = mask << 504;
	std::string curStr;
	for (uint32_t i = 0; i < 64; ++i) {
		BinNum curNum = number & mask;
		std::string curByte = curNum.getVal().substr((i * 8), 8);
		uint8_t index = std::stoi(BinNum(curByte, 8).getVal(10));
		uint8_t transformVal = this->transformMatrix[index];
		curStr.append(std::bitset<8>(transformVal).to_string());
		mask = mask >> 8;
	}
	return BinNum(curStr, 512);
}

// we take a 256 bit input
// for each iteration, we append the result of 
// accessing the 256 bit input at the tau[i]'th byte
BinNum Streebog::pBox(BinNum number) const { // P in Stree spec
	std::string curStr;
	for (uint8_t i = 0; i < 64; ++i) {
		uint32_t byteNum = this->tau[i];
		uint32_t accessPoint = byteNum * 8;
		std::string subString = number.getVal().substr(accessPoint, 8);
		curStr.append(subString);
	}
	return BinNum(curStr, 512);
}

BinNum Streebog::precompShift(BinNum number) const { // L in Stree spec
	// parse as 64 bit values
	uint32_t constants[8] = { 7, 15, 23, 31, 39, 47, 55, 63 };
	uint64_t varArr[8];
	for (uint32_t i = 0; i < 8; ++i) {
		varArr[i] = std::stoull(number.getVal().substr(i * 64, 64), nullptr, 2);
	}
	for (uint32_t i = 0; i < 8; ++i) {
		uint64_t tmp = 0;
		for (uint32_t j = 0; j < 8; ++j) {
			tmp = tmp ^ this->A[j][varArr[constants[i] - j]];
		}
		varArr[i] = tmp;
	}
	std::string retStr;
	for (uint32_t i = 0; i < 8; ++i) {
		retStr += itoa(varArr[i], nullptr, 2);
	}
	return BinNum(retStr, 512, 2);
}

BinNum Streebog::xorTransform(BinNum number1, BinNum number2) const {
	return number1 ^ number2;
}


// this is equivalent to
// E(LPS(h XOR N),m) XOR h XOR m
// where LPS(h XOR N) is = K
// we then utilize X[K_13]LPSX[K+12]...LPSX[K_1](m)
// values K_i \in 512 bitlength space are calculated as
// 
BinNum Streebog::roundFunct(uint32_t rounds, BinNum curHash, BinNum message) const {

}
