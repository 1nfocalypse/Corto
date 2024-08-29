#include "BinNum.h"
#include <stdexcept>
#include <iostream>

// ctor tests

// Test Base initialization

int main() {
	BinNum myNum("00001111");

	try {
		BinNum badStr("211101010101");
	}
	catch (std::invalid_argument& e) {
		std::cout << "Caught expected bad init." << std::endl;
	}

	return 0;
}