#include <iostream>
#include <string>
#include <stdexcept>
#include <bitset>
#include <vector>
#include <list>
#include <fstream>
#include <thread>
#include <mutex>
#include <typeinfo>
#include <random>
#include "BinNum.h"
#include "Kuznyechik.h"
#include "Magma.h"
#include "Streebog.h"

#define std_usage std::cout << "Usage: ./corto -h" << std::endl;

/*
 *    ______           __      
 *   / ____/___  _____/ /_____ 
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
 * github as an issue. 
 * 
 * Additionally, Kuznyechik and Magma are both able to perform in either multithreaded ECB mode with a variable thread
 * count or in single thread CBC mode for increased diffusion. For usage, please see the help menu or the README.
 * 
 * Corto and all related files within this project are licensed under the MIT license. For more information, consult the
 * LICENSE.md file in the repository.
 *
 * Project Music Recommendation: Lieliess - XD? (Frenchcore) https://www.youtube.com/watch?v=Krk-EgU9N60
 * Corto / Armitage is a character from the novel "Neuromancer" by William Gibson. I highly recommend reading the book!
 ************************************************************************************************************************
 */

/* TO DO
 * [1]: Edit tester classes to maintain parity with one another                             [ ]
 * [2]: Add tester classes as friends to their respective classes (make the guts private)   [ ]
 * [3]: Take a closer look at speeding up Kuznyechik                                        [X]
 * [4]: Implement ctrl structure for terminal args                                          [X]
 * [5]: Document, document, document                                                        [ ]
 * [6]: Clean up artefacts                                                                  [ ]
 * [7]: Write readme/make art                                                               [ ]
 * [8]: Make public                                                                         [ ]
 */

/*
 * ./corto [-h/-help/--help] -> prints help | ARGC = 2                                              H:[X]I:[X]
 * ./corto [-i/-info/--info] -> prints info | ARGC = 2                                              H:[X]I:[X]
 * ./corto --verify -> verifies all implementations | ARGC = 2                                      H:[X]I:[ ]
 * ./corto -mag --verify -> verifies magma | ARGC = 3                                               H:[X]I:[ ]
 * ./corto -kuz --verify -> verifies kuznyechik | ARGC = 3                                          H:[X]I:[ ]
 * ./corto -str --verify -> verifies streebog | ARGC = 3                                            H:[X]I:[ ]
 * ./corto -str -[256/512] -[stl="<HASH_STR>/tgt="<HSH_TRGT_PATH>] -out=[<PATH_TO_OUT>||0 5         H:[X]I:[X]
 * ./corto -[mag/kuz] -[cbc/ecb=N] -[e/d] -k="<KEY_HERE>" -tgt="<ENC_TRGT_PTH>" -out="<OUTPUT_DIR>" H:[X]I:[ ]
 */

// forward decs
void printHelp();
void printInfo();
std::list<std::string> strToBits(std::string data);
void hashOut(std::string strLit, std::string tarOutPath, int bits);
void procInput(std::string inPath, std::string outPath, int bits);
void argHandler(uint32_t argc, std::vector<std::string> args);
template<typename T> void verifyAlgorithm(const T& algo);
template<typename T> void cbc(const T& crypter, bool encOrDec, std::string key, std::string inputTar, std::string outputTar);
template<typename T> void ecb(const T& crypter, uint32_t threads, bool encOrDec, std::string key, std::string inputTar, std::string outputTar);

// main()
// PRE: Program starts
// POST: Program halts
// WARNINGS: None
// STATUS: Completed, tested

int main(uint32_t argc, char *argv[]) {
    // proc args into vector, cast into std strings
    std::random_device comp;
    std::vector<std::string> arguments;
    for (uint32_t i = 0; i < argc; ++i) {
        arguments.push_back(std::string(argv[i]));
    }
    if (comp.entropy() == 0 && arguments[2] == "-cbc") {
        std::cout << "ERROR: Your device does not support TRNG.\n" <<
            "CBC mode requires RDRAND support. Try ecb?" << std::endl;
        return 0;
    }
    argHandler(argc, arguments);
    return 0;
}

std::list<std::string> strToBits(std::string data) {
    std::list<std::string> dataList;
    for (char i : data) {
        dataList.push_back(std::bitset<8>(i).to_string());
    }
    return dataList;
}

template<typename T>
void verifyAlgorithm(const T& algo) {
    algo.verify();
}

void hashOut(std::string strLit, std::string tarOutPath, int bits) {
    Streebog str;
    std::list<std::string> data = strToBits(strLit);
    std::string hashVal = "0x" + str.hash(data, bits);
    if (tarOutPath == "0") {
        std::cout << hashVal;
    }
    else {
        std::ofstream outFile;
        if (bits == 512) {
            tarOutPath += "corto-streebog512-out.txt";
        }
        else {
            tarOutPath += "corto-streebog256-out.txt";
        }
        outFile.open(tarOutPath);
        outFile << hashVal;
        outFile.close();
    }
}

void procInput(std::string inPath, std::string outPath, int bits) {
    std::ifstream inFile(inPath.c_str());
    std::string data;
    if (inFile.good()) {
        auto out = std::string();
        auto buf = std::string(100, '\0');
        while (inFile.read(&buf[0], 100)) {
            out.append(buf, 0, inFile.gcount());
        }
        out.append(buf, 0, inFile.gcount());
        data = out;
        inFile.close();
    }
    else {
        std::cout << "Error: Could not open input file." << std::endl;
    }
    hashOut(data, outPath, bits);
}

// yes i know this sucks
// i'll actually do a proper command pattern and allat with the big one
// i'm just lazy
void argHandler(uint32_t argc, std::vector<std::string> arguments) {
    switch (argc) {
    case 2:
        if (arguments[1] == "-h" || arguments[1] == "-help" || arguments[1] == "--help") {
            printHelp();
        }
        else if (arguments[1] == "-i" || arguments[1] == "-info" || arguments[1] == "--info") {
            printInfo();
        }
        else if (arguments[1] == "--verify") {
            verifyAlgorithm(Streebog());
            verifyAlgorithm(Kuznyechik());
            verifyAlgorithm(Magma());
        }
        else {
            std_usage;
        }
        break;
    case 3:
        if (arguments[1] == "-str") {
            if (arguments[2] == "--verify") {
                verifyAlgorithm(Streebog());
            }
            else {
                std_usage;
            }
        }
        else if (arguments[1] == "-kuz") {
            if (arguments[2] == "--verify") {
                verifyAlgorithm(Kuznyechik());
            }
            else {
                std_usage;
            }
        }
        else if (arguments[1] == "-mag") {
            if (arguments[2] == "--verify") {
                verifyAlgorithm(Magma());
            }
            else {
                std_usage;
            }
        }
        else {
            std_usage;
        }
        break;
    case 5:
        if (arguments[1] == "-str" && (arguments[2] == "-512" || arguments[2] == "-256")) {
            int bits;
            if (arguments[2] == "-512")
                bits = 512;
            else
                bits = 256;
            std::string tarType = arguments[3].substr(0, 5);
            std::string tarOut = arguments[4].substr(5);

            if (tarType == "-stl=") {
                std::string tarStr = arguments[3].substr(5, arguments[3].length() - 5);
                hashOut(tarStr, tarOut, bits);
            }
            else if (tarType == "-tgt=") {
                std::string tarFile = arguments[3].substr(5, arguments[3].length() - 5);
                procInput(tarFile, tarOut, bits);
            }
            else {
                std_usage;
            }
        }
        else {
            std_usage;
        }
        break;
    case 7:
        if ((arguments[1] == "-mag" || arguments[1] == "-kuz") &&
            (arguments[2] == "-cbc" || arguments[2].rfind("-ecb=", 0) == 0) &&
            (arguments[3] == "-e" || arguments[3] == "-d") && 
            (arguments[4].rfind("-k=", 0) == 0) &&
            (arguments[5].rfind("-tgt=", 0) == 0) &&
            (arguments[6].rfind("-out=", 0) == 0)) {

            bool cipher = (arguments[1] == "-mag"); // 1 for mag, 0 for kuz
            bool mode = (arguments[2] == "-cbc"); // 1 for cbc, 0 for ebc
            bool encOrDec = (arguments[3] == "-e"); // 1 for enc, 0 for dec
            uint32_t threads;
            std::string key = arguments[4].substr(3);
            std::string tarPath = arguments[4].substr(5);
            std::string outPath = arguments[6].substr(5);
            if (!mode) {
                threads = std::stoul(arguments[2].substr(5));
            }
            // i'm a dumbass, gotta do e/d
            if (cipher) {
                Magma mag;
                if (mode) {
                    // cbc(mag), needs key, input, output
                    cbc(mag, encOrDec, key, tarPath, outPath);
                }
                else {
                    // ebc(mag)
                    ecb(mag, encOrDec, threads, key, tarPath, outPath);
                }
            }
            else {
                Kuznyechik kuz;
                if (mode) {
                    // cbc(kuz)
                    cbc(kuz, encOrDec, key, tarPath, outPath);
                }
                else {
                    // ebc(kuz)
                    ecb(kuz, threads, encOrDec, key, tarPath, outPath);
                }
            }
        }
        else {
            std_usage;
        }
    default:
        std_usage;
        std::cout << "Default reached. RM AFTER DEBUGGING." << std::endl;
    }
}

template<typename T>
void cbc(const T& crypter, bool encOrDec, std::string key, std::string inputTar, std::string outputTar) {
    uint32_t blockCtr = 0;  // magma can encrypt up to about 30 GB of data before needing to be rekeyed (2^32 blocks of 64 bits)
    uint32_t blockSize;     // holds block size depending on algorithm passed
    std::string cbcPreviousOutput;  // holds the previous result to xor with current
    std::string cbcCurOutput; // current
    Streebog str; // streebog hasher for KDF-256
    std::ifstream inFile(inputTar); // read from here
    std::ofstream outFile(outputTar); // output to here
    std::string fileExtension = inputTar.substr(inputTar.find(".")); // file extension, to be appended at the back of the file using ecb
    std::random_device rd; // TRNG source for nonces (no need to be kept secret, will be inserted at beginning of file)

    // kdf from user entered code (should really enforce minimum entropic standards)
    std::cout << "Generating keys..." << std::endl;
    // seperate the key into a list
    std::list<std::string> keyList;
    for (char character : key) {
        keyList.push_back(std::bitset<8>(character).to_string());
    }
    std::string derivedKey = str.hash(keyList, 256);
    
    
    BinNum passableKey(derivedKey, 256, 16);
    std::vector<BinNum> keys = crypter.keyScheduler(passableKey);
    std::cout << "Done." << std::endl;
    // determine block size in bytes
    if (typeid(crypter) == typeid(Magma())) {
        blockSize = 8; // 8 8 bit chars -> 64 bit bs
    }
    else {
        blockSize = 16; // 16 8 bit chars -> 128 bit bs
    }
    std::cout << "Generating nonce..." << std::endl;
    std::string nonce;
    uint32_t limit = 8 * blockSize;
    uint32_t curCount = 0;
    // while (curCount != limit) {

    // }


    if (inFile.good()) {
        std::string cipherTarget; // cipher target
        auto buf = std::string(blockSize, '\0'); // input buffer
        while (inFile.read(&buf[0], blockSize)) {
            // read in blocksize
            // if gcount < blocksize
            // pad
            // else op
            if (inFile.gcount() < 128) {
                buf = buf.substr(0, inFile.gcount());
                std::string myStr("\0", 128 - inFile.gcount());
                buf = buf + myStr;
            }
            // if enc
            // send block to encryption
            // if blockCtr >= 1, CBC
            // else if dec
            // send block to dec
            // read next block... oh fuck i have to read from the end
            
            // out.append(buf, 0, inFile.gcount());
        }
        // out.append(buf, 0, inFile.gcount());
        // data = out;
        inFile.close();
    }
    else {
        std::cout << "Could not find target file." << std::endl;
        return;
    }
    inFile.close();
    outFile.close();
}

template<typename T>
void ecb(const T& crypter, uint32_t threads, bool encOrDec, std::string key, std::string inputTar, std::string outputTar) {
    Streebog str;
    std::ifstream inFile(inputTar.c_str());
    std::ofstream outFile(outputTar.c_str());
    uint32_t blockCtr = 0;
    int blockSize;
    std::vector<std::thread> threadPool;
    std::vector<BinNum> blocks;
    std::mutex mtx;
    std::string fileExtension = inputTar.substr(inputTar.find("."));

    std::cout << "Generating keys..." << std::endl;
    std::list<std::string> keyList;
    for (char character : key) {
        keyList.push_back(std::bitset<8>(character).to_string());
    }
    std::string derivedKey = str.hash(keyList, 256);
    BinNum passableKey(derivedKey, 256, 16);
    std::vector<BinNum> keys = crypter.keyScheduler(passableKey);
    std::cout << "Done." << std::endl;

    if (typeid(crypter) == typeid(Magma())) {
        blockSize = 8;
    }
    else {
        blockSize = 16;
    }

    if (inFile.good()) {
        // read in N blocks (if possible)
        auto out = std::string();
        auto buf = std::string(blockSize, '\0');
        while (inFile.read(&buf[0], blockSize)) {
            // reads in a single block
            if (inFile.gcount() < blockSize) {
                // if block < blockSize, pad
                buf = buf.substr(0, inFile.gcount());
                std::string myStr("\0", blockSize - inFile.gcount());
                buf = buf + myStr;
            }
            // buf contains blockSize bytes
            // ********************************************************************************
            // MUST CONVERT TO BINNUM
            // blocks.push_back(buf);



            ++blockCtr;
            if (++blockCtr == 0 && typeid(crypter) == typeid(Magma())) {
                std::string newKey;
                std::cout << "Magma block encryption limit hit. Please rekey to continue.\nNew key: " << std::endl;
                std::cin >> newKey;
                keyList.clear();
                for (char character : newKey) {
                    keyList.push_back(std::bitset<8>(character).to_string());
                }
                derivedKey = str.hash(keyList, 256);
                passableKey = BinNum(derivedKey, 256, 16);
                keys = crypter.keyScheduler(passableKey);
                blockCtr = 0;
            }
            // push block back
            if (blocks.size() == threads || inFile.gcount() < blockSize) {
                std::vector<BinNum> results(blocks.size());
                for (uint32_t i = 0; i < blocks.size(); ++i) {
                    if (encOrDec) {
                        threadPool.push_back(std::thread(&T::threadEncrypt, keys, blocks[i], i, std::ref(results), std::ref(mtx)));
                    }
                    else {
                        threadPool.push_back(std::thread(&T::threadDecrypt, keys, blocks[i], i, std::ref(results), std::ref(mtx)));
                    }
                }
                for (std::thread thread : threadPool) {
                    thread.join();
                }
                threadPool.clear();
                for (auto data : results) {
                    // ********************************************************************************
                    // MUST CONVERT FROM BINNUM (1 or 2 uint64_t values)
                    // outFile << data;




                }
            }
        }
        // append filetype at the end, close outFile
        std::string endBlock;
        for (unsigned char character : fileExtension) {
            endBlock.push_back(character);
        }
        while (endBlock.size() % (blockSize * 8) != 0) {




            // need to pad with PRNG bytes
            // doesn't need to be CSPRNG since max 15 bytes (one char filetype) < state size of say, MT19937 seeded w/ MSB hashed key
            endBlock.push_back('\0');
        }
        outFile << endBlock;
        outFile.close();
    }
    else {
        std::cout << "Failed to find target input file." << std::endl;
    }
}


void printHelp() {
    std::cout << "-------------------------------" << std::endl;
    std::cout << "Corto - General Help" << std::endl;
    std::cout << "-------------------------------" << std::endl;
    std::cout << "Usage: ./Corto -<ARG>" << std::endl;
    std::cout << "-h/-help/--help\t\t\t- Displays this message." << std::endl;
    std::cout << "-i/-info/--info\t\t\t- Displays information about Corto." << std::endl;
    std::cout << "--verify\t\t\t- Verifies all systems to standard." << std::endl;
    std::cout << "Examples: " << std::endl;
    std::cout << "./Corto -h" << std::endl;
    std::cout << "./Corto --verify" << std::endl;
    std::cout << "-------------------------------" << std::endl;
    std::cout << "Corto - Cipher Help" << std::endl;
    std::cout << "-------------------------------" << std::endl;
    std::cout << "Usage: ./Corto -[kuz/mag] -[cbc/ecb=N] -[e/d] -k=\"<KEY_HERE>\" -tgt=\"<ENC_TRGT_PTH>\" -out=\"<OUTPUT_PTH>" << std::endl;
    std::cout << "-[kuz/mag]\t\t\t- Denotes Kuznyechik (*.kuz) or Magma (*.mag) operation." << std::endl;
    std::cout << "-[cbc/ebc=N]\t\t\t- Denotes block cipher mode of operation. Uses N threads if EBC is selected." << std::endl;
    std::cout << "-[e/d]\t\t\t\t- Denotes encrypt or decrypt operation." << std::endl;
    std::cout << "-k=\"<KEY_HERE>\"\t\t\t- Key for operation provided in quotes." << std::endl;
    std::cout << "-tgt=\"<ENC_TRGT_PTH>\"\t\t- Path to file targeted for encryption in quotes." << std::endl;
    std::cout << "-out=\"<OUTPUT_PTH>\"\t\t- Path to output target directory in quotes." << std::endl;
    std::cout << "Examples: " << std::endl;
    std::cout << "./Corto -kuz -cbc -e -k=\"my super secret key\" -tgt=\"C:/Users/Me/Desktop/SecretInfo.txt\" -out=\"./\"" << std::endl;
    std::cout << "./Corto -mag -ecb=6 -d -k=\"badkey\" -tgt=\"./SecretInfo.mag\" -out=\"C:/Users/Me/Documents/\"" << std::endl;
    std::cout << "-------------------------------" << std::endl;
    std::cout << "Corto - Hash Help" << std::endl;
    std::cout << "-------------------------------" << std::endl;
    std::cout << "Usage: ./Corto -str -[256/512] -[stl=\"<STRING_LITERAL>\"/tgt=\"<HASH_TRGT_PTH>\"] -out=[\"<OUTPUT_PTH>\"/0]" << std::endl;
    std::cout << "-str\t\t\t\t- Denotes Streebog operation." << std::endl;
    std::cout << "-[256/512]\t\t\t- Denotes 512 or 256 bit mode." << std::endl;
    std::cout << "-stl=\"<STRING_LITERAL>\"\t\t- Hashes passed string literal in quotes." << std::endl;
    std::cout << "-tgt=\"<HASH_TGT>\"\t\t- Path to file to be hashed." << std::endl;
    std::cout << "-out=\"<PATH_TO_OUTPUT>\"/0\t- Path to hash output. 0 denotes print output in terminal." << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "./Corto -str -512 -stl=\"My super cool string\" -out=0" << std::endl;
    std::cout << "./Corto -str -256 -tgt=\"C:/Users/Me/NewInstall.zip\" -out=\"C:/Users/Me/Hashes/\"" << std::endl;
    std::cout << "-------------------------------" << std::endl;
    std::cout << "For additional information, please view the information card via -i or the README at" << std::endl;
    std::cout << "https://github.com/1nfocalpyse/Corto" << std::endl;
}
void printInfo() {
    std::cout << "                      ______           __      "                      << std::endl;
    std::cout << "                     / ____/___  _____/ /_____ "                      << std::endl;
    std::cout << "                    / /   / __ \\/ ___/ __/ __ \\"                    << std::endl;
    std::cout << "                   / /___/ /_/ / /  / /_/ /_/ /"                      << std::endl;
    std::cout << "                   \\____/\\____/_/   \\__/\\____/ "                  << std::endl;
    std::cout << "******************************************************************"   << std::endl;
    std::cout << "      Written by 1nfocalypse. https://github.com/1nfocalypse"         << std::endl;
    std::cout << "******************************************************************"   << std::endl;
    std::cout << "Corto is a complete implementation of the Russian Federation's "      << std::endl;
    std::cout << "GOST cryptographic standards, to include GOST R 34.11-2012 "          << std::endl;
    std::cout << "\"Streebog\" and GOST R 34.12 - 2015 \"Kuznyechik\" and \"Magma\"."   << std::endl;
    std::cout << "Streebog is a 256 and 512 bit cryptographic hash, with Corto"         << std::endl;
    std::cout << "supporting both file and string literal inputs. Kuznyechik is"        << std::endl;
    std::cout << "a SP-Net based 128 bit block cipher occurring over 10 rounds,"        << std::endl;
    std::cout << "while Magma is a Feistel Network based 64 bit block cipher"           << std::endl;
    std::cout << "that utilizes 32 rounds and a substition-rotation round function."    << std::endl;
    std::cout << "Both ciphers utilize 256 bit keys. All functions are considered"      << std::endl;
    std::cout << "cryptographically weak compared to other available systems, but"      << std::endl;
    std::cout << "are still in use and officially supported by the Russian Federation." << std::endl;
    std::cout << "******************************************************************"   << std::endl;
    std::cout << "The Corto project is licensed under the MIT license."                 << std::endl;
    std::cout << "For more information, please consult the repo available at"           << std::endl;
    std::cout << "https://github.com/1nfocalpyse/Corto"                                 << std::endl;
}