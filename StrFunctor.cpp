#include "StrFunctor.h"
#include "Streebog.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <bitset>
#include <cassert>
#include <list>

void StrFunctor::operator()() {
	Streebog hasher;
	std::cout << "Verifying Streebog..." << std::endl;
	std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
	std::cout << "Testing key derivation: ";
	hasher.setH(hasher.initNum512);
	hasher.setN(hasher.initNum512);
	hasher.setSigma(hasher.initNum512);
	BinNum message("01323130393837363534333231303938373635343332313039383736353433323130393837363534333231303938373635343332313039383736353433323130", 512, 16);
	hasher.setInitKey(hasher.precompShift(hasher.pBox(hasher.sBox(hasher.getH() ^ hasher.getN()))));
	assert(hasher.getInitKey() == BinNum("b383fc2eced4a574b383fc2eced4a574b383fc2eced4a574b383fc2eced4a574b383fc2eced4a574b383fc2eced4a574b383fc2eced4a574b383fc2eced4a574", 512, 16));
	std::cout << "Success." << std::endl;
	std::cout << "Keygen functional." << std::endl;
	std::cout << "***********************" << std::endl;
	std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
	std::cout << "Testing E subfunction: ";
	// apply E(K,m)
	BinNum eRes = hasher.E(hasher.getInitKey(), message);
	assert(eRes == BinNum("fc221dc8b814fc27a4de079d10097600209e5375776898961f70bded0647bd8f1664cfa8bb8d8ff1e0df3e621568b66aa075064b0e81cce132c8d1475809ebd2", 512, 16));
	std::cout << "Success." << std::endl;
	std::cout << "E functional." << std::endl;
	std::cout << "***********************" << std::endl;
	std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
	// apply g_N(h, m)
	std::cout << "Testing round function." << std::endl;
	BinNum g_NRes = hasher.gN(hasher.getN(), hasher.getH(), message);
	std::cout << "Testing case 1: ";
	assert(g_NRes == BinNum("fd102cf8812ccb1191ea34af21394f3817a86641445aa9a626488adb33738ebd2754f6908cbbbac5d3ed0f522c50815c954135793fb1f5d905fee4736b3bdae2", 512, 16));
	std::cout << "Success." << std::endl;
	hasher.setH(g_NRes);
	hasher.setN(BinNum("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001f8", 512, 16));
	hasher.setSigma(message);
	BinNum g_0NRes = hasher.gN(hasher.initNum512, hasher.getH(), hasher.getN()); // TURNS OUT G_N IS MISLEADING - DO NOT BELIEVE IT - THERE ARE TWO N VARIABLES
	BinNum knownGood("5c881fd924695cf196c2e4fec20d14b642026f2a0b1716ebaabb7067d4d597523d2db69d6d3794622147a14f19a66e7f9037e1d662d34501a8901a5de7771d7c", 512, 16);
	std::cout << "Testing case 2: ";
	assert(g_0NRes == knownGood);
	std::cout << "Success." << std::endl;
	hasher.setH(g_0NRes);
	BinNum g_0SigmaRes = hasher.gN(hasher.initNum512, hasher.getH(), hasher.getSigma());
	BinNum sigmaKnownGood("486f64c1917879417fef082b3381a4e211c324f074654c38823a7b76f830ad00fa1fbae42b1285c0352f227524bc9ab16254288dd6863dccd5b9f54a1ad0541b", 512, 16);
	std::cout << "Testing case 3: ";
	assert(g_0SigmaRes == sigmaKnownGood);
	std::cout << "Success." << std::endl;
	std::cout << "Hash functional." << std::endl;
	std::cout << "***********************" << std::endl;
	std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
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
	BinNum exp512M1("486f64c1917879417fef082b3381a4e211c324f074654c38823a7b76f830ad00fa1fbae42b1285c0352f227524bc9ab16254288dd6863dccd5b9f54a1ad0541b", 512, 16);
	BinNum obt512M1(hasher.hash(bytes, 512), 512, 16);
	BinNum exp256M1("00557be5e584fd52a449b16b0251d05d27f94ab76cbaa6da890b59d8ef1e159d", 256, 16);
	BinNum obt256M1(hasher.hash(bytes, 256), 256, 16);
	std::cout << "Case 1: 512 bits - ";
	assert(exp512M1 == obt512M1);
	std::cout << "Success." << std::endl;
	std::cout << "Case 2: 256 bits - ";
	assert(exp256M1 == obt256M1);
	std::cout << "Success." << std::endl;
	std::cout << "***********************" << std::endl;
	std::cout << "Testing general hash function - M2 (Oversized)" << std::endl;
	BinNum message3("fbe2e5f0eee3c820fbeafaebef20fffbf0e1e0f0f520e0ed20e8ece0ebe5f0f2f120fff0eeec20f120faf2fee5e2202ce8f6f3ede220e8e6eee1e8f0f2d1202ce8f0f2e5e220e5d1", 576, 16);
	binaryTranslation = message3.getVal();
	std::list<std::string> bytes2;
	for (uint32_t i = 0; i < 72; ++i) {
		bytes2.push_back(binaryTranslation.substr(i * 8, 8));
	}
	BinNum exp512M2("28fbc9bada033b1460642bdcddb90c3fb3e56c497ccd0f62b8a2ad4935e85f037613966de4ee00531ae60f3b5a47f8dae06915d5f2f194996fcabf2622e6881e", 512, 16);
	BinNum obt512M2(hasher.hash(bytes2, 512), 512, 16);
	BinNum exp256M2("508f7e553c06501d749a66fc28c6cac0b005746d97537fa85d9e40904efed29d", 256, 16);
	BinNum obt256M2(hasher.hash(bytes2, 256), 256, 16);
	std::cout << "Case 1: 512 bits - ";
	assert(exp512M2 == obt512M2);
	std::cout << "Success." << std::endl;
	std::cout << "Case 2: 256 bits - ";
	assert(exp256M2 == obt256M2);
	std::cout << "Success." << std::endl;
	std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << std::endl;
	std::cout << "All routines intact." << std::endl;
	std::cout << "Continuing..." << std::endl;
}