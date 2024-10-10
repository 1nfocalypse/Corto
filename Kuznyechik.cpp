#include <iostream>
#include <string>
#include <stdexcept>
#include <bitset>
#include <vector>
#include <chrono>
#include "Kuznyechik.h"
#include "BinNum.h"
#include "KuzFunctor.h"

BinNum Kuznyechik::sBox(BinNum number) {
    unsigned int transformMatrix[] = {
        252, 238, 221, 17, 207, 110, 49, 22, 251, 196, 250, 218, 35, 197, 4, 77,
        233, 119, 240, 219, 147, 46, 153, 186, 23, 54, 241, 187, 20, 205, 95, 193,
        249, 24, 101, 90, 226, 92, 239, 33, 129, 28, 60, 66, 139, 1, 142, 79,
        5, 132, 2, 174, 227, 106, 143, 160, 6, 11, 237, 152, 127, 212, 211, 31,
        235, 52, 44, 81, 234, 200, 72, 171, 242, 42, 104, 162, 253, 58, 206, 204,
        181, 112, 14, 86, 8, 12, 118, 18, 191, 114, 19, 71, 156, 183, 93, 135,
        21, 161, 150, 41, 16, 123, 154, 199, 243, 145, 120, 111, 157, 158, 178, 177,
        50, 117, 25, 61, 255, 53, 138, 126, 109, 84, 198, 128, 195, 189, 13, 87,
        223, 245, 36, 169, 62, 168, 67, 201, 215, 121, 214, 246, 124, 34, 185, 3,
        224, 15, 236, 222, 122, 148, 176, 188, 220, 232, 40, 80, 78, 51, 10, 74,
        167, 151, 96, 115, 30, 0, 98, 68, 26, 184, 56, 130, 100, 159, 38, 65,
        173, 69, 70, 146, 39, 94, 85, 47, 140, 163, 165, 125, 105, 213, 149, 59,
        7, 88, 179, 64, 134, 172, 29, 247, 48, 55, 107, 228, 136, 217, 231, 137,
        225, 27, 131, 73, 76, 63, 248, 254, 141, 83, 170, 144, 202, 216, 133, 97,
        32, 113, 103, 164, 45, 43, 9, 91, 203, 155, 37, 208, 190, 229, 108, 82,
        89, 166, 116, 210, 230, 244, 180, 192, 209, 102, 175, 194, 57, 75, 99, 182
    };
    BinNum y("0", 128);
    BinNum mask("11111111", 128); // equivalent to a normal 0xFF mask to obtain least significant byte (assuming big endian)
    for (long int i = 15; i >= 0; --i) {
        // shift return value by 8 to make room for new byte
        y = y << 8;
        // we must convert our masked number back to an unsigned int
        // this is our S-box index
        BinNum maskedNum = (number >> (8 * i)) & mask;
        std::string binStr = maskedNum.getVal();
        unsigned int index = static_cast<unsigned int>(std::stoul(binStr.c_str(), nullptr, 2));
        BinNum transformResult(std::bitset<8>(static_cast<uint8_t>(transformMatrix[index])).to_string(), 128);
        y = y ^ transformResult;
    }
    return y;
}

BinNum Kuznyechik::sBoxInv(BinNum number) {
    unsigned int transformMatrixInv[] = {
         165, 45, 50, 143, 14, 48, 56, 192, 84, 230, 158, 57, 85, 126, 82, 145,
         100, 3, 87, 90, 28, 96, 7, 24, 33, 114, 168, 209, 41, 198, 164, 63,
         224, 39, 141, 12, 130, 234, 174, 180, 154, 99, 73, 229, 66, 228, 21, 183,
         200, 6, 112, 157, 65, 117, 25, 201, 170, 252, 77, 191, 42, 115, 132, 213,
         195, 175, 43, 134, 167, 177, 178, 91, 70, 211, 159, 253, 212, 15, 156, 47,
         155, 67, 239, 217, 121, 182, 83, 127, 193, 240, 35, 231, 37, 94, 181, 30,
         162, 223, 166, 254, 172, 34, 249, 226, 74, 188, 53, 202, 238, 120, 5, 107,
         81, 225, 89, 163, 242, 113, 86, 17, 106, 137, 148, 101, 140, 187, 119, 60,
         123, 40, 171, 210, 49, 222, 196, 95, 204, 207, 118, 44, 184, 216, 46, 54,
         219, 105, 179, 20, 149, 190, 98, 161, 59, 22, 102, 233, 92, 108, 109, 173,
         55, 97, 75, 185, 227, 186, 241, 160, 133, 131, 218, 71, 197, 176, 51, 250,
         150, 111, 110, 194, 246, 80, 255, 93, 169, 142, 23, 27, 151, 125, 236, 88,
         247, 31, 251, 124, 9, 13, 122, 103, 69, 135, 220, 232, 79, 29, 78, 4,
         235, 248, 243, 62, 61, 189, 138, 136, 221, 205, 11, 19, 152, 2, 147, 128,
         144, 208, 36, 52, 203, 237, 244, 206, 153, 16, 68, 64, 146, 58, 1, 38,
         18, 26, 72, 104, 245, 129, 139, 199, 214, 32, 10, 8, 0, 76, 215, 116
    };
    BinNum y("0", 128);
    BinNum mask("11111111", 128); // equivalent to a normal 0xFF mask to obtain least significant byte (assuming big endian)
    for (long int i = 15; i >= 0; --i) {
        y = y << 8;
        // we must convert our masked number back to an unsigned int
        // masked number
        BinNum maskedNum = (number >> (8 * i)) & mask;
        std::string binStr = maskedNum.getVal();
        unsigned int index = static_cast<unsigned int>(std::stoul(binStr.c_str(), nullptr, 2));
        BinNum transformResult(std::bitset<8>(static_cast<uint8_t>(transformMatrixInv[index])).to_string(), 128);
        y = y ^ transformResult;
    }
    return y;
}

BinNum Kuznyechik::pBoxWrapper(BinNum number) { // L in Kuz spec
    for (uint32_t i = 0; i < 16; ++i) {
        number = pBox(number);
    }
    return number;
}

BinNum Kuznyechik::pBoxInvWrapper(BinNum number) { // L^-1 in Kuz spec
    for (uint32_t i = 0; i < 16; ++i) {
        number = pBoxInv(number);
    }
    return number;
}

BinNum Kuznyechik::pBox(BinNum number) { // R in Kuz spec
    BinNum ret128 = BinNum(std::bitset<8>(linearFunct(number)).to_string(), 128);
    return (ret128 << (8 * 15) ^ (number >> 8));
}

BinNum Kuznyechik::pBoxInv(BinNum number) { // R^-1 in Kuz spec
    BinNum numberCopy(number.getVal(), 129);
    BinNum one128("1", 129);
    BinNum two128("10", 129);
    BinNum initial((number >> (15 * 8)).getVal(), 129);
    numberCopy = (numberCopy << 8) & ((two128 << 127) - one128); // may be incorrect
    BinNum toXor((numberCopy ^ initial).getVal().substr(1,128));
    BinNum linRet128 = BinNum(std::bitset<8>(linearFunct(toXor)).to_string(), 128); // needs to get cast down here before being sent to linear funct
    BinNum chopped(numberCopy.getVal().substr(1, 128));
    return (chopped ^ linRet128);
}

uint8_t Kuznyechik::linearFunct(BinNum number) {
    std::vector<uint8_t> constants = { 148, 32, 133, 16, 194, 192, 1, 251, 1, 192, 194, 16, 133, 32, 148, 1 };
    BinNum numberCopy(number);
    BinNum retVal("0", 8);
    BinNum zero128("0", 128);
    BinNum FFMask128("11111111", 128);
    while (numberCopy != zero128) {
        BinNum maskedVal = numberCopy & FFMask128;
        uint8_t xVal = static_cast<uint8_t>(std::stoul(maskedVal.getVal(), nullptr, 2));
        retVal = retVal ^ BinNum(std::bitset<8>(kuzMult(xVal, constants.back())).to_string());
        constants.pop_back();
        numberCopy = numberCopy >> 8;
    }
    return static_cast<uint8_t>(std::stoul(retVal.getVal(), nullptr, 2));
}

uint8_t Kuznyechik::kuzMult(uint8_t x, uint8_t y) {
    // cast these to BinNum, allocate 128 bits
    // then have mult return a 128 bit binNum
    // modBin then needs changes to accept two BinNums
    // then KuzMult needs to mask the last 8 bits
    // and return that
    // we exclusively work with 128 bits internally
    BinNum xBin(std::bitset<8>(x).to_string(), 128);
    BinNum yBin(std::bitset<8>(y).to_string(), 128);
    BinNum toMod = multBinPolys(xBin, yBin);
    BinNum modulus("111000011", 128); // 111000011 -> x^7 + x^6 + x^5 + x + 1
    BinNum retVal = modBinPolys(toMod, modulus);
    // convert BinNum to uint8
    uint8_t castRetVal = std::stoul(retVal.getVal(), nullptr, 2) & 0xFF;
    return castRetVal;
}

BinNum Kuznyechik::multBinPolys(BinNum x, BinNum y) {
    BinNum zero128("0", 128);
    if (x == zero128 || y == zero128) {
        return zero128;
    }
    BinNum condConst("0", 128);
    BinNum retNum("0", 128);
    BinNum loopCst("1", 128);
    while (x != condConst) {
        if ((x & loopCst) == loopCst) {
            retNum = retNum ^ y;
        }
        y = y << 1;
        x = x >> 1;
    }
    #pragma warning(push)
    #pragma warning(disable: 6001)
    return retNum;
    #pragma warning(pop)
}

BinNum Kuznyechik::modBinPolys(BinNum x, BinNum y) {
    uint8_t numBits = curBits(y);
    while (1) {
        uint8_t numBits2 = curBits(x);
        if (numBits2 < numBits) {
            return x;
        }
        BinNum modShift = (y << (numBits2 - numBits));
        x = x ^ modShift;
    }
}

uint8_t Kuznyechik::curBits(BinNum number) {
    uint8_t retNum = 0;
    BinNum zero128("0", 128);
    while (number != zero128) {
        ++retNum;
        number = number >> 1;
    }
    return retNum;
}

std::vector<BinNum> Kuznyechik::keyScheduler(BinNum number) {
    std::vector<BinNum> keys;
    BinNum one256("1", 256);
    BinNum two256("10", 256);
    // gonna have to find out how ot systemically turn these into 128 bit keys before sending them to pBoxWrapper and sBox
    BinNum key1 = BinNum(((number >> 128) & BinNum("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", 256, 16)).getVal().substr(128, 128));
    BinNum key2 = BinNum((number & (((two256 << 127) - one256))).getVal().substr(128, 128));
    keys.push_back(key1);
    keys.push_back(key2);
    for (uint8_t i = 0; i < 4; ++i) {
        for (uint8_t j = 0; j < 8; ++j) {
            uint8_t xorModArg = 8 * i + j + 1; // will never exceed 32
            BinNum xorModifier = pBoxWrapper(BinNum(std::bitset<8>(xorModArg).to_string(), 128)); // verify compliance with input
            BinNum key1Prev(key1);
            key1 = pBoxWrapper(sBox(key1 ^ xorModifier)) ^ key2;
            key2 = key1Prev;
        }
        keys.push_back(key1);
        keys.push_back(key2);
    }
    return keys;
}

BinNum Kuznyechik::encrypt(BinNum plaintext, std::vector<BinNum> keys) {
    BinNum ptCopy(plaintext);
    for (uint32_t i = 0; i < 9; ++i) {
        ptCopy = pBoxWrapper(sBox(ptCopy ^ keys[i]));
    }
    return ptCopy ^ keys.back();
}

BinNum Kuznyechik::decrypt(BinNum ciphertext, std::vector<BinNum> keys) {
    BinNum ctCopy(ciphertext);
    std::reverse(keys.begin(), keys.end());
    for (uint32_t i = 0; i < 9; ++i) {
        ctCopy = sBoxInv(pBoxInvWrapper(ctCopy ^ keys[i]));
    }
    return ctCopy ^ keys.back();
}

void Kuznyechik::threadEncrypt(std::vector<BinNum> keys, BinNum tarBlock, uint32_t retIndex, std::vector<BinNum>& results, std::mutex& mtx) {
    mtx.lock();
    results[retIndex] = encrypt(tarBlock, keys);
    mtx.unlock();
}

void Kuznyechik::threadDecrypt(std::vector<BinNum> keys, BinNum tarBlock, uint32_t retIndex, std::vector<BinNum>& results, std::mutex& mtx) {
    mtx.lock();
    results[retIndex] = decrypt(tarBlock, keys);
    mtx.unlock();
}


void Kuznyechik::verify() const {
    KuzFunctor()();
}