#pragma once
#include <cstdint>
#include <vector>
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
public:

	uint32_t substitution(uint32_t value);

	uint32_t rotation(uint32_t x, uint32_t k);

	std::vector<uint32_t> split(uint64_t value);

	uint64_t join(uint32_t left, uint32_t right);




	// std::vector<uint32_t> keyScheduler(BinNum key);
	std::vector<BinNum> keyScheduler(BinNum key) const;

	std::vector<BinNum> keyScheduler(BinNum key);

	/*uint64_t test_encrypt(uint64_t plaintext, BinNum key);

	uint64_t test_decrypt(uint64_t ciphertext, BinNum key);*/

	BinNum threadEncrypt();

	BinNum threadDecrypt();

	void verify() const;
};

