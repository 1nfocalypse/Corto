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

	return 0;
}