#include <iostream>
#include <string>
#include <iomanip>
#include "BinNum.h"

// Helper function to compare expected and actual results
void checkResult(const std::string& testName, const BinNum& actual, const std::string& expected, bool expectException = false) {
    try {
        std::string actualStr = actual.getVal(); // Assuming `toString()` method exists to get the binary string representation
        if (expectException) {
            std::cout << testName << ": Failed (Exception expected but not thrown)" << std::endl;
        }
        else if (actualStr == expected) {
            std::cout << testName << ": Passed" << std::endl;
        }
        else {
            std::cout << testName << ": Failed (Expected " << expected << ", got " << actualStr << ")" << std::endl;
        }
    }
    catch (const std::exception& e) {
        if (expectException) {
            std::cout << testName << ": Passed (Exception thrown as expected)" << std::endl;
        }
        else {
            std::cout << testName << ": Failed (Exception thrown unexpectedly: " << e.what() << ")" << std::endl;
        }
    }
}

void testConstructorAndPrint() {
    try {
        BinNum bin1("101010");
        BinNum bin2("1100", 8);
        BinNum bin3("15", 8, 16);
        BinNum bin4("15", 8, 10);
        BinNum bin5("7FFFFFFF", 128, 16);
        BinNum bin6("7FFFFFFF", 128, 16);
        BinNum bin7 = bin5 + bin6;
        checkResult("Bin5 + 6", bin7, (bin5 * BinNum("10", 128)).getVal());
        std::cout << bin7.getVal(10) << std::endl;
        BinNum bin8((bin7 * bin7).getVal(), 256);
        BinNum bin9((bin8 * bin8).getVal(), 256);
        std::cout << bin9.getVal(10) << std::endl;

        checkResult("Constructor bin1", bin1, "101010");
        checkResult("Constructor bin2", bin2, "00001100");
        checkResult("Constructor bin3", bin3, "00010101");
        checkResult("Constructor bin4", bin4, "00001111");
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

void testAddition() {
    try {
        BinNum bin1("01010");
        BinNum bin2("00110");
        BinNum result = bin1 + bin2;

        checkResult("Addition", result, "10000"); // Adjust expected result if output is truncated
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

void testSubtraction() {
    try {
        BinNum bin1("1010");
        BinNum bin2("0110");
        BinNum result = bin1 - bin2;

        checkResult("Subtraction", result, "0100"); // Adjust expected result if output is truncated
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

void testMultiplication() {
    try {
        BinNum bin1("01010");
        BinNum bin2("00011");
        BinNum result = bin1 * bin2;

        checkResult("Multiplication", result, "11110"); // Adjust expected result if output is truncated
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

void testDivision() {
    try {
        BinNum bin1("1010");
        BinNum bin2("0010");
        BinNum result = bin1 / bin2;

        checkResult("Division", result, "0101"); // Adjust expected result if output is truncated
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

void testBitwiseOperations() {
    try {
        BinNum bin1("1010");
        BinNum bin2("1100");

        checkResult("Bitwise AND", bin1 & bin2, "1000");
        checkResult("Bitwise OR", bin1 | bin2, "1110");
        checkResult("Bitwise XOR", bin1 ^ bin2, "0110");
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

void testShiftOperations() {
    try {
        BinNum bin1("1010");

        checkResult("Left Shift by 2", bin1 << 2, "1000");
        checkResult("Right Shift by 2", bin1 >> 2, "0010");
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

void testComparisonOperators() {
    try {
        BinNum bin1("1010");
        BinNum bin2("0110");

        bool result;
        result = bin1 > bin2;
        std::cout << "bin1 > bin2: " << (result ? "Passed" : "Failed") << std::endl; // Expected output: Passed (true)

        result = bin1 < bin2;
        std::cout << "bin1 < bin2: " << (result ? "Failed" : "Passed") << std::endl; // Expected output: Failed (false)

        result = bin1 >= bin2;
        std::cout << "bin1 >= bin2: " << (result ? "Passed" : "Failed") << std::endl; // Expected output: Passed (true)

        result = bin1 <= bin2;
        std::cout << "bin1 <= bin2: " << (result ? "Failed" : "Passed") << std::endl; // Expected output: Failed (false)

        result = bin1 == bin2;
        std::cout << "bin1 == bin2: " << (result ? "Failed" : "Passed") << std::endl; // Expected output: Failed (false)

        result = bin1 != bin2;
        std::cout << "bin1 != bin2: " << (result ? "Passed" : "Failed") << std::endl; // Expected output: Passed (true)
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

void testEdgeCases() {
    try {
        // Test empty binary string
        try {
            BinNum bin1("");
            std::cout << "No string test: Failed (Exception expected)" << std::endl;
        }
        catch (const std::invalid_argument&) {
            std::cout << "No string test: Passed (Exception thrown as expected)" << std::endl;
        }
        catch (...) {
            std::cout << "No string test: Failed (Unexpected exception type)" << std::endl;
        }

        // Test binary string with invalid characters
        try {
            BinNum bin2("10A0");
            std::cout << "Invalid binary string test: Failed (Exception expected)" << std::endl;
        }
        catch (const std::invalid_argument&) {
            std::cout << "Invalid binary string test: Passed (Exception thrown as expected)" << std::endl;
        }
        catch (...) {
            std::cout << "Invalid binary string test: Failed (Unexpected exception type)" << std::endl;
        }

        // Test binary string with leading/trailing spaces
        try {
            BinNum bin3("  1010  ");
            std::cout << "Invalid spaced string test: Failed (Exception expected)" << std::endl;
        }
        catch (const std::invalid_argument&) {
            std::cout << "Invalid spaced string test: Passed (Exception thrown as expected)" << std::endl;
        }
        catch (...) {
            std::cout << "Invalid spaced string test: Failed (Unexpected exception type)" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

int main() {
    // Run all test cases
    testConstructorAndPrint();
    testAddition();
    testSubtraction();
    testMultiplication();
    testDivision();
    testBitwiseOperations();
    testShiftOperations();
    testComparisonOperators();
    testEdgeCases();

    return 0;
}