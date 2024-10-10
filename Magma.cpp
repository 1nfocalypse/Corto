#include "Magma.h"
#include "MagFunctor.h"
#include <bitset>
#include <iostream>
#include <algorithm>
#include <mutex>

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

std::vector<BinNum> Magma::keyScheduler(BinNum key) {
	std::vector<BinNum> keys;
	BinNum lsbMask32("FFFFFFFF", 256, 16);
	for (int i = 7; i >= 0; --i) {
		keys.push_back((key >> (32 * i)) & lsbMask32);
	}
	for (uint32_t i = 0; i < 2; ++i) {
		for (uint32_t j = 0; j < 8; ++j) {
			keys.push_back(keys[j]);
		}
	}
	for (int i = 7; i >= 0; --i) {
		keys.push_back(keys[i]);
	}
	// resize all keys
	for (uint32_t i = 0; i < keys.size(); ++i) {
		keys[i] = BinNum(keys[i].getVal().substr(256 - 32, 32));
	}
	// should now all be 32 bits
	return keys;
}

void Magma::threadEncrypt(std::vector<BinNum> keys, BinNum tarBlock, uint32_t retIndex, std::vector<BinNum>& results, std::mutex& mtx) {
	mtx.lock();
	results[retIndex] = encrypt(tarBlock, keys);
	mtx.unlock();
}

void Magma::threadDecrypt(std::vector<BinNum> keys, BinNum tarBlock, uint32_t retIndex, std::vector<BinNum>& results, std::mutex& mtx) {
	mtx.lock();
	results[retIndex] = decrypt(tarBlock, keys);
	mtx.unlock();
}

BinNum Magma::encrypt(BinNum plaintext, std::vector<BinNum> keys) {
	std::vector<uint32_t> keys32;
	for (BinNum key : keys) {
		keys32.push_back(std::stoul(key.getVal(), nullptr, 2));
	}
	uint64_t block = std::stoull(plaintext.getVal(), nullptr, 2);
	std::vector<uint32_t> seperated = split(block);
	uint32_t left = seperated[0];
	uint32_t right = seperated[1];
	for (uint32_t i = 0; i < 31; ++i) {
		uint32_t newL = right;
		uint32_t newR = left ^ rotation(right, keys32[i]);
		left = newL;
		right = newR;
	}
	uint64_t resultingEnc64 = join(left ^ rotation(right, keys32[31]), right);
	return BinNum(std::bitset<64>(resultingEnc64).to_string());
}

BinNum Magma::decrypt(BinNum ciphertext, std::vector<BinNum> keys) {
	std::vector<uint32_t> keys32;
	for (BinNum key : keys) {
		keys32.push_back(std::stoul(key.getVal(), nullptr, 2));
	}
	uint64_t block = std::stoull(ciphertext.getVal(), nullptr, 2);
	std::vector<uint32_t> seperated = split(block);
	std::reverse(keys32.begin(), keys32.end());
	uint32_t left = seperated[0];
	uint32_t right = seperated[1];
	for (uint32_t i = 0; i < 31; ++i) {
		uint32_t newL = right;
		uint32_t newR = left ^ rotation(right, keys32[i]);
		left = newL;
		right = newR;
	}
	uint64_t resultingDec64 = join(left ^ rotation(right, keys32[31]), right);
	return BinNum(std::bitset<64>(resultingDec64).to_string());
}


void Magma::verify() const {
	MagFunctor()();
}