#include "Streebog.h"
#include <string>
#include <bitset>
#include <stdexcept>
#include <vector>
#include <iostream>

// consider processing as hexadecimal strings instead of binary

// instrumented for testing -> testing blocks will be seperated out by spaces and ***** bars
std::string Streebog::hash(std::list<std::string> cleartext, int size) {
	if (size != 512 && size != 256) {
		throw std::invalid_argument("Passed invalid hash size. Must be 512 or 256.");
	}
	if (size == 256) {
		this->h = initNum256;
	}
	else {
		this->h = initNum512;
	}
	this->N = initNum512;
	this->sigma = initNum512;
	// stage 2
	while (cleartext.size() > 64) {
		// calculate subvector
		std::string subvector = "";
		std::list<std::string>::iterator rearIterator = cleartext.end();
		--rearIterator; // set to the first accessible element
		for (uint32_t i = 0; i < 64; ++i) {
			subvector = *rearIterator + subvector;
			--rearIterator;
			cleartext.pop_back();
		}
		// cleartext list -> M'
		// subvector string = m
		this->h = gN(getN(), getH(), BinNum(subvector, 512, 2));
		BinNum c512as1024("512", 1024, 10);
		BinNum n1024(N.getVal(), 1024);
		c512as1024 = c512as1024 + n1024;
		this->N = BinNum(c512as1024.getVal().substr(512, 512));
		BinNum sigma1024(this->sigma.getVal(), 1024);
		BinNum message1024(subvector, 1024);
		this->sigma = BinNum((sigma1024 + message1024).getVal().substr(512, 512));
		// M already equal M'
	}
	std::string lsbInit = "";
	std::list<std::string>::iterator it = cleartext.begin();
	while (it != cleartext.end()) {
		lsbInit += *it;
		++it;
	}
	uint32_t messageCardinality = lsbInit.size();
	if (messageCardinality < 512) {
		lsbInit = "1" + lsbInit;
		for (uint32_t i = 0; 0 < 512 - lsbInit.size(); ++i) {
			lsbInit = "0" + lsbInit;
		}
	}
	BinNum message(lsbInit, 512, 2);
	this->h = gN(getN(), getH(), message);
	BinNum mCardTransform(std::bitset<32>(messageCardinality).to_string(), 1024);
	BinNum n1024(this->N.getVal(), 1024);
	this->N = BinNum((n1024 + mCardTransform).getVal().substr(512, 512));
	BinNum sigma1024(this->sigma.getVal(), 1024);
	BinNum m1024(lsbInit, 1024);
	this->sigma = BinNum((m1024 + sigma1024).getVal().substr(512, 512));
	this->h = gN(BinNum("0", 512), getH(), getN());
	this->h = gN(BinNum("0", 512), getH(), getSigma());
	if (size == 256) {
		return this->h.getVal(16).substr(0, 64);
	}
	else {
		return this->h.getVal(16);
	}
}

// seems correct
BinNum Streebog::sBox(BinNum number) const { // S in Stree spec
	BinNum mask("FF", 512, 16);
	mask = mask << 504;
	std::string curStr;
	for (uint32_t i = 0; i < 64; ++i) {
		BinNum curNum = number & mask;
		std::string curByte = curNum.getVal().substr((i * 8), 8);
		uint8_t index = std::stoi(BinNum(curByte, 8).getVal(10));
		uint8_t transformVal = this->transformMatrix[index];
		curStr.append(std::bitset<8>(transformVal).to_string());
		mask = mask >> 8;
	}
	return BinNum(curStr, 512);
}

// we take a 256 bit input
// for each iteration, we append the result of 
// accessing the 256 bit input at the tau[i]'th byte
BinNum Streebog::pBox(BinNum number) const { // P in Stree spec
	std::string curStr;

	uint8_t tmpArr[64];

	for (uint8_t i = 0; i < 64; ++i) {
		uint32_t byteNum = this->tau[i];
		uint32_t accessPoint = byteNum * 8;
		std::string subString = number.getVal().substr(accessPoint, 8);
		curStr.append(subString);
		tmpArr[i] = static_cast<uint8_t>(std::stoul((subString), nullptr, 2));
	}
	return BinNum(curStr, 512);
}

BinNum Streebog::precompShift(BinNum number) const { // L in Streebog spec
	// process number into an array of bytes
	uint8_t numBytes[64];
	for (uint32_t i = 0; i < 64; ++i) {
		numBytes[i] = static_cast<uint8_t>(std::stoul(number.getVal().substr(8 * i, 8), nullptr, 2));
	}
	for (uint32_t i = 0; i < 8; ++i) {
		uint64_t tmp = 0;
		for (uint32_t j = 0; j < 8; ++j) {
			for (uint32_t k = 0; k < 8; k++) {
				if ((numBytes[i * 8 + j] & (1 << (7 - k))) != 0) {
					tmp = tmp ^ this->AManual[j * 8 + k];
				}
			}
		}
		// convert back to bytes
		for (uint32_t j = 0; j < 8; ++j) {
			numBytes[i * 8 + j] = (tmp & ((uint64_t)0xFF << (7 - j) * 8)) >> (7 - j) * 8;
		}
	}
	// convert to bitStr
	std::string retStr;
	for (uint32_t i = 0; i < 64; ++i) {
		retStr += std::bitset<8>(numBytes[i]).to_string();
	}
	return BinNum(retStr, 512, 2);
}


std::vector<BinNum> Streebog::keyGen(BinNum initKey) const {
	std::vector<BinNum> retVals = { initKey };
	for (uint32_t i = 1; i < 13; ++i) {
		std::string curConst = this->itConstants[i - 1].getVal();
		std::reverse(curConst.begin(), curConst.end());
		BinNum constRep(curConst);
		BinNum arg = retVals[i - 1] ^ this->itConstants[i - 1];
		BinNum val1 = sBox(arg);
		BinNum val2 = pBox(val1);
		BinNum val3 = precompShift(val2);
	
		retVals.push_back(val3);
	}
	return retVals;
}

BinNum Streebog::gN(BinNum n, BinNum h, BinNum m) const {
	return E(precompShift(pBox(sBox(h ^ n))), m) ^ h ^ m;
}

BinNum Streebog::E(BinNum left, BinNum message) const {
	std::vector<BinNum> keys = keyGen(left);
	BinNum tmp = precompShift(pBox(sBox(keys[0] ^ message)));
	for (uint32_t i = 0; i < 11; ++i) {
	 	tmp = precompShift(pBox(sBox(keys[i + 1] ^ tmp)));
	}
	return tmp ^ keys[12];
}


void Streebog::setH(BinNum number) {
	this->h = number;
}

void Streebog::setN(BinNum number) {
	this->N = number;
}

void Streebog::setSigma(BinNum number) {
	this->sigma = number;
}

BinNum Streebog::getH() const {
	return this->h;
}

BinNum Streebog::getN() const {
	return this->N;
}

BinNum Streebog::getSigma() const {
	return this->sigma;
}

void Streebog::setInitKey(BinNum number) {
	this->initKey = number;
}

BinNum Streebog::getInitKey() const {
	return this->initKey;
}

void Streebog::verify() const {
	std::cout << "Not implemented." << std::endl;
}