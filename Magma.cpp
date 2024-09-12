#include "Magma.h"
#include <bitset>

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
	std::vector<uint32_t> null = { 5 };
	return null;
}

uint64_t Magma::join(uint32_t left, uint32_t right) {
	return 0;
}

std::vector<uint32_t> Magma::keyGen(BinNum key) {
	std::vector<uint32_t> null = { 5 };
	return null;
}

uint64_t Magma::encrypt(uint64_t plaintext, BinNum key) {
	return 0;
}

uint64_t Magma::decrypt(uint64_t ciphertext, BinNum key) {
	return 0;
}