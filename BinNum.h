#pragma once
#include <string>

class BinNum {
private:
    uint32_t size;
    uint32_t base = 2;
    std::string myStr;

    // shiftLeft(unsigned int shift)
    // PRE: BinNum exists, shift is an unsigned int representing the number of positions to shift.
    // POST: Returns a new BinNum object that is the result of left-shifting the current BinNum by the specified number of positions.
    // WARNINGS: If the shift is greater than or equal to the size of the binary number, the result will be padded with zeros on the right.
    BinNum shiftLeft(unsigned int shift) const;

    // shiftRight(unsigned int shift)
    // PRE: BinNum exists, shift is an unsigned int representing the number of positions to shift.
    // POST: Returns a new BinNum object that is the result of right-shifting the current BinNum by the specified number of positions.
    // WARNINGS: If the shift is greater than or equal to the size of the binary number, the result will be padded with zeros on the left.
    BinNum shiftRight(unsigned int shift) const;

    // AND(const BinNum& other)
    // PRE: BinNum objects exist and have the same size.
    // POST: Returns a new BinNum object representing the bitwise AND operation between the current BinNum and the provided BinNum.
    // WARNINGS: Throws a length_error if the sizes of the two BinNum objects do not match.
    BinNum AND(const BinNum& other) const;

    // XOR(const BinNum& other)
    // PRE: BinNum objects exist and have the same size.
    // POST: Returns a new BinNum object representing the bitwise XOR operation between the current BinNum and the provided BinNum.
    // WARNINGS: Throws a length_error if the sizes of the two BinNum objects do not match.
    BinNum XOR(const BinNum& other) const;

    // OR(const BinNum& other)
    // PRE: BinNum objects exist and have the same size.
    // POST: Returns a new BinNum object representing the bitwise OR operation between the current BinNum and the provided BinNum.
    // WARNINGS: Throws a length_error if the sizes of the two BinNum objects do not match.
    BinNum OR(const BinNum& other) const;

    // ADD(const BinNum& other)
    // PRE: BinNum objects exist and have the same size.
    // POST: Returns a new BinNum object representing the result of binary addition of the current BinNum and the provided BinNum.
    // WARNINGS: Throws a length_error if the sizes of the two BinNum objects do not match, or an overflow_error if the result exceeds the size limit.
    BinNum ADD(const BinNum& other) const;

    // SUB(const BinNum& other)
    // PRE: BinNum objects exist and have the same size. The current BinNum must be greater than or equal to the provided BinNum.
    // POST: Returns a new BinNum object representing the result of binary subtraction of the provided BinNum from the current BinNum.
    // WARNINGS: Throws a length_error if the sizes of the two BinNum objects do not match, or an underflow_error if the current BinNum is less than the provided BinNum.
    BinNum SUB(const BinNum& other) const;

    // MULT(const BinNum& other)
    // PRE: BinNum objects exist and have the same size.
    // POST: Returns a new BinNum object representing the result of binary multiplication of the current BinNum and the provided BinNum.
    // WARNINGS: Throws an overflow_error if the result exceeds the size limit during intermediate addition operations.
    BinNum MULT(const BinNum& other) const;

    // DIV(const BinNum& other)
    // PRE: BinNum objects exist, have the same size, and the provided BinNum must not be zero.
    // POST: Returns a new BinNum object representing the result of binary division of the current BinNum by the provided BinNum.
    // WARNINGS: Throws an invalid_argument if the provided BinNum is zero, or a length_error if sizes do not match.
    BinNum DIV(const BinNum& other) const;

    // MOD(const BinNum& other)
    // PRE: BinNum objects exist, have the same size, and the provided BinNum must not be zero.
    // POST: Returns a new BinNum object representing the result of binary modulo operation of the current BinNum by the provided BinNum.
    // WARNINGS: Throws an invalid_argument if the provided BinNum is zero, or a length_error if sizes do not match.
    BinNum MOD(const BinNum& other) const;

    // GT(const BinNum& other)
    // PRE: BinNum objects exist and have the same size.
    // POST: Returns true if the current BinNum is greater than the provided BinNum, false otherwise.
    // WARNINGS: Throws a length_error if sizes do not match.
    bool GT(const BinNum& other) const;

    // LT(const BinNum& other)
    // PRE: BinNum objects exist and have the same size.
    // POST: Returns true if the current BinNum is less than the provided BinNum, false otherwise.
    // WARNINGS: Throws a length_error if sizes do not match.
    bool LT(const BinNum& other) const;

    // EQ(const BinNum& other)
    // PRE: BinNum objects exist and have the same size.
    // POST: Returns true if the current BinNum is equal to the provided BinNum, false otherwise.
    // WARNINGS: Throws a length_error if sizes do not match.
    bool EQ(const BinNum& other) const;

    // GEQ(const BinNum& other)
    // PRE: BinNum objects exist and have the same size.
    // POST: Returns true if the current BinNum is greater than or equal to the provided BinNum, false otherwise.
    // WARNINGS: Throws a length_error if sizes do not match.
    bool GEQ(const BinNum& other) const;

    // LEQ(const BinNum& other)
    // PRE: BinNum objects exist and have the same size.
    // POST: Returns true if the current BinNum is less than or equal to the provided BinNum, false otherwise.
    // WARNINGS: Throws a length_error if sizes do not match.
    bool LEQ(const BinNum& other) const;

    // decToBin(std::string decStr)
    // PRE: Valid string is passed.
    // POST: Valid string of correct base returned.
    // WARNINGS: None
    std::string decToBin(std::string decStr) const;

    // decToBin(std::string decStr)
    // PRE: Valid string is passed.
    // POST: Valid string of correct base returned.
    // WARNINGS: None
    std::string hexToBin(std::string hexStr) const;

    // decToBin(std::string decStr)
    // PRE: Valid string is passed.
    // POST: Valid string of correct base returned.
    // WARNINGS: None
    std::string binToDec(std::string bitStr) const;

    // decToBin(std::string decStr)
    // PRE: Valid string is passed.
    // POST: Valid string of correct base returned.
    // WARNINGS: Throws std::invalid_argument if the passed string's length mod 4 != 0
    std::string binToHex(std::string bitStr) const;

public:

    // explicit BinNum(std::string bitstr)
    // PRE: bitstr is a binary string containing only '0' and '1'.
    // POST: Constructs a BinNum object with the given binary string.
    // WARNINGS: Throws std::invalid_argument if bitstr is empty or contains non-binary characters.
    explicit BinNum(std::string bitstr);

    // BinNum(std::string bitstr, uint32_t bitSize)
    // PRE: bitstr is a binary string containing only '0' and '1'. bitSize is the desired bit size.
    // POST: Constructs a BinNum object with the given binary string, padded to bitSize if necessary.
    // WARNINGS: Throws std::invalid_argument if bitstr contains non-binary characters or if bitSize is less than the length of bitstr.
    BinNum(std::string bitstr, uint32_t bitSize);

    // BinNum(std::string number, uint32_t bitSize, uint32_t base)
    // PRE: number, bitSize, base passed. If bitSize is 0, it conforms to the passed number.
    // POST: Object created with binary representation stored with correct length. Base set to zero.
    // WARNINGS: Throws invalid argument if base other than 2, 10, 16 passed, if invalid string is passed.
    // Throws length error if insufficient bits provided to hold the passed number in binary.
    BinNum(std::string number, uint32_t bitSize, uint32_t base);

    // BinNum(const BinNum& other)
    // PRE: other is a valid BinNum object.
    // POST: Constructs a BinNum object as a copy of the provided BinNum object.
    // WARNINGS: None
    BinNum(const BinNum& other);

    // ~BinNum()
    // PRE: BinNum object exists.
    // POST: Destroys the BinNum object.
    // WARNINGS: None
    ~BinNum() = default;

    // BinNum& operator=(const BinNum& other)
    // PRE: This BinNum object and the provided BinNum object exist.
    // POST: Assigns the value of the provided BinNum object to this BinNum object.
    // WARNINGS: None
    BinNum& operator=(const BinNum& other);

    // BinNum operator<<(unsigned int shift) const
    // PRE: BinNum object exists and shift is an unsigned int representing the number of positions to shift.
    // POST: Returns a new BinNum object that is the result of left-shifting the current BinNum by the specified number of positions.
    // WARNINGS: None
    BinNum operator<<(unsigned int shift) const;

    // BinNum operator>>(unsigned int shift) const
    // PRE: BinNum object exists and shift is an unsigned int representing the number of positions to shift.
    // POST: Returns a new BinNum object that is the result of right-shifting the current BinNum by the specified number of positions.
    // WARNINGS: None
    BinNum operator>>(unsigned int shift) const;

    // BinNum operator^(const BinNum& other) const
    // PRE: BinNum objects exist and have the same size.
    // POST: Returns a new BinNum object representing the result of bitwise XOR operation between the current BinNum and the provided BinNum.
    // WARNINGS: Throws a length_error if the sizes of the two BinNum objects do not match.
    BinNum operator^(const BinNum& other) const;

    // BinNum operator&(const BinNum& other) const
    // PRE: BinNum objects exist and have the same size.
    // POST: Returns a new BinNum object representing the result of bitwise AND operation between the current BinNum and the provided BinNum.
    // WARNINGS: Throws a length_error if the sizes of the two BinNum objects do not match.
    BinNum operator&(const BinNum& other) const;

    // BinNum operator|(const BinNum& other) const
    // PRE: BinNum objects exist and have the same size.
    // POST: Returns a new BinNum object representing the result of bitwise OR operation between the current BinNum and the provided BinNum.
    // WARNINGS: Throws a length_error if the sizes of the two BinNum objects do not match.
    BinNum operator|(const BinNum& other) const;

    // BinNum operator+(const BinNum& other) const
    // PRE: BinNum objects exist and have the same size.
    // POST: Returns a new BinNum object representing the result of binary addition between the current BinNum and the provided BinNum.
    // WARNINGS: Throws a length_error if the sizes of the two BinNum objects do not match, or an overflow_error if the result exceeds the size limit.
    BinNum operator+(const BinNum& other) const;

    // BinNum operator-(const BinNum& other) const
    // PRE: BinNum objects exist and have the same size. The current BinNum must be greater than or equal to the provided BinNum.
    // POST: Returns a new BinNum object representing the result of binary subtraction of the provided BinNum from the current BinNum.
    // WARNINGS: Throws a length_error if the sizes of the two BinNum objects do not match, or an underflow_error if the current BinNum is less than the provided BinNum.
    BinNum operator-(const BinNum& other) const;

    // BinNum operator*(const BinNum& other) const
    // PRE: BinNum objects exist and have the same size.
    // POST: Returns a new BinNum object representing the result of binary multiplication of the current BinNum and the provided BinNum.
    // WARNINGS: Throws an overflow_error if the result exceeds the size limit during intermediate addition operations.
    BinNum operator*(const BinNum& other) const;

    // BinNum operator/(const BinNum& other) const
    // PRE: BinNum objects exist, have the same size, and the provided BinNum must not be zero.
    // POST: Returns a new BinNum object representing the result of binary division of the current BinNum by the provided BinNum.
    // WARNINGS: Throws an invalid_argument if the provided BinNum is zero, or a length_error if sizes do not match.
    BinNum operator/(const BinNum& other) const;

    // BinNum operator%(const BinNum& other) const
    // PRE: BinNum objects exist, have the same size, and the provided BinNum must not be zero.
    // POST: Returns a new BinNum object representing the result of binary modulo operation of the current BinNum by the provided BinNum.
    // WARNINGS: Throws an invalid_argument if the provided BinNum is zero, or a length_error if sizes do not match.
    BinNum operator%(const BinNum& other) const;

    // bool operator>(const BinNum& other) const
    // PRE: BinNum objects exist and have the same size.
    // POST: Returns true if the current BinNum is greater than the provided BinNum, false otherwise.
    // WARNINGS: Throws a length_error if sizes do not match.
    bool operator>(const BinNum& other) const;

    // bool operator<(const BinNum& other) const
    // PRE: BinNum objects exist and have the same size.
    // POST: Returns true if the current BinNum is less than the provided BinNum, false otherwise.
    // WARNINGS: Throws a length_error if sizes do not match.
    bool operator<(const BinNum& other) const;

    // bool operator>=(const BinNum& other) const
    // PRE: BinNum objects exist and have the same size.
    // POST: Returns true if the current BinNum is greater than or equal to the provided BinNum, false otherwise.
    // WARNINGS: Throws a length_error if sizes do not match.
    bool operator>=(const BinNum& other) const;

    // bool operator<=(const BinNum& other) const
    // PRE: BinNum objects exist and have the same size.
    // POST: Returns true if the current BinNum is less than or equal to the provided BinNum, false otherwise.
    // WARNINGS: Throws a length_error if sizes do not match.
    bool operator<=(const BinNum& other) const;

    // bool operator!=(const BinNum& other) const
    // PRE: BinNum objects exist and have the same size.
    // POST: Returns true if the current BinNum is not equal to the provided BinNum, false otherwise.
    // WARNINGS: Throws a length_error if sizes do not match.
    bool operator!=(const BinNum& other) const;

    // bool operator==(const BinNum& other) const
    // PRE: BinNum objects exist and have the same size.
    // POST: Returns true if the current BinNum is equal to the provided BinNum, false otherwise.
    // WARNINGS: Throws a length_error if sizes do not match.
    bool operator==(const BinNum& other) const;

    // void set(std::string other)
    // PRE: other is a binary string containing only '0' and '1'.
    // POST: Sets the value of the current BinNum to the given binary string.
    // WARNINGS: Throws std::invalid_argument if other contains non-binary characters.
    void set(std::string other);

    // void set(std::string other)
    // PRE: other is a binary string containing only '0' and '1'.
    // POST: Sets the value of the current BinNum to the given binary string.
    // WARNINGS: Throws std::invalid_argument if other contains invalid characters for the given base.
    void set(std::string other, uint32_t base);

    // void setBitSize(uint32_t bitSize)
    // PRE: bitSize is the desired bit size for the current BinNum.
    // POST: Sets the bit size of the current BinNum, adjusting its binary string if necessary.
    // WARNINGS: Throws std::invalid_argument if bitSize is less than the length of the current binary string.
    void setBitSize(uint32_t bitSize);

    // std::string getVal() const
    // PRE: BinNum object exists.
    // POST: Returns the binary string representation of the current BinNum.
    // WARNINGS: None
    std::string getVal() const;

    // std::string getVal(uint32_t base) const
    // PRE: BinNum object exists
    // POST: Returns value in given format
    // WARNINGS: Throws std::invalid_argument if improper base is passed.
    std::string getVal(uint32_t base) const;

    // uint32_t getSize() const
    // PRE: BinNum object exists.
    // POST: Returns the size of the binary string of the current BinNum.
    // WARNINGS: None
    uint32_t getSize() const;

    // void print() const
    // PRE: BinNum object exists.
    // POST: Prints the binary string representation of the current BinNum to the standard output.
    // WARNINGS: None
    void print() const;

};