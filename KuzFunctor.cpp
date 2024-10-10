#include "KuzFunctor.h"
#include "Kuznyechik.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <bitset>
#include <cassert>
#include <list>
#include <thread>
#include <mutex>
#include "BinNum.h"
#include "Streebog.h"

void KuzFunctor::operator()() {
	Kuznyechik crypter;
	std::cout << "Verifying Kuznyechik..." << std::endl;
	std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
	std::cout << "Testing S-box." << std::endl;
	std::cout << "Testing case 1: ";
	assert(BinNum(crypter.sBox(BinNum("ffeeddccbbaa99881122334455667700", 128, 16))) == BinNum("b66cd8887d38e8d77765aeea0c9a7efc", 128, 16));
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 2: ";
	assert(BinNum(crypter.sBox(BinNum("b66cd8887d38e8d77765aeea0c9a7efc", 128, 16))) == BinNum("559d8dd7bd06cbfe7e7b262523280d39", 128, 16));
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 3: ";
	assert(BinNum(crypter.sBox(BinNum("559d8dd7bd06cbfe7e7b262523280d39", 128, 16))) == BinNum("0c3322fed531e4630d80ef5c5a81c50b", 128, 16));
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 4: ";
	assert(BinNum(crypter.sBox(BinNum("0c3322fed531e4630d80ef5c5a81c50b", 128, 16))) == BinNum("23ae65633f842d29c5df529c13f5acda", 128, 16));
	std::cout << "Success." << std::endl;
	std::cout << "S-box encode functional." << std::endl;
	std::cout << "***********************" << std::endl;
	std::cout << "Testing S-box inversion." << std::endl;
	std::cout << "Testing case 1: ";
	assert(BinNum(crypter.sBoxInv(BinNum("b66cd8887d38e8d77765aeea0c9a7efc", 128, 16))) == BinNum("ffeeddccbbaa99881122334455667700", 128, 16));
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 2: ";
	assert(BinNum(crypter.sBoxInv(BinNum("559d8dd7bd06cbfe7e7b262523280d39", 128, 16))) == BinNum("b66cd8887d38e8d77765aeea0c9a7efc", 128, 16));
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 3: ";
	assert(BinNum(crypter.sBoxInv(BinNum("0c3322fed531e4630d80ef5c5a81c50b", 128, 16))) == BinNum("559d8dd7bd06cbfe7e7b262523280d39", 128, 16));
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 4: ";
	assert(BinNum(crypter.sBoxInv(BinNum("23ae65633f842d29c5df529c13f5acda", 128, 16))) == BinNum("0c3322fed531e4630d80ef5c5a81c50b", 128, 16));
	std::cout << "Success." << std::endl;
	std::cout << "S-box decode functional." << std::endl;
	std::cout << "***********************" << std::endl;
	std::cout << "S-box integrity intact." << std::endl;

	std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
	std::cout << "Testing P-box." << std::endl;
	std::cout << "Testing case 1: ";
	assert(BinNum(crypter.pBox(BinNum("00000000000000000000000000000100", 128, 16))) == BinNum("94000000000000000000000000000001", 128, 16));
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 2: ";
	assert(BinNum(crypter.pBox(BinNum("94000000000000000000000000000001", 128, 16))) == BinNum("a5940000000000000000000000000000", 128, 16));
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 3: ";
	assert(BinNum(crypter.pBox(BinNum("a5940000000000000000000000000000", 128, 16))) == BinNum("64a59400000000000000000000000000", 128, 16));
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 4: ";
	assert(BinNum(crypter.pBox(BinNum("64a59400000000000000000000000000", 128, 16))) == BinNum("0d64a594000000000000000000000000", 128, 16));
	std::cout << "Success." << std::endl;
	std::cout << "P-box encode functional." << std::endl;
	std::cout << "***********************" << std::endl;
	std::cout << "Testing P-box inversion." << std::endl;
	std::cout << "Testing case 1: ";
	assert(BinNum(crypter.pBoxInv(BinNum("0d64a594000000000000000000000000", 128, 16))) == BinNum("64a59400000000000000000000000000", 128, 16));
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 2: ";
	assert(BinNum(crypter.pBoxInv(BinNum("64a59400000000000000000000000000", 128, 16))) == BinNum("a5940000000000000000000000000000", 128, 16));
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 3: ";
	assert(BinNum(crypter.pBoxInv(BinNum("a5940000000000000000000000000000", 128, 16))) == BinNum("94000000000000000000000000000001", 128, 16));
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 4: ";
	assert(BinNum(crypter.pBoxInv(BinNum("94000000000000000000000000000001", 128, 16))) == BinNum("00000000000000000000000000000100", 128, 16));
	std::cout << "Success." << std::endl;
	std::cout << "P-box decode functional." << std::endl;
	std::cout << "***********************" << std::endl;
	std::cout << "P-box integrity intact." << std::endl;

	// these are the slow bits
	std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
	std::cout << "Testing P composition." << std::endl;
	std::cout << "Testing case 1: ";
	assert(BinNum(crypter.pBoxWrapper(BinNum("64a59400000000000000000000000000", 128, 16))) == BinNum("d456584dd0e3e84cc3166e4b7fa2890d", 128, 16));
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 2: ";
	assert(BinNum(crypter.pBoxWrapper(BinNum("d456584dd0e3e84cc3166e4b7fa2890d", 128, 16))) == BinNum("79d26221b87b584cd42fbc4ffea5de9a", 128, 16));
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 3: ";
	assert(BinNum(crypter.pBoxWrapper(BinNum("79d26221b87b584cd42fbc4ffea5de9a", 128, 16))) == BinNum("0e93691a0cfc60408b7b68f66b513c13", 128, 16));
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 4: ";
	assert(BinNum(crypter.pBoxWrapper(BinNum("0e93691a0cfc60408b7b68f66b513c13", 128, 16))) == BinNum("e6a8094fee0aa204fd97bcb0b44b8580", 128, 16));
	std::cout << "Success." << std::endl;
	std::cout << "P-box composition functional." << std::endl;
	std::cout << "***********************" << std::endl;
	std::cout << "Testing P composition inversion." << std::endl;
	std::cout << "Testing case 1: ";
	assert(BinNum(crypter.pBoxInvWrapper(BinNum("e6a8094fee0aa204fd97bcb0b44b8580", 128, 16))) == BinNum("0e93691a0cfc60408b7b68f66b513c13", 128, 16));
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 2: ";
	assert(BinNum(crypter.pBoxInvWrapper(BinNum("0e93691a0cfc60408b7b68f66b513c13", 128, 16))) == BinNum("79d26221b87b584cd42fbc4ffea5de9a", 128, 16));
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 3: ";
	assert(BinNum(crypter.pBoxInvWrapper(BinNum("79d26221b87b584cd42fbc4ffea5de9a", 128, 16))) == BinNum("d456584dd0e3e84cc3166e4b7fa2890d", 128, 16));
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 4: ";
	assert(BinNum(crypter.pBoxInvWrapper(BinNum("d456584dd0e3e84cc3166e4b7fa2890d", 128, 16))) == BinNum("64a59400000000000000000000000000", 128, 16));
	std::cout << "Success." << std::endl;
	std::cout << "P-box composition functional." << std::endl;
	std::cout << "***********************" << std::endl;
	std::cout << "P-box composition integrity intact." << std::endl;

	std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
	std::cout << "Testing key scheduler." << std::endl;
	std::cout << "Generating keys..." << std::endl;
	std::vector<BinNum> keys = crypter.keyScheduler(BinNum("8899aabbccddeeff0011223344556677fedcba98765432100123456789abcdef", 256, 16));
	std::cout << "Done." << std::endl;
	std::cout << "Testing case 1: ";
	assert(BinNum("8899aabbccddeeff0011223344556677", 128, 16) == keys[0]);
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 2: ";
	assert(BinNum("fedcba98765432100123456789abcdef", 128, 16) == keys[1]);
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 3: ";
	assert(BinNum("db31485315694343228d6aef8cc78c44", 128, 16) == keys[2]);
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 4: ";
	assert(BinNum("3d4553d8e9cfec6815ebadc40a9ffd04", 128, 16) == keys[3]);
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 5: ";
	assert(BinNum("57646468c44a5e28d3e59246f429f1ac", 128, 16) == keys[4]);
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 6: ";
	assert(BinNum("bd079435165c6432b532e82834da581b", 128, 16) == keys[5]);
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 7: ";
	assert(BinNum("51e640757e8745de705727265a0098b1", 128, 16) == keys[6]);
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 8: ";
	assert(BinNum("5a7925017b9fdd3ed72a91a22286f984", 128, 16) == keys[7]);
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 9: ";
	assert(BinNum("bb44e25378c73123a5f32f73cdb6e517", 128, 16) == keys[8]);
	std::cout << "Success." << std::endl;
	std::cout << "Testing case 10: ";
	assert(BinNum("72e9dd7416bcf45b755dbaa88e4a4043", 128, 16) == keys[9]);
	std::cout << "Success." << std::endl;
	std::cout << "Keygen functional." << std::endl;
	std::cout << "***********************" << std::endl;
	std::cout << "Key scheduler integrity intact." << std::endl;

	std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
	std::cout << "Testing encryption." << std::endl;
	std::cout << "Plaintext: 1122334455667700ffeeddccbbaa9988" << std::endl;
	std::cout << "Key: 8899aabbccddeeff0011223344556677fedcba98765432100123456789abcdef" << std::endl;
	std::cout << "Testing case: ";
	assert(crypter.encrypt(BinNum("1122334455667700ffeeddccbbaa9988", 128, 16), keys) == BinNum("7f679d90bebc24305a468d42b9d4edcd", 128, 16));
	std::cout << "Success." << std::endl;
	std::cout << "Encryption functional." << std::endl;
	std::cout << "***********************" << std::endl;
	std::cout << "Encryption integrity intact." << std::endl;

	std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
	std::cout << "Testing decryption." << std::endl;
	std::cout << "Ciphertext: 7f679d90bebc24305a468d42b9d4edcd" << std::endl;
	std::cout << "Key: 8899aabbccddeeff0011223344556677fedcba98765432100123456789abcdef" << std::endl;
	std::cout << "Testing case: ";
	assert(crypter.decrypt(BinNum("7f679d90bebc24305a468d42b9d4edcd", 128, 16), keys) == BinNum("1122334455667700ffeeddccbbaa9988", 128, 16));
	std::cout << "Success." << std::endl;
	std::cout << "Decryption functional." << std::endl;
	std::cout << "***********************" << std::endl;
	std::cout << "Decryption integrity intact." << std::endl;
	std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
	std::cout << "Kuznyechik implementation verified to GOST specifications." << std::endl;


	std::vector<BinNum> knownGoodResults; // kgr is cleartext
	Streebog str;
	std::list<std::string> myList = { "00010101" };
	std::string hashRes = str.hash(myList, 512);
	for (uint32_t i = 0; i < 8; ++i) {
		knownGoodResults.push_back(BinNum(hashRes.substr(i * 8, 32), 128, 16));
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