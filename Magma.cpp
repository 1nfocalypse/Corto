#include "Magma.h"
#include <bitset>
#include <algorithm>

uint32_t Magma::substitution(uint32_t value) {
	uint64_t y = 0;
	for (int i = 7; i >= 0; --i) {
		// shift value 4i right, then grab the 4 lsb
		uint32_t j = (value >> (4 * i)) & 0xF;
		y = y << 4; // shift, since we're functionally appending
		y = y ^ pi[i][j]; // performs substitution
	}
	return static_cast<uint32_t>(y);
}

uint32_t Magma::rotation(uint32_t x, uint32_t k) {
	uint64_t xTmp = x;
	uint64_t kTmp = k;
	uint64_t additive = (x + k) % 0xFFFFFFFF; // mod off overflows
	uint32_t modResult = static_cast<uint32_t>(additive);
	uint32_t substResult = this->substitution(modResult);
	// now perform binary rot 11 per spec transform (15)
	uint64_t lShift = static_cast<uint64_t>(substResult) << 11;
	uint64_t rShift = substResult >> 21;
	uint32_t result = (rShift + lShift) & 0xFFFFFFFF; // mask off last 32
	return result;
}

std::vector<uint32_t> Magma::split(uint64_t value) {
	uint32_t rValue = value & 0xFFFFFFFF;
	uint32_t lValue = static_cast<uint32_t>(value >> 32);
	std::vector<uint32_t> retVals = { lValue, rValue };
	return retVals;
}

uint64_t Magma::join(uint32_t left, uint32_t right) {
	uint64_t leftCpy = left;
	leftCpy = leftCpy << 32;
	leftCpy = leftCpy ^ right;
	return leftCpy;
}

// key is 256 bit binnum
// ret vector is 32 32 bit ints

std::vector<uint32_t> Magma::keyGen(BinNum key) {
	std::vector<uint32_t> keys;
	BinNum lsbMask32("FFFFFFFF", 256, 16);
	for (int i = 7; i >= 0; --i) {
		keys.push_back(std::stoul(((key >> (32 * i)) & lsbMask32).getVal(), nullptr, 2));
	}
	for (uint32_t i = 0; i < 2; ++i) {
		for (uint32_t j = 0; j < 8; ++j) {
			keys.push_back(keys[j]);
		}
	}
	for (int i = 7; i >= 0; --i) {
		keys.push_back(keys[i]);
	}
	return keys;
}

uint64_t Magma::encrypt(uint64_t plaintext, BinNum key) {
	std::vector<uint32_t> keys = keyGen(key);
	std::vector<uint32_t> seperated = split(plaintext);
	// 32 rounds of feistel network
	uint32_t left = seperated[0];
	uint32_t right = seperated[1];
	for (uint32_t i = 0; i < 31; ++i) {
		uint32_t newL = right;
		uint32_t newR = left ^ rotation(right, keys[i]);
		left = newL;
		right = newR;
	}
	return join(left ^ rotation(right, keys[31]), right);
}

uint64_t Magma::decrypt(uint64_t ciphertext, BinNum key) {
	std::vector<uint32_t> keys = keyGen(key);
	std::vector<uint32_t> seperated = split(ciphertext);
	std::reverse(keys.begin(), keys.end());
	uint32_t left = seperated[0];
	uint32_t right = seperated[1];
	for (uint32_t i = 0; i < 31; ++i) {
		uint32_t newL = right;
		uint32_t newR = left ^ rotation(right, keys[i]);
		left = newL;
		right = newR;
	}
	return join(left ^ rotation(right, keys[31]), right);
}