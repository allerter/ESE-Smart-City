#include <iostream>
#include <cstring>
#include <vector>
#include <assert.h>
#include "function.h"

// Test Case 1: Successful transmission
void testSuccessfulTransmisions(){
    std::cout << "Running testSuccessfulTransmisions() ";
    std::vector<Message> tx_buff1 = {Message(1, "Hello", 0)};
    task1_communication_part1(tx_buff1);
    // Check if tx_buff is cleared
    assert(tx_buff1[0].receiver == 0);
    assert(tx_buff1[0].message == "");
    assert(tx_buff1[0].tx_attempts == 0);
    std::cout << "PASSED \n";
}

// Test Case 2: I2C error on the first attempt
void testFailedFirstAttempt(){
    std::cout << "Running testFailedFirstAttempt() ";

    std::vector<Message> tx_buff2 = {Message(2, "Error", 1)};
    try {
        task1_communication_part1(tx_buff2);
    } catch (const ComException& e) {
        bool expectedMsgIsCorrect = std::strcmp(e.what(), "i2c Error: 1 with slave 2") == 0;
        assert(expectedMsgIsCorrect);
        assert(tx_buff2[0].receiver == 2);
        assert(tx_buff2[0].message == "Error");
        assert(tx_buff2[0].tx_attempts == 1);
    }
    std::cout << "PASSED \n";
}

// Test Case 3: I2C error with maximum number of attempts reached
void testFailedSecondAttempt(){
    std::cout << "Running testFailedSecondAttempt() ";
    std::vector<Message> tx_buff3 = {Message(3, "MaxAttempts", 2)};
    try {
        task1_communication_part1(tx_buff3);
    } catch (const ComException& e) {
    }
    try {
        task1_communication_part1(tx_buff3);
    } catch (const ComException& e) {
        // Check if exception is caught and tx_buff is cleared
        bool expectedMsgIsCorrect = std::strcmp(e.what(), "Exceeded allowed attempts! Message deleted.") == 0;
        assert(expectedMsgIsCorrect);
        assert(tx_buff3[0].receiver == 3);
        assert(tx_buff3[0].message == "MaxAttempts");
        assert(tx_buff3[0].tx_attempts == 2);
    }
    std::cout << "PASSED \n";
}

// Test Case 3: I2C error with maximum number of attempts reached
// Test Case 4: Empty tx_buff
void testEmptyBuff(){
    std::cout << "Running testEmptyBuff() ";
    std::vector<Message> tx_buff4;
    task1_communication_part1(tx_buff4);  // Should not throw any exception
    std::cout << "PASSED \n";
}

int main() {
    testSuccessfulTransmisions();
    testFailedFirstAttempt();
    testFailedSecondAttempt();
    testEmptyBuff();
    return 0;
}
