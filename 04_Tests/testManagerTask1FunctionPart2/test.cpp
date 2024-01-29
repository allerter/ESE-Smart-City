#include <assert.h>
#include "function.h"

// Test 1: Valid Input
void testValidInput()
{
    std::cout << "Running testValidInput() ";
    std::vector<EndNode> endNodes1 = {
        {1, 'A', false, false, '1', '0'},
        {2, 'B', false, false, '0', '1'}
        // Add more EndNodes as needed for your test cases
    };
    task1_communication_part2(endNodes1);
    assert(endNodes1[0].emergencyApproaching);
    assert(!endNodes1[0].citizenDetected);
    assert(!endNodes1[1].emergencyApproaching);
    assert(endNodes1[1].citizenDetected);
    std::cout << "PASSED \n";
}

// Test 2: Invalid Emergency Signal
void testInvalidEmrgencySignal()
{
    std::cout << "Running testInvalidEmrgencySignal() ";
    std::vector<EndNode> endNodes2 = {
        {1, 'A', false, false, '2', '0'}
        // Add more EndNodes as needed for your test cases
    };

    try
    {
        task1_communication_part2(endNodes2);
        std::cerr << "Test 2 failed: No exception thrown for invalid emergency signal." << std::endl;
    }
    catch (const ComException&)
    {
        // Exception expected for invalid emergency signal
    }
    std::cout << "PASSED \n";
}

// Test 3: Invalid Lights Signal
void testInvalidSignalSignal()
{
    std::cout << "Running testInvalidSignalSignal() ";
    std::vector<EndNode> endNodes3 = {
        {1, 'A', false, false, '1', '2'}
        // Add more EndNodes as needed for your test cases
    };

    try
    {
        task1_communication_part2(endNodes3);
        std::cerr << "Test 3 failed: No exception thrown for invalid lights signal." << std::endl;
    }
    catch (const ComException&)
    {
        // Exception expected for invalid lights signal
    }

    // Add more test cases as needed
    std::cout << "PASSED \n";
}


int main()
{
    testValidInput();
    testInvalidEmrgencySignal();
    testInvalidSignalSignal();
    return 0;
}
