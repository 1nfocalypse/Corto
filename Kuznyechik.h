#pragma once
#include <string>
#include <vector>
#include "BinNum.h"

/*
 *     __ __                                  __    _ __  
 *    / //___  ______  ____  __  _____  _____/ /_  (_/ /__
 *   / ,< / / / /_  / / __ \/ / / / _ \/ ___/ __ \/ / //_/
 *  / /| / /_/ / / /_/ / / / /_/ /  __/ /__/ / / / / ,<   
 * /_/ |_\__,_/ /___/_/ /_/\__, /\___/\___/_/ /_/_/_/|_|  
 *                        /____/                          
 *
 *************************************************************************************************************************
 * Kuznyechik, literally translated as "Grasshopper", is a block cipher specified in GOST R 34.12-2015. It operates with 
 * a 128 bit block size and a 256 bit key size. It is a Substitution-Permutation Network consisting of 10 rounds, with a 
 * Feistel Network structure for key scheduling. It uses multiple operations, including polynomial arithmetic, shifts, 
 * and polynomial modulo arithmetic to manipulate the data as it progresses. 
 * 
 * Kuznyechik has a public cryptanalysis where a Meet-in-the-Middle attack was successful on Round 5. This is the same
 * class of cryptanalysis leading to why double and triple DES are not used. By memoizing round results, brute forcing
 * becomes a markedly more effective tactice for breaking the cipher. Additionally, it is suspected that the S-box 
 * utilized in both Streebog (GOST R 34.11-2012) and Kuznyechik was not properly generated, and may be a malicious 
 * component of the cryptosystem. Caution in use is advised as a result. 
 * 
 * Please verify that ANY implementation you use at minimum meets the specifications set forth in GOST R 34.12-2015,
 * available in the repo in Russian (I might translate it, and by me, I mean Google Translate). You can confirm this
 * implementation meets those standards by running
 * ./corto -kuz --verify
 *
 * Music Recommendation: Leotrix & CURE97 - Weedcore (Future Riddim) https://www.youtube.com/watch?v=QPWZWjcCczE
 ************************************************************************************************************************
 */

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

	std::vector<BinNum> keyScheduler(BinNum number) const;




	BinNum test_encrypt(BinNum plaintext, BinNum key);

	BinNum test_decrypt(BinNum ciphertext, BinNum key);

	template <typename T>
	BinNum threadEncrypt();

	template <typename T>
	BinNum threadDecrypt();

	void verify() const;

};

