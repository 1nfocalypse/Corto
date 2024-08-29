#include <iostream>
#include <string>
#include <stdexcept>
#include <bitset>
#include <cassert>
#include "Kuznyechik.h"
#include "BinNum.h"

int main() {
	Kuznyechik crypter;
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
	std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;

	return 0;
}