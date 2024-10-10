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

// forward decs
void printHelp();
void printInfo();
std::list<std::string> strToBits(std::string data);
void hashOut(std::string &strLit, std::string tarOutPath, int bits);
void procInput(std::string inPath, std::string outPath, int bits);
void argHandler(uint32_t argc, std::vector<std::string> args);
template<typename T>
void verifyAlgorithm(const T& algo);
template<typename T>
void cbc(T& crypter, bool encOrDec, std::string key, std::string inputTar, std::string outputTar);
template<typename T>
void ecb(T& crypter, uint32_t threads, bool encOrDec, std::string key, std::string inputTar, std::string outputTar);

// main()
// PRE: Program starts
// POST: Program halts
// WARNINGS: None
// STATUS: Completed, tested

int main(uint32_t argc, char *argv[]) {
    // proc args into vector, cast into std strings
    std::vector<std::string> arguments;
    for (uint32_t i = 0; i < argc; ++i) {
        arguments.push_back(std::string(argv[i]));
    }
    argHandler(argc, arguments);
    return 0;
}

// strToBits(std::string data)
// PRE: String of data passed
// POST: String is seperated into a list of bytes
// WARNINGS: Large strings may cause issues depending on user memory availability due to contiguity requirements.
std::list<std::string> strToBits(std::string data) {
    std::list<std::string> dataList;
    for (char i : data) {
        dataList.push_back(std::bitset<8>(i).to_string());
    }
    return dataList;
}

// verifyAlgorithm(const T& algo)
// PRE: A valid cryptographic object is passed
// POST: Verification for referenced algorithm is complete
// WARNINGS: None
template<typename T>
void verifyAlgorithm(const T& algo) {
    algo.verify();
}

// hashOut(std::string &strLit, std::string tarOutPath, int bits)
// PRE: valid string literal, output path, and bits (512/256) passed
// POST: Hash value is outputted to file
// WARNINGS: File name collision may occur
void hashOut(std::string &strLit, std::string tarOutPath, int bits) {
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

// procInput(std::string inPath, std::string outPath, int bits)
// PRE: input path, output path, and bits (512/256) passed
// POST: Target is read in as string
// WARNINGS: Large files may cause issues depending on user memory availability due to contiguity requirements.
void procInput(std::string inPath, std::string outPath, int bits) {
    std::ifstream inFile(inPath.c_str(), std::ios::binary);
    std::string bytes;
    if (inFile.good()) {
        char byte;
        while (inFile.get(byte)) {
            bytes += static_cast<char>(byte);
        }
        inFile.close();
    }
    else {
        std::cout << "Error: Could not open input file." << std::endl;
    }
    hashOut(bytes, outPath, bits);
}

// argHandler(uint32_t argc, std::vector<std::string> arguments)
// PRE: argcount, preprocessed arguments passed
// POST: Correct function is called in accordance with arguments
// WARNINGS: None
// NOTES: Yes, I know this is bad practice. For major combinations later on, a proper command pattern will be used.
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
            bool mode = (arguments[2] == "-cbc"); // 1 for cbc, 0 for ecb
            bool encOrDec = (arguments[3] == "-e"); // 1 for enc, 0 for dec
            uint32_t threads;
            std::string key = arguments[4].substr(3);
            std::string tarPath = arguments[5].substr(5);
            std::string outPath = arguments[6].substr(5);
            if (!mode) {
                threads = std::stoul(arguments[2].substr(5));
            }
            if (cipher) {
                Magma mag;
                if (mode) {
                    // cbc(mag), needs key, input, output
                    cbc(mag, encOrDec, key, tarPath, outPath);
                }
                else {
                    // ecb(mag)
                    ecb(mag, threads, encOrDec, key, tarPath, outPath);
                }
            }
            else {
                Kuznyechik kuz;
                if (mode) {
                    // cbc(kuz)
                    cbc(kuz, encOrDec, key, tarPath, outPath);
                }
                else {
                    // ecb(kuz)
                    ecb(kuz, threads, encOrDec, key, tarPath, outPath);
                }
            }
        }
        else {
            std_usage;
        }
        break;
    default:
        std_usage;
    }
}


// cbc(T& crypter, bool encOrDec, std::string key, std::string inputTar, std::string outputTar)
// PRE: Cipher object, encryption or decryption flag, key, input target, and output target are passed and valid
// POST: Target data is encrypted with relevant algorithm in CBC mode.
// WARNINGS: Will halt if invalid paths are given.
// NOTES: Proper polymorphism will be adopted for the cumulative project such that less risky type identification is used
template<typename T>
void cbc(T& crypter, bool encOrDec, std::string key, std::string inputTar, std::string outputTar) {
    uint32_t blockCtr = 0;  // magma can encrypt up to about 30 GB of data before needing to be rekeyed (2^32 blocks of 64 bits)
    uint32_t blockSize = 0;     // holds block size depending on algorithm passed
    std::string cbcPreviousOutput;  // holds the previous result to xor with current
    std::string cbcCurOutput; // current
    Streebog str; // streebog hasher for KDF-256
    std::ifstream inFile(inputTar, std::ios::binary); // read from here
    std::ofstream outFile(outputTar, std::ios::binary); // output to here
    std::string fileExtension = inputTar.substr(inputTar.find("."));
    std::random_device rd; // TRNG source for nonces (no need to be kept secret, will be inserted at beginning of file)

    // determine what we're doing operations with
    if (typeid(crypter) == typeid(Magma)) {
        blockSize = 8;
    }
    else {
        blockSize = 16;
    }
    std::string inTarCpy = inputTar;
    std::reverse(inTarCpy.begin(), inTarCpy.end());
    std::string fileName = inTarCpy.substr(0, inTarCpy.find("/"));
    std::reverse(fileName.begin(), fileName.end());
    if (encOrDec) {
        // encrypting
        // grab filename
        if (blockSize == 16) {
            fileName += ".kuz-cbc";
            std::cout << "This implementation of Kuznyechik is slow (~5.5 bytes/s)." << std::endl;
            std::cout << "Please be patient or reconsider its use for large files." << std::endl;
        }
        else {
            fileName += ".mag-cbc";
        }
        std::string outFileFullPath = outputTar + fileName;
        outFile.open(outFileFullPath.c_str(), std::ios::binary);
    }
    else {
        fileName = fileName.substr(0, fileName.length() - 8); // corto always uses 8 characters including . for filetypes
        outFile.open((outputTar + fileName).c_str(), std::ios::binary);
    }
    if (!outFile.good()) {
        std::cout << "Unable to open output file. Ensure appropriate write permissions exist." << std::endl;
        return;
    }
    std::cout << "Generating keys..." << std::endl;
    std::list<std::string> keyList;
    for (char character : key) {
        keyList.push_back(std::bitset<8>(character).to_string());
    }
    std::string derivedKey = str.hash(keyList, 256);
    BinNum passableKey(derivedKey, 256, 16);
    std::vector<BinNum> keys = crypter.keyScheduler(passableKey);
    std::cout << "Done." << std::endl;

    BinNum nonce("0", 1);
    if (encOrDec) {
        std::cout << "Generating nonce..." << std::endl;
        if (rd.entropy() == 0) { 
            std::cout << "CBC is not supported by your CPU. Try ECB?" << std::endl; 
            return;
        }
        if (blockSize == 8) {
            std::string nonceFirst = std::bitset<32>(rd()).to_string();
            std::string nonceLast = std::bitset<32>(rd()).to_string();
            nonce = BinNum(nonceFirst + nonceLast, 64);
        }
        else {
            std::string binNonce;
            for (uint32_t i = 0; i < 4; ++i) {
                binNonce += std::bitset<32>(rd()).to_string();
            }
            nonce = BinNum(binNonce, 128);
        }
        std::cout << "Done." << std::endl;
    }

    if (inFile.good()) {
        auto buf = std::string(blockSize, '\0');
        std::list<BinNum> inFileContents;
        // read file data into a list
        while (inFile.read(&buf[0], blockSize) || inFile.gcount() > 0) {
            uint32_t curGCount = inFile.gcount();
            std::string toBinNum;
            bool eof = inFile.peek() == EOF;
            if (encOrDec && curGCount > 0 && inFile.peek() == EOF) {
                // pad with null bytes
                uint32_t index = curGCount;
                while (index != blockSize) {
                    buf[index] = '\0';
                    ++index;
                }
            }
            for (char ch : buf) {
                toBinNum += std::bitset<8>(ch).to_string();
            }
            inFileContents.push_back(BinNum(toBinNum));
        }
        if (encOrDec) {
            // encrypting
            std::list<BinNum>::iterator it = inFileContents.begin();
            // output nonce
            if (blockSize == 8) {
                uint64_t value = std::stoull(nonce.getVal(), nullptr, 2);
                unsigned char bytes[8];
                for (uint32_t i = 0; i < sizeof(value); ++i) {
                    bytes[i] = (value >> ((7 - i) * 8)) & 0xFF;
                }
                for (int i = 0; i < 8; ++i) {
                    outFile << bytes[i];
                }
            }
            else {
                uint64_t val1 = std::stoull(nonce.getVal().substr(0, 64), nullptr, 2);
                uint64_t val2 = std::stoull(nonce.getVal().substr(64, 64), nullptr, 2);
                unsigned char bytes[16];
                for (uint32_t i = 0; i < sizeof(val1); ++i) {
                    bytes[i] = (val1 >> ((7 - i) * 8)) & 0xFF;
                }
                for (uint32_t i = 0; i < sizeof(val2); ++i) {
                    bytes[i + 8] = (val2 >> ((7 - i) * 8)) & 0xFF;
                }
                for (uint32_t i = 0; i < 16; ++i) {
                    outFile << bytes[i];
                }
            }
            BinNum prev = nonce;
            while (it != inFileContents.end()) {
                BinNum current = *it;
                BinNum toEnc = current ^ prev;
                BinNum encrypted = crypter.encrypt(toEnc, keys);
                // output encrypted
                if (blockSize == 8) {
                    uint64_t value = std::stoull(encrypted.getVal(), nullptr, 2);
                    unsigned char bytes[8];
                    for (uint32_t i = 0; i < sizeof(value); ++i) {
                        bytes[i] = (value >> ((7 - i) * 8)) & 0xFF;
                    }
                    for (int i = 0; i < 8; ++i) {
                        outFile << bytes[i];
                    }
                }
                else {
                    uint64_t val1 = std::stoull(encrypted.getVal().substr(0, 64), nullptr, 2);
                    uint64_t val2 = std::stoull(encrypted.getVal().substr(64, 64), nullptr, 2);
                    unsigned char bytes[16];
                    for (uint32_t i = 0; i < sizeof(val1); ++i) {
                        bytes[i] = (val1 >> ((7 - i) * 8)) & 0xFF;
                    }
                    for (uint32_t i = 0; i < sizeof(val2); ++i) {
                        bytes[i + 8] = (val2 >> ((7 - i) * 8)) & 0xFF;
                    }
                    for (uint32_t i = 0; i < 16; ++i) {
                        outFile << bytes[i];
                    }
                }
                ++it;
                prev = encrypted;
            }
            outFile.close();
        }
        else {
            // decrypting
            std::list<BinNum>::iterator it = inFileContents.end();
            --it; // go to first valid block
            BinNum cur = *it;
            BinNum prev = *(--it);
            std::list<BinNum> decryptedList;
            while (it != inFileContents.begin()) {
                BinNum decrypted = crypter.decrypt(cur, keys);
                BinNum plaintext = decrypted ^ prev;
                decryptedList.push_front(plaintext);
                cur = prev;
                prev = *(--it);
            }
            // it = nonce
            BinNum nonce = *it;
            BinNum startVal = *(++it);
            decryptedList.push_front(crypter.decrypt(startVal, keys) ^ nonce);
            for (BinNum cur : decryptedList) {
                if (blockSize == 8) {
                    uint64_t value = std::stoull(cur.getVal(), nullptr, 2);
                    unsigned char bytes[8];
                    for (uint32_t i = 0; i < sizeof(value); ++i) {
                        bytes[i] = (value >> ((7 - i) * 8)) & 0xFF;
                    }
                    for (int i = 0; i < 8; ++i) {
                        outFile << bytes[i];
                    }
                }
                else {
                    uint64_t val1 = std::stoull(cur.getVal().substr(0, 64), nullptr, 2);
                    uint64_t val2 = std::stoull(cur.getVal().substr(64, 64), nullptr, 2);
                    unsigned char bytes[16];
                    for (uint32_t i = 0; i < sizeof(val1); ++i) {
                        bytes[i] = (val1 >> ((7 - i) * 8)) & 0xFF;
                    }
                    for (uint32_t i = 0; i < sizeof(val2); ++i) {
                        bytes[i + 8] = (val2 >> ((7 - i) * 8)) & 0xFF;
                    }
                    for (uint32_t i = 0; i < 16; ++i) {
                        outFile << bytes[i];
                    }
                }
            }
            outFile.close();
        }
    }
}

// ecb(T& crypter, uint32_t threads, bool encOrDec, std::string key, std::string inputTar, std::string outputTar)
// PRE: Cipher object, number of threads, encryption or decryption flag, key, input target, and output target are passed and valid
// POST: Target data is encrypted with relevant algorithm in ECB mode with n threads.
// WARNINGS: Will halt if invalid paths are given. Unintentional behavior may arise from bad thread input.
// NOTES: Proper polymorphism will be adopted for the cumulative project such that less risky type identification is used
template<typename T>
void ecb(T& crypter, uint32_t threads, bool encOrDec, std::string key, std::string inputTar, std::string outputTar) {
    Streebog str; 
    uint32_t blockCtr = 0;
    int blockSize;
    std::vector<std::thread> threadPool;
    std::vector<BinNum> blocks;
    std::mutex mtx;
    std::ifstream inFile(inputTar.c_str(), std::ios::binary);
    std::ofstream outFile;

    if (threads == 0) {
        return;
    }

    if (typeid(crypter) == typeid(Magma)) {
        blockSize = 8;
    }
    else {
        blockSize = 16;
    }

    // determine file types, names, etc.
    std::string inTarCpy = inputTar;
    std::reverse(inTarCpy.begin(), inTarCpy.end());
    std::string fileName = inTarCpy.substr(0, inTarCpy.find("/"));
    std::reverse(fileName.begin(), fileName.end());
    if (encOrDec) {
        // encrypting
        // grab filename
        if (blockSize == 16) {
            fileName += ".kuz-ecb";
            std::cout << "This implementation of Kuznyechik is slow (~5.5 bytes/s)." << std::endl;
            std::cout << "Please be patient or reconsider its use for large files." << std::endl;
        }
        else {
            fileName += ".mag-ecb";
        }
        std::string outFileFullPath = outputTar + fileName;
        outFile.open(outFileFullPath.c_str(), std::ios::binary);
    }
    else {
        fileName = fileName.substr(0, fileName.length() - 8); // corto always uses 8 characters including . for filetypes
        outFile.open((outputTar + fileName).c_str(), std::ios::binary);
    }

    if (!outFile.good()) {
        std::cout << "Unable to open output file. Ensure appropriate write permissions exist." << std::endl;
        return;
    }

    std::cout << "Generating keys..." << std::endl;
    std::list<std::string> keyList;
    for (char character : key) {
        keyList.push_back(std::bitset<8>(character).to_string());
    }
    std::string derivedKey = str.hash(keyList, 256);
    BinNum passableKey(derivedKey, 256, 16);
    std::vector<BinNum> keys = crypter.keyScheduler(passableKey);
    std::cout << "Keys generating. Operating..." << std::endl;
    if (inFile.good()) {
        std::vector<std::string> buf;
        char byte;
        while (inFile.get(byte)) {
            buf.push_back(std::bitset<8>(byte).to_string());
            if (inFile.peek() == EOF && buf.size() != blockSize) {
                for (uint32_t i = 0; i < blockSize - buf.size() + 2; ++i) {
                    buf.push_back(std::bitset<8>('\0').to_string());
                }
            }
            // check block count (birthday paradox rekey prompt for Magma)
            // (~30GB, several thousand PB for Kuznyechik so no check)
            if (blockCtr == UINT32_MAX && blockSize == 8) {
                std::string newKey;
                std::cout << "Magma block limit hit. Please rekey to continue.\nNew key: " << std::endl;
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
            // check if we've formed a full block
            if (buf.size() == blockSize) {
                // concat vector
                std::string toBinNum;
                for (std::string curByte : buf) {
                    toBinNum += curByte;
                }
                // push back now complete block into block container
                blocks.push_back(BinNum(toBinNum, blockSize * 8));
                buf.clear();
                ++blockCtr;
            }
            // if all threads can be occupied or EOF, run it
            if (blocks.size() == threads || inFile.peek() == EOF) {
                std::vector<BinNum> results;
                results.reserve(blocks.size());
                results.resize(blocks.size(), BinNum("0", 1));
                for (uint32_t i = 0; i < blocks.size(); ++i) {
                    // lambdas for function call parity w/ PBR in threads
                    if (encOrDec) {
                        threadPool.push_back(std::thread([&crypter, keys, blocks, i, &results, &mtx]() {
                            crypter.threadEncrypt(keys, blocks[i], i, results, mtx);
                            }));
                    }
                    else {
                        threadPool.push_back(std::thread([&crypter, keys, blocks, i, &results, &mtx]() {
                            crypter.threadDecrypt(keys, blocks[i], i, results, mtx);
                            }));
                    }
                }
                for (uint32_t i = 0; i < threadPool.size(); ++i) {
                    threadPool[i].join();
                }
                threadPool.clear();
                std::cout << "Writing blocks " << blockCtr - blockSize << "-" << blockCtr << "..." << std::endl;
                for (auto data : results) {
                    // once a thread cycle is completely ran, output the data
                    if (blockSize == 8) {
                        uint64_t value = std::stoull(data.getVal(), nullptr, 2);
                        unsigned char bytes[8];
                        for (uint32_t i = 0; i < sizeof(value); ++i) {
                            bytes[i] = (value >> ((7 - i) * 8)) & 0xFF;
                        }
                        for (int i = 0; i < 8; ++i) {
                            outFile << bytes[i];
                        }
                    }
                    else {
                        uint64_t val1 = std::stoull(data.getVal().substr(0, 64), nullptr, 2);
                        uint64_t val2 = std::stoull(data.getVal().substr(64, 64), nullptr, 2);
                        unsigned char bytes[16];
                        for (uint32_t i = 0; i < sizeof(val1); ++i) {
                            bytes[i] = (val1 >> ((7 - i) * 8)) & 0xFF;
                        }
                        for (uint32_t i = 0; i < sizeof(val2); ++i) {
                            bytes[i + 8] = (val2 >> ((7 - i) * 8)) & 0xFF;
                        }
                        for (uint32_t i = 0; i < 16; ++i) {
                            outFile << bytes[i];
                        }
                    }
                }
                blocks.clear();
            }
        }
        outFile.close();
    }
    else {
        std::cout << "Failed to find target input file." << std::endl;
    }
}

// printInfo()
// PRE: Called
// POST: Usage information outputted
// WARNINGS: None
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
    std::cout << "Usage: ./Corto -[kuz/mag] -[cbc/ecb=N] -[e/d] -k=\"<KEY_HERE>\" -tgt=\"<ENC_TRGT_PTH>\" -out=\"<OUTPUT_PTH>\"" << std::endl;
    std::cout << "-[kuz/mag]\t\t\t- Denotes Kuznyechik (*.kuz) or Magma (*.mag) operation." << std::endl;
    std::cout << "-[cbc/ecb=N]\t\t\t- Denotes block cipher mode of operation. Uses N threads if ECB is selected." << std::endl;
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

// printInfo()
// PRE: Called
// POST: Information outputted
// WARNINGS: None
void printInfo() {
    std::cout << "                      ______           __      "                                  << std::endl;
    std::cout << "                     / ____/___  _____/ /_____ "                                  << std::endl;
    std::cout << "                    / /   / __ \\/ ___/ __/ __ \\"                                << std::endl;
    std::cout << "                   / /___/ /_/ / /  / /_/ /_/ /"                                  << std::endl;
    std::cout << "                   \\____/\\____/_/   \\__/\\____/ "                              << std::endl;
    std::cout << "*******************************************************************    "          << std::endl;
    std::cout << "      Written by 1nfocalypse. https://github.com/1nfocalypse      *    "          << std::endl;
    std::cout << "*******************************************************************    "          << std::endl;
    std::cout << "Corto is a complete implementation of the Russian Federation's    *    "          << std::endl;
    std::cout << "GOST cryptographic standards, to include GOST R 34.11-2012        *    "          << std::endl;
    std::cout << "\"Streebog\" and GOST R 34.12 - 2015 \"Kuznyechik\" and \"Magma\".\t  *"          << std::endl;
    std::cout << "Streebog is a 256 and 512 bit cryptographic hash, with Corto      *    "          << std::endl;
    std::cout << "supporting both file and string literal inputs. Kuznyechik is     *    "          << std::endl;
    std::cout << "a SP-Net based 128 bit block cipher occurring over 10 rounds,     *    "          << std::endl;
    std::cout << "while Magma is a Feistel Network based 64 bit block cipher        *    "          << std::endl;
    std::cout << "that utilizes 32 rounds and a substition-rotation round function. *    "          << std::endl;
    std::cout << "Both ciphers utilize 256 bit keys. All functions are considered   *    "          << std::endl;
    std::cout << "cryptographically weak compared to other available systems, but   *    "          << std::endl;
    std::cout << "are still in use and officially supported by the Russian          *    "          << std::endl;
    std::cout << "Federation.                                                       *    "          << std::endl;
    std::cout << "*******************************************************************    "          << std::endl;
    std::cout << "The Corto project is licensed under the MIT license."                             << std::endl;
    std::cout << "For more information, please consult the repo available at"                       << std::endl;
    std::cout << "https://github.com/1nfocalpyse/Corto"                                             << std::endl;
}
