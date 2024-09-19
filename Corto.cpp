#include <iostream>
#include <string>
#include <stdexcept>
#include <bitset>
#include <vector>
#include "BinNum.h"
#include "Kuznyechik.h"
#include "Magma.h"
#include "Streebog.h"

/*
 *    ______           __      
 *   / ________  _____/ /_____ 
 *  / /   / __ \/ ___/ __/ __ \
 * / /___/ /_/ / /  / /_/ /_/ /
 * \____/\____/_/   \__/\____/ 
 *                           
 ************************************************************************************************************************
 * Corto is the handler for and overall name of this implementation of the GOST standards R 34.11-2012 "Streebog" and
 * R 34.12-2015 "Kuznyechik" and "Magma". While all three systems described in the standards have known attacks, they
 * remain the official cryptographic standards of the Russian Federation. For additional information regarding usage,
 * specifications, and functionality, please consult the README. Additionally, while all implementations are verified
 * to meet the test cases set forth in R 34.11-2012 and R 34.12-2015, this implementation has not been peer-reviewed,
 * and as such may contain errors. Please be mindful in your use and report any deviations or bugs you notice on the
 * github. 
 * 
 * Corto and all related files within this project are licensed under the MIT license. For more information, consult the
 * LICENSE.md file in the repository.
 *
 * Project Music Recommendation: Lieliess - XD? (Frenchcore) https://www.youtube.com/watch?v=Krk-EgU9N60
 * Corto / Armitage is a character from the novel "Neuromancer" by William Gibson. I highly recommend reading the book!
 ************************************************************************************************************************
 */

// forward declarations
uint32_t encrypt(); // encryption handler
uint32_t decrypt(); // decryption handler

/* TO DO
 * [1]: Edit tester classes to maintain parity with one another
 * [2]: Add tester classes as friends to their respective classes (make the guts private)
 * [3]: Take a closer look at speeding up Kuznyechik
 * [4]: Implement ctrl structure for terminal args
 * [5]: Document, document, document
 * [6]: Clean up artefacts
 * [7]: Write readme/make art
 * [8]: Make public
 */



// main()
// PRE: Program starts
// POST: Program halts
// WARNINGS: None
// STATUS: Completed, tested

// also, implement CBC for Kuznyechik and Magma. 
// consider processing as hexadecimal strings instead of binary

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
        std::cout << "Magma has several known attacks." << std::endl;
        std::cout << "Streebog is considered a weaker hash." << std::endl;
        std::cout << "For more information, please consult the README." << std::endl;
        std::cout << "Created by 1nfocalypse: https://github.com/1nfocalypse\n\n" << std::endl;
        std::cout << "Please choose a menu option below." << std::endl;
        std::cout << "********************************************************" << std::endl;
        std::cout << "1. Encrypt" << std::endl;
        std::cout << "2. Decrypt" << std::endl;
        std::cout << "3. Hash" << std::endl;
        std::cout << "4. Quit" << std::endl;
        std::cout << "********************************************************" << std::endl;
        std::getline(std::cin, menChoiceProxy);
        int menChoice = menChoiceProxy[0] - '0';
        while (menChoice < 1 || menChoice > 4) {
            std::cout << "Invalid choice. Please select a valid option." << std::endl;
            std::cout << "********************************************************" << std::endl;
            std::cout << "1. Encrypt" << std::endl;
            std::cout << "2. Decrypt" << std::endl;
            std::cout << "3. Hash" << std::endl;
            std::cout << "4. Quit" << std::endl;
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