#pragma once
#include <string>
#include <vector>
#include "BinNum.h"

class Kuznyechik
{

private:

	/*BinNum sBox(BinNum number);

	BinNum sBoxInv(BinNum number);

	BinNum pBoxWrapper(BinNum number);

	BinNum pBoxInvWrapper(BinNum number);

	BinNum pBox(BinNum number);

	BinNum pBoxInv(BinNum number);

	uint8_t linearFunct(BinNum number);

	uint8_t kuzMult(uint8_t x, uint8_t y);

	uint8_t multBinPolys(uint8_t x, uint8_t y);

	uint8_t modBinPolys(uint8_t x, uint8_t y);

	uint8_t curBits(uint8_t number);

	std::vector<BinNum> keyScheduler(BinNum number);*/

public:

	BinNum sBox(BinNum number);

	BinNum sBoxInv(BinNum number);

	BinNum pBoxWrapper(BinNum number);

	BinNum pBoxInvWrapper(BinNum number);

	BinNum pBox(BinNum number);

	BinNum pBoxInv(BinNum number);

	BinNum multBinPolys(BinNum x, BinNum y);

	BinNum modBinPolys(BinNum x, BinNum y);

	uint8_t linearFunct(BinNum number);

	uint8_t kuzMult(uint8_t x, uint8_t y);

	uint8_t curBits(BinNum number);

	uint8_t curBits(uint8_t number);

	std::vector<BinNum> keyScheduler(BinNum number);




	BinNum encrypt(BinNum plaintext, BinNum key);

	BinNum decrypt(BinNum ciphertext, BinNum key);

	bool integrityCheck();

};

