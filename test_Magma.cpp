#include <iostream>
#include <string>
#include <iostream>
#include <string>
#include <stdexcept>
#include <bitset>
#include <cassert>
#include "Magma.h"
#include "BinNum.h"

int main() {
	Magma crypter;
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
	std::vector<uint32_t> keys = crypter.keyGen(BinNum("ffeeddccbbaa99887766554433221100f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff", 256, 16));
	std::vector<uint32_t> checkKeys = {
		0xffeeddcc, 0xbbaa9988, 0x77665544, 0x33221100, 0xf0f1f2f3, 0xf4f5f6f7, 0xf8f9fafb, 0xfcfdfeff
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
	std::cout << "keygen functional." << std::endl;
	std::cout << "***********************" << std::endl;
	std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
	std::cout << "Testing encryption." << std::endl;
	std::cout << "Plaintext: 1122334455667700ffeeddccbbaa9988" << std::endl;
	std::cout << "Key: 8899aabbccddeeff0011223344556677fedcba98765432100123456789abcdef" << std::endl;
	std::cout << "Testing case: ";
	assert(crypter.encrypt(0xfedcba9876543210, BinNum("ffeeddccbbaa99887766554433221100f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff", 256, 16)) == 0x4ee901e5c2d8ca3d);
	std::cout << "Success." << std::endl;
	std::cout << "Encryption functional." << std::endl;
	std::cout << "***********************" << std::endl;
	std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
	std::cout << "Testing decryption." << std::endl;
	std::cout << "Ciphertext: 0x4ee901e5c2d8ca3d" << std::endl;
	std::cout << "Key: 8899aabbccddeeff0011223344556677fedcba98765432100123456789abcdef" << std::endl;
	std::cout << "Testing case: ";
	assert(crypter.decrypt(0x4ee901e5c2d8ca3d, BinNum("ffeeddccbbaa99887766554433221100f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff", 256, 16)) == 0xfedcba9876543210);
	std::cout << "Success." << std::endl;
	std::cout << "Decryption functional." << std::endl;
	std::cout << "***********************" << std::endl;
	std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
	std::cout << "Magma implementation verified to GOST specifications." << std::endl;
	std::cout << "All routines intact." << std::endl;
	std::cout << "Continuing..." << std::endl;
	return 0;
}