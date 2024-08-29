#include <iostream>
#include <string>
#include <stdexcept>
#include <bitset>
#include <vector>
#include "BinNum.h"
#include "Kuznyechik.h"

/*
 *    ______           __      
 *   / ________  _____/ /_____ 
 *  / /   / __ \/ ___/ __/ __ \
 * / /___/ /_/ / /  / /_/ /_/ /
 * \____/\____/_/   \__/\____/ 
 *                           
 ************************************************************************************************************************
 * Corto is an implementation of the Russian Kuznyechik (lit. Grasshopper), a symmetric block cipher with a 128 bit block
 * size and a key length of 256 bits. While susceptible to a round 5 Meet-In-The-Middle attack, it is still specified in
 * GOST R 34.12-2015 and RFC 7801. It is one of two symmetric ciphers currently in use by the Russian Federation, with 
 * the other being Magma. It is based on a Substition-Permutation Network, and uses a Feistel Network for key
 * scheduling. It was originally designed and released by Kuzmin, Nachaev, and Co. in 2015. For additional information,
 * please consult the README. As a reminder, it is never advised to use unverified implementations of cryptographic
 * schemes, especially those with known vulnerabilities.
 *
 * Project Music Recommendation: Lieliess - XD? (Frenchcore) https://www.youtube.com/watch?v=Krk-EgU9N60
 * Corto -> Read Neuromancer
 ************************************************************************************************************************
 */


// forward declarations
uint32_t encrypt(); // encryption handler
uint32_t decrypt(); // decryption handler
/*
BinNum sBox(BinNum number); // substitution box per GOST spec
BinNum sBoxInv(BinNum number); // inverse of S-box per GOST spec
BinNum pBoxWrapper(BinNum number); // permutation box wrapper for full coverage
BinNum pBoxInvWrapper(BinNum number); // inverse of P-box wrapper for full coverage
BinNum pBox(BinNum number); // permutation box per GOST spec
BinNum pBoxInv(BinNum number); // inverse of P-box per GOST spec
uint8_t kuzMult(uint8_t x, uint8_t y); // 8-bit Kuznyechik multiplication (similar to Rijndael mult with a different modulus)
uint32_t roundFunc(int number); // Feistel-network based keyscheduler
*/

// main()
// PRE: Program starts
// POST: Program halts
// WARNINGS: None
// STATUS: Completed, tested
int main() {
    while (1) {
        std::string menChoiceProxy;
        std::cout << "   ______           __      " << std::endl;
        std::cout << "  / ________  _____/ /_____ " << std::endl;
        std::cout << " / /   / __ \\/ ___/ __/ __ \\" << std::endl;
        std::cout << "/ /___/ /_/ / /  / /_/ /_/ /" << std::endl;
        std::cout << "\\____/\\____/_/   \\__/\\____/ " << std::endl;
        std::cout << "********************************************************" << std::endl;
        std::cout << "USE AT YOUR OWN RISK!" << std::endl;
        std::cout << "This implementation of Kuznyechik is unverified." << std::endl;
        std::cout << "Kuznyechik has a known attack." << std::endl;
        std::cout << "For more information, please consult the README." << std::endl;
        std::cout << "Created by 1nfocalypse: https://github.com/1nfocalypse\n\n" << std::endl;
        std::cout << "Please choose a menu option below." << std::endl;
        std::cout << "********************************************************" << std::endl;
        std::cout << "1. Encrypt" << std::endl;
        std::cout << "2. Decrypt" << std::endl;
        std::cout << "3. Quit" << std::endl;
        std::cout << "********************************************************" << std::endl;
        std::getline(std::cin, menChoiceProxy);
        int menChoice = menChoiceProxy[0] - '0';
        while (menChoice < 1 || menChoice > 3) {
            std::cout << "Invalid choice. Please select a valid option." << std::endl;
            std::cout << "********************************************************" << std::endl;
            std::cout << "1. Encrypt" << std::endl;
            std::cout << "2. Decrypt" << std::endl;
            std::cout << "3. Quit" << std::endl;
            std::cout << "********************************************************" << std::endl;
            std::getline(std::cin, menChoiceProxy);
            menChoice = menChoiceProxy[0] - '0';
        }
        switch (menChoice) {
        case 1:
            encrypt();
            break;
        case 2:
            decrypt();
            break;
        case 3:
            std::cout << "Quitting..." << std::endl;
            return 0;
        default:
            std::cout << "Unexpected Error: Uncaught menu input. Exiting..." << std::endl;
            return 1;
        }
    }
    return 0;
}

uint32_t encrypt() {
    return 540;  
}

uint32_t decrypt() {
    return 540;
}
/*
// should have and return a 128 bit BinNum
BinNum sBox(BinNum number) {
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

// takes and returns 128 bit binnum
BinNum sBoxInv(BinNum number) {
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

BinNum pBoxWrapper(BinNum number) { // per spec, this is the function L()
    for (uint32_t i = 0; i < 16; ++i) {
        number = pBox(number);
    }
    return number;
}

BinNum pBoxInvWrapper(BinNum number) { // per spec, this is the function L^-1
    for (uint32_t i = 0; i < 16; ++i) {
        number = pBoxInv(number);
    }
    return number;
}

BinNum pBox(BinNum number) {
    BinNum numberCopy(number);
    std::vector<uint8_t> constants = { 148, 32, 133, 16, 194, 192, 1, 251, 1, 192, 194, 16, 133, 32, 148, 1 };
    BinNum FFMask128("11111111", 128);
    BinNum retVal("0", 8);
    BinNum zero128("0", 128);
    while (numberCopy != zero128) {
        BinNum maskedVal = numberCopy & FFMask128;
        uint8_t xVal = static_cast<uint8_t>(std::stoul(maskedVal.getVal(), nullptr, 2));
        retVal = retVal ^ BinNum(std::bitset<8>(kuzMult(xVal, constants.back())).to_string());
        constants.pop_back();
        numberCopy = numberCopy >> 8;
    }
    BinNum ret128(retVal.getVal(), 128);
    return (ret128 << (8 * 15) ^ (number >> 8));
}

BinNum pBoxInv(BinNum number) {
    BinNum numberCopy(number);
    BinNum initial(number >> (15 * 8));


}

uint8_t linear_funct(BinNum number) {

}

uint8_t kuzMult(uint8_t x, uint8_t y) {
    uint8_t toMod = multBinPolynomials(x, y);
    uint8_t modulus = 451; // 111000011 -> x^7 + x^6 + x^5 + x + 1
    return modBinPolynomials(toMod, modulus);
}

uint8_t multBinPolynomials(uint8_t x, uint8_t y) {

}

uint8_t modBinPolynomials(uint8_t x, uint8_t y) {

}
*/
