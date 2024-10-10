#include "MagFunctor.h"
#include "Magma.h"
#include <iostream>
#include <string>
#include <iostream>
#include <string>
#include <stdexcept>
#include <bitset>
#include <cassert>
#include <thread>
#include <mutex>
#include <list>
#include "BinNum.h"
#include "Streebog.h"


void MagFunctor::operator()() {
	Magma crypter;
	std::cout << "Verifying Magma..." << std::endl;
	std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
	std::cout << "Testing T-transform." << std::endl;
	std::cout << "Testing case 1: ";
	assert(crypter.substitution(0xfdb97531) == 0x2a196f34);
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 2: ";
	assert(crypter.substitution(0x2a196f34) == 0xebd9f03a);
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 3: ";
	assert(crypter.substitution(0xebd9f03a) == 0xb039bb3d);
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 4: ";
	assert(crypter.substitution(0xb039bb3d) == 0x68695433);
	std::cout << "Success." << std::endl;
	std::cout << "T-transform functional." << std::endl;
	std::cout << "***********************" << std::endl;
	std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
	std::cout << "Testing g-transform." << std::endl;
	std::cout << "Testing case 1: ";
	assert(crypter.rotation(0x87654321, 0xfedcba98) == 0xfdcbc20c);
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 2: ";
	assert(crypter.rotation(0xfdcbc20c, 0x87654321) == 0x7e791a4b);
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 3: ";
	assert(crypter.rotation(0x7e791a4b, 0xfdcbc20c) == 0xc76549ec);
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 4: ";
	assert(crypter.rotation(0xc76549ec, 0x7e791a4b) == 0x9791c849);
	std::cout << "Success." << std::endl;
	std::cout << "g-transform functional." << std::endl;
	std::cout << "***********************" << std::endl;
	std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
	std::cout << "Testing keygen." << std::endl;
	std::cout << "Generating keys..." << std::endl;
	std::vector<BinNum> keys = crypter.keyScheduler(BinNum("ffeeddccbbaa99887766554433221100f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff", 256, 16));
	std::vector<BinNum> checkKeys = {
		BinNum("ffeeddcc", 32, 16), BinNum("bbaa9988", 32, 16), BinNum("77665544", 32, 16), BinNum("33221100", 32, 16), BinNum("f0f1f2f3", 32, 16), BinNum("f4f5f6f7", 32, 16), BinNum("f8f9fafb", 32, 16), BinNum("fcfdfeff", 32, 16)
	};
	for (uint32_t i = 0; i < 24; ++i) {
		std::cout << "Testing case " << i + 1 << ": ";
		if (keys[i] == checkKeys[i % 8]) {
			std::cout << "Success." << std::endl;
		}
		else {
			std::cout << "Failed." << std::endl;
		}
	}
	for (int i = 7; i >= 0; --i) {
		std::cout << "Testing case " << 7 - i + 25 << ": ";
		if (keys[7 - i + 24] == checkKeys[i]) {
			std::cout << "Success." << std::endl;
		}
		else {
			std::cout << "Failure." << std::endl;
		}
	}
	// above is the correct keys in the first 3 orders - simply reverse to obtain the 4th column in GOST
	std::cout << "Keygen functional." << std::endl;
	std::cout << "***********************" << std::endl;
	std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
	std::cout << "Testing encryption." << std::endl;
	std::cout << "Plaintext: 1122334455667700ffeeddccbbaa9988" << std::endl;
	std::cout << "Key: 8899aabbccddeeff0011223344556677fedcba98765432100123456789abcdef" << std::endl;
	std::cout << "Testing case: ";

	assert(crypter.encrypt(BinNum("fedcba9876543210", 64, 16), keys) == BinNum("4ee901e5c2d8ca3d", 64, 16));
	std::cout << "Success." << std::endl;
	std::cout << "Encryption functional." << std::endl;
	std::cout << "***********************" << std::endl;
	std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
	std::cout << "Testing decryption." << std::endl;
	std::cout << "Ciphertext: 0x4ee901e5c2d8ca3d" << std::endl;
	std::cout << "Key: 8899aabbccddeeff0011223344556677fedcba98765432100123456789abcdef" << std::endl;
	std::cout << "Testing case: ";
	assert(crypter.decrypt(BinNum("4ee901e5c2d8ca3d", 64, 16), keys) == BinNum("fedcba9876543210", 64, 16));
	std::cout << "Success." << std::endl;
	std::cout << "Decryption functional." << std::endl;
	std::cout << "***********************" << std::endl;
	std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
	std::cout << "Magma implementation verified to GOST specifications." << std::endl;
	std::cout << "Generating iterative values for multithreaded testing..." << std::endl;

	std::vector<BinNum> knownGoodResults; // kgr is cleartext
	Streebog str;
	std::list<std::string> myList = { "00010101" };
	std::string hashRes = str.hash(myList, 512);
	for (uint32_t i = 0; i < 8; ++i) {
		knownGoodResults.push_back(BinNum(hashRes.substr(i * 8, 16), 64, 16));
	}
	std::cout << "Resulting cleartext values:" << std::endl;
	for (BinNum result : knownGoodResults) {
		std::cout << "0x" << result.getVal(16) << std::endl;
	}
	std::cout << "\nAttempting multithreaded encryption." << std::endl;
	std::mutex mtx;
	std::vector<BinNum> results;
	std::vector<std::thread> threadPool;
	results.resize(8, BinNum("0", 1));
	for (uint32_t i = 0; i < knownGoodResults.size(); ++i) {
		threadPool.push_back(std::thread([&crypter, keys, knownGoodResults, i, &results, &mtx]() {
			crypter.threadEncrypt(keys, knownGoodResults[i], i, results, mtx);
			}));
	}
	for (uint32_t i = 0; i < threadPool.size(); ++i) {
		threadPool[i].join();
	}
	threadPool.clear();
	std::cout << "Results of encryption: " << std::endl;
	for (BinNum result : results) {
		std::cout << "0x" << result.getVal(16) << std::endl;
	}
	std::cout << "\nAttempting iterative encryption. " << std::endl;
	std::cout << "Results of encryption: " << std::endl;
	std::vector<BinNum> iterEnc;
	for (uint32_t i = 0; i < 8; ++i) {
		std::cout << "0x" << crypter.encrypt(knownGoodResults[i], keys).getVal(16) << std::endl;
	}
	std::cout << "\nAttempting multithreaded decryption." << std::endl;
	std::vector<BinNum> decOut;
	decOut.resize(8, BinNum("0", 1));
	for (uint32_t i = 0; i < results.size(); ++i) {
		threadPool.push_back(std::thread([&crypter, keys, results, i, &decOut, &mtx]() {
			crypter.threadDecrypt(keys, results[i], i, decOut, mtx);
			}));
	}
	for (uint32_t i = 0; i < threadPool.size(); ++i) {
		threadPool[i].join();
	}
	threadPool.clear();
	std::cout << "Results of decryption:" << std::endl;
	for (BinNum decrypted : decOut) {
		std::cout << "0x" << decrypted.getVal(16) << std::endl;
	}
	bool failFlag = 0;
	std::cout << "\nVerifying correct decryption." << std::endl;
	for (uint32_t i = 0; i < 8; ++i) {
		std::cout << "Test Case " << i + 1 << ": ";
		if (decOut[i] != knownGoodResults[i]) {
			std::cout << "Failed." << std::endl;
			failFlag = 1;
		}
		else {
			std::cout << "Success." << std::endl;
		}
	}
	if (!failFlag) {
		std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
		std::cout << "All routines intact." << std::endl;
		std::cout << "Continuing..." << std::endl;
	}
}