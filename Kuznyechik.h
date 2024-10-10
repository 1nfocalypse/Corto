#pragma once
#include <string>
#include <vector>
#include <mutex>
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
 * becomes a markedly more effective tactice for breaking the cipher (albeit still considered "infeasible" due to the 
 * high computational cost). Additionally, it is suspected that the S-box utilized in both Streebog (GOST R 34.11-2012) 
 * and Kuznyechik was not properly generated, and may be a malicious component of the cryptosystem. Caution in use is 
 * advised as a result. 
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
	friend class KuzFunctor;
private:

	// sBox(BinNum number)
	// PRE: 128 bit BinNum passed
	// POST: 128 bit BinNum with substituted bytes returned
	// WARNINGS: None
	BinNum sBox(BinNum number);

	// sBoxInv(BinNum number)
	// PRE: 128 bit BinNum passed
	// POST: 128 bit BinNum with original bytes returned
	// WARNINGS: None
	BinNum sBoxInv(BinNum number);

	// pBoxWrapper(BinNum number)
	// PRE: 128 bit BinNum passed
	// POST: Convoluted 128 bit BinNum returned (pBox applied 16 times)
	// WARNINGS: None
	BinNum pBoxWrapper(BinNum number);

	// pBoxInvWrapper(BinNum number)
	// PRE: 128 bit BinNum passed
	// POST: Deconvoluted 128 bit BinNum returned (pBoxInv applied 16 times)
	// WARNINGS: None
	BinNum pBoxInvWrapper(BinNum number);

	// pBox(BinNum number)
	// PRE: 128 bit BinNum passed
	// POST: 128 bit BinNum returned after linear function and shift applied
	// WARNINGS: None
	BinNum pBox(BinNum number);

	// pBoxInv(BinNum number)
	// PRE: 128 bit BinNum passed
	// POST: Original 128 bit BinNum returned after linear function and shift applied
	// WARNINGS: None
	BinNum pBoxInv(BinNum number);

	// multBinPolys(BinNum x, BinNum y)
	// PRE: Two 128 bit BinNums are passed
	// POST: Returns resulting product treating numbers as polynomials
	// WARNINGS: None
	BinNum multBinPolys(BinNum x, BinNum y);

	// modBinPolys(BinNum x, BinNum y)
	// PRE: Two 128 bit BinNums are passed
	// POST: Returns x mod y, treating x,y as polynomials
	// WARNINGS: None
	BinNum modBinPolys(BinNum x, BinNum y);

	// linearFunct(BinNum number)
	// PRE: 128 bit BinNum passed
	// POST: 8 bit result returned as uint8_t
	// WARNINGS: None
	uint8_t linearFunct(BinNum number);

	// kuzMult(uint8_t x, uint8_t y)
	// PRE: Two uint8_t's passed
	// POST: uint8_t product of Kuznyechik multiplication returned
	// WARNINGS: None
	uint8_t kuzMult(uint8_t x, uint8_t y);

	// curBits(BinNum number);
	// PRE: 128 bit BinNum passed
	// POST: Returns minimum number of bits required to express the passed number
	// WARNINGS: None
	uint8_t curBits(BinNum number);

public:

	// keyScheduler(BinNum key)
	// PRE: 256 bit BinNum provided as key
	// POST: 10 keys returned in vector as BinNums
	// WARNINGS: None
	std::vector<BinNum> keyScheduler(BinNum number);

	// encrypt(BinNum plaintext, std::vector<BinNum> keys)
	// PRE: 128 bit BinNum, keys passed
	// POST: 128 bit BinNum ciphertext returned
	// WARNINGS: None
	BinNum encrypt(BinNum plaintext, std::vector<BinNum> keys);

	// decrypt(BinNum plaintext, std::vector<BinNum> keys)
	// PRE: 128 bit BinNum, keys passed
	// POST: 128 bit BinNum cleartext returned
	// WARNINGS: None
	BinNum decrypt(BinNum plaintext, std::vector<BinNum> keys);

	// threadEncrypt(std::vector<BinNum> keys, BinNum tarBlock, uint32_t retIndex, std::vector<BinNum> &results, std::mutex &mtx)
	// PRE: keys generated from keyScheduler, target block, the index of where to store the results, a reference to the results vector, and a referenced mutex passed
	// POST: Applies result of encryption to results[retIndex]
	// WARNINGS: None
	void threadEncrypt(std::vector<BinNum> keys, BinNum tarBlock, uint32_t retIndex, std::vector<BinNum>& results, std::mutex& mtx);

	// threadEncrypt(std::vector<BinNum> keys, BinNum tarBlock, uint32_t retIndex, std::vector<BinNum> &results, std::mutex &mtx)
	// PRE: keys generated from keyScheduler, target block, the index of where to store the results, a reference to the results vector, and a referenced mutex passed
	// POST: Applies result of decryption to results[retIndex]
	// WARNINGS: None
	void threadDecrypt(std::vector<BinNum> keys, BinNum tarBlock, uint32_t retIndex, std::vector<BinNum>& results, std::mutex& mtx);

	// verify() const
	// PRE: Verification argument provided
	// POST: Calls verification functor, demonstrating compliance with GOST R 34.12-2015 testing standards.
	// WARNINGS: None
	void verify() const;

};

