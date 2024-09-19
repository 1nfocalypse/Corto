#include <iostream>
#include <string>
#include <stdexcept>
#include <bitset>
#include <cassert>
#include <list>
#include "Streebog.h"
#include "BinNum.h"

int main() {
	Streebog hasher;
	hasher.setH(hasher.initNum512);
	hasher.setN(hasher.initNum512);
	hasher.setSigma(hasher.initNum512);
	BinNum message("01323130393837363534333231303938373635343332313039383736353433323130393837363534333231303938373635343332313039383736353433323130", 512, 16);
	hasher.setInitKey(hasher.precompShift(hasher.pBox(hasher.sBox(hasher.getH() ^ hasher.getN()))));
	assert(hasher.getInitKey() == BinNum("b383fc2eced4a574b383fc2eced4a574b383fc2eced4a574b383fc2eced4a574b383fc2eced4a574b383fc2eced4a574b383fc2eced4a574b383fc2eced4a574", 512, 16));
	std::cout << "Initial key successfully derived." << std::endl;
	// apply E(K,m)
	BinNum eRes = hasher.E(hasher.getInitKey(), message);
	assert(eRes == BinNum("fc221dc8b814fc27a4de079d10097600209e5375776898961f70bded0647bd8f1664cfa8bb8d8ff1e0df3e621568b66aa075064b0e81cce132c8d1475809ebd2", 512, 16));
	std::cout << "E(K, m) produced correct value." << std::endl;
	// apply g_N(h, m)
	BinNum g_NRes = hasher.gN(hasher.getN(), hasher.getH(), message);
	assert(g_NRes == BinNum("fd102cf8812ccb1191ea34af21394f3817a86641445aa9a626488adb33738ebd2754f6908cbbbac5d3ed0f522c50815c954135793fb1f5d905fee4736b3bdae2", 512, 16));
	std::cout << "Successfully derived new h value. Applying..." << std::endl;
	hasher.setH(g_NRes);
	hasher.setN(BinNum("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001f8", 512, 16)); 
	hasher.setSigma(message);
	BinNum g_0NRes = hasher.gN(hasher.initNum512, hasher.getH(), hasher.getN()); // TURNS OUT G_N IS MISLEADING - DO NOT BELIEVE IT - THERE ARE TWO N VARIABLES
	BinNum knownGood("5c881fd924695cf196c2e4fec20d14b642026f2a0b1716ebaabb7067d4d597523d2db69d6d3794622147a14f19a66e7f9037e1d662d34501a8901a5de7771d7c", 512, 16);
	assert(g_0NRes == knownGood);
	std::cout << "Succesfully found new h value." << std::endl;
	hasher.setH(g_0NRes);
	BinNum g_0SigmaRes = hasher.gN(hasher.initNum512, hasher.getH(), hasher.getSigma());
	BinNum sigmaKnownGood("486f64c1917879417fef082b3381a4e211c324f074654c38823a7b76f830ad00fa1fbae42b1285c0352f227524bc9ab16254288dd6863dccd5b9f54a1ad0541b", 512, 16);
	assert(g_0SigmaRes == sigmaKnownGood);
	std::cout << "Successfully obtained GOST specification." << std::endl;
	std::cout << "Testing general hash function - M1 (Undersized)" << std::endl;
	Streebog generalHasher;
	BinNum message2("323130393837363534333231303938373635343332313039383736353433323130393837363534333231303938373635343332313039383736353433323130", 504, 16);
	std::string binaryTranslation = message2.getVal();
	// initialize list -> will always be fed to hasher as a doubly linked list with each node containing 1 byte
	std::list<std::string> bytes;
	for (uint32_t i = 0; i < 63; ++i) {
		bytes.push_back(binaryTranslation.substr(i * 8, 8));
	}
	BinNum res512(hasher.hash(bytes, 512), 512, 16);
	BinNum res256(hasher.hash(bytes, 256), 256, 16);
	std::cout << "512: 0x" <<  hasher.hash(bytes, 512) << std::endl;
	std::cout << "256: 0x" << hasher.hash(bytes, 256) << std::endl;
	std::cout << "***************************************************************************" << std::endl;
	std::cout << "Testing general hash function - M2 (Oversized)" << std::endl;
	BinNum message3("fbe2e5f0eee3c820fbeafaebef20fffbf0e1e0f0f520e0ed20e8ece0ebe5f0f2f120fff0eeec20f120faf2fee5e2202ce8f6f3ede220e8e6eee1e8f0f2d1202ce8f0f2e5e220e5d1", 576, 16);
	binaryTranslation = message3.getVal();
	std::list<std::string> bytes2;
	for (uint32_t i = 0; i < 72; ++i) {
		bytes2.push_back(binaryTranslation.substr(i * 8, 8));
	}
	std::cout << "512: 0x" << hasher.hash(bytes2, 512) << std::endl;
	std::cout << "256: 0x" << hasher.hash(bytes2, 256) << std::endl;
	return 0;
}