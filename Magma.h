#pragma once
#include <cstdint>
#include <vector>
#include <mutex>
#include "BinNum.h"

/*
 *     __  ___                               ____   __________ ___________
 *    /  |/  ____ _____ _____ ___  ____ _   / / /  / ____/ __ / ___/_  __/
 *   / /|_/ / __ `/ __ `/ __ `__ \/ __ `/  / / /  / / __/ / / \__ \ / /   
 *  / /  / / /_/ / /_/ / / / / / / /_/ /  / / /  / /_/ / /_/ ___/ // /    
 * /_/  /_/\__,_/\__, /_/ /_/ /_/\__,_/  / / /   \____/\____/____//_/     
 *              /____/                  /_/_/                             
 * 
 *************************************************************************************************************************
 * Magma, also known as GOST prior to 2015, is a block cipher specified in GOST R 34.12-2015. It operates with a 64 bit
 * block size and a 256 bit key size. It is a Feistel Network consisting of 32 rounds, utilizing a S box and a rotational
 * shift as the round function. The key schedule is also incredibly simple - it simply segments the key into eight 
 * subkeys, and uses them four times each, reversing the key order to populate the last 8 slots. Originally, Magma/GOST
 * did not have a specified S-box, as it was to be handed out by the Soviet Union state apparatus and otherwise kept
 * confidential. Naturally, this lead to speculation regarding the provided S-boxes, with many assuming that the state
 * gave organizations it intended to spy on weak S-boxes in order to facilitate intelligence gathering. However, with the
 * 2015 update, we have obtained a standardized S-box, which is used here. 
 * 
 * Magma || GOST enjoyed a high level of success for several decades, and is still theoretically secure IFF proper
 * practice is applied. (For example, if you attempt to encrypt 2^32 blocks (about 34 GB) with Magma via the Corto 
 * controller, you will be prompted to rekey your cipher). However, numerous successful attacks exist against the cipher 
 * otherwise, which one should keep in mind when utilizing any cryptographic algorithm.
 * 
 * Please verify that ANY implementation you use at minimum meets the specifications set forth in GOST R 34.12-2015, 
 * available in the repo in Russian (I might translate it, and by me, I mean Google Translate). You can confirm this 
 * implementation meets those standards by running
 * ./corto -mag --verify
 * 
 * Music Recommendation: Aborted Tortoise - Do Not Resuscitate (Punk) https://www.youtube.com/watch?v=faJHi_JJzvc
 ************************************************************************************************************************
 */

class Magma
{
	friend class MagFunctor;
private:
	uint64_t pi[8][16] = {
		{12, 4, 6, 2, 10, 5, 11, 9, 14, 8, 13, 7, 0, 3, 15, 1},
		{6, 8, 2, 3, 9, 10, 5, 12, 1, 14, 4, 7, 11, 13, 0, 15},
		{11, 3, 5, 8, 2, 15, 10, 13, 14, 1, 7, 4, 12, 9, 6, 0},
		{12, 8, 2, 1, 13, 4, 15, 6, 7, 0, 10, 5, 3, 14, 9, 11},
		{7, 15, 5, 10, 8, 1, 6, 13, 0, 9, 3, 14, 11, 4, 2, 12},
		{5, 13, 15, 6, 9, 2, 12, 10, 11, 7, 8, 1, 4, 3, 14, 0},
		{8, 14, 2, 5, 6, 9, 1, 12, 15, 4, 11, 0, 13, 10, 3, 7},
		{1, 7, 14, 13, 0, 5, 8, 3, 4, 15, 10, 6, 9, 12, 11, 2}
	};

	// substitution(uint32_t value)
	// PRE: 32 bit unsigned integer passed
	// POST: Substitutes each nibble according to S box pi
	// WARNINGS: None
	uint32_t substitution(uint32_t value);

	// rotation(uint32_t x, uint32_t k)
	// PRE: 2 32 bit uint32_t passed
	// POST: performs round function (binary rot11) and s box for each block
	// WARNINGS: None
	uint32_t rotation(uint32_t x, uint32_t k);

	// split(uint64_t value)
	// PRE: 64 bit unsigned integer passed
	// POST: Vector of 2 uint32_t returned, first being the MSB and second being the LSB
	// WARNINGS: None
	std::vector<uint32_t> split(uint64_t value);

	// join(uint32_t left, uint32_t right)
	// PRE: 2 32 bit uint32_t passed
	// POST: 64 bit int returned, left being MSB and right being LSB
	// WARNINGS: None
	uint64_t join(uint32_t left, uint32_t right);

public:

	// keyScheduler(BinNum key)
	// PRE: 256 bit BinNum provided as key
	// POST: 32 keys returned in vector as BinNums
	// WARNINGS: None
	std::vector<BinNum> keyScheduler(BinNum key);

	// threadEncrypt(std::vector<BinNum> keys, BinNum tarBlock, uint32_t retIndex, std::vector<BinNum> &results, std::mutex &mtx)
	// PRE: keys generated from keyScheduler, target block, the index of where to store the results, a reference to the results vector, and a referenced mutex passed
	// POST: Applies result of encryption to results[retIndex]
	// WARNINGS: None
	void threadEncrypt(std::vector<BinNum> keys, BinNum tarBlock, uint32_t retIndex, std::vector<BinNum> &results, std::mutex &mtx);

	// threadEncrypt(std::vector<BinNum> keys, BinNum tarBlock, uint32_t retIndex, std::vector<BinNum> &results, std::mutex &mtx)
	// PRE: keys generated from keyScheduler, target block, the index of where to store the results, a reference to the results vector, and a referenced mutex passed
	// POST: Applies result of decryption to results[retIndex]
	// WARNINGS: None
	void threadDecrypt(std::vector<BinNum> keys, BinNum tarBlock, uint32_t retIndex, std::vector<BinNum>& results, std::mutex &mtx);

	// encrypt(BinNum plaintext, std::vector<BinNum> keys)
	// PRE: 64 bit BinNum, keys passed
	// POST: 64 bit BinNum ciphertext returned
	// WARNINGS: None
	BinNum encrypt(BinNum plaintext, std::vector<BinNum> keys);

	// decrypt(BinNum ciphertext, std::vector<BinNum> keys)
	// PRE: 64 bit BinNum, keys passed
	// POST: 64 bit BinNum cleartext returned
	// WARNINGS: None
	BinNum decrypt(BinNum ciphertext, std::vector<BinNum> keys);

	// verify() const
	// PRE: Verification argument provided
	// POST: Calls verification functor, demonstrating compliance with GOST R 34.12-2015 testing standards.
	// WARNINGS: None
	void verify() const;
};

