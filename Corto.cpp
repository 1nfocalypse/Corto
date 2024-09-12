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
 * the other being Magma, a 64 bit Feistel Network. It is based on a Substition-Permutation Network, and uses a Feistel
 * Network for key scheduling. It was originally designed and released by Kuzmin, Nachaev, and Co. in 2015. For additional
 * information, please consult the README. As a reminder, it is never advised to use non-peer reviewed implementations of 
 * cryptographic schemes, especially those with known vulnerabilities. While this is verified to the standards of GOST,
 * it is *not* peer-reviewed. Additionally, it employs the Streebog-256 cryptographic hash, also verified to GOST spec, 
 * however, this hash inherits some of the underlying problems with Kuznyechik and as such is also of dubious security,
 * particularly as time moves onwards.
 *
 * Project Music Recommendation: Lieliess - XD? (Frenchcore) https://www.youtube.com/watch?v=Krk-EgU9N60
 * Corto -> Read Neuromancer
 ************************************************************************************************************************
 */


// forward declarations
uint32_t encrypt(); // encryption handler
uint32_t decrypt(); // decryption handler

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
    std::cout << "";
    return 540;  
}

uint32_t decrypt() {
    return 540;
}