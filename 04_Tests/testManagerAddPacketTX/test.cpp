#include <iostream>
#include <vector>
#include <assert.h>
#include "function.h"

// Test Case 1: Successful transmission
void testSuccessfulTransmission() {
    std::cout << "Running testSuccessfulTransmission() ";
    std::vector<EndNode> endNodes1 = {EndNode('A', 1)};
    std::vector<Message> tx_buff1 = {Message(0, "")};
    bool result = addPacketTX(endNodes1, tx_buff1, 'A', "Hello");
    // Check if transmission was successful
    assert(result);
    assert(tx_buff1[0].receiver == 1);
    assert(tx_buff1[0].message == "Hello");
    std::cout << "PASSED \n";
}

// Test Case 2: Unsuccessful transmission (invalid destination node)
void testInvalidDestinationNode() {
    std::cout << "Running testInvalidDestinationNode() ";
    std::vector<EndNode> endNodes2 = {EndNode('B', 2)};
    std::vector<Message> tx_buff2 = {Message(0, "")};
    bool result = addPacketTX(endNodes2, tx_buff2, 'A', "Error");
    // Check if transmission was unsuccessful due to invalid destination node
    assert(!result);
    assert(tx_buff2[0].receiver == 0);
    assert(tx_buff2[0].message == "");
    std::cout << "PASSED \n";
}

// Test Case 3: Unsuccessful transmission (buffer full)
void testBufferFull() {
    std::cout << "Running testBufferFull() ";
    std::vector<EndNode> endNodes3 = {EndNode('C', 3)};
    std::vector<Message> tx_buff3 = {Message(0, "")};
    bool result1 = addPacketTX(endNodes3, tx_buff3, 'C', "Existing");
    // First transmission attempt should be successful
    assert(result1);
    bool result2 = addPacketTX(endNodes3, tx_buff3, 'C', "Overflow");
    // Check if second transmission attempt fails due to buffer being full
    assert(!result2);
    assert(tx_buff3[0].receiver == 3);
    assert(tx_buff3[0].message == "Existing");
    std::cout << "PASSED \n";
}

int main() {
    testSuccessfulTransmission();
    testInvalidDestinationNode();
    testBufferFull();
    return 0;
}
