#include "function.h"

#define MAX_NUM_OF_TX_ATTEMPTS 2

void task1_communication_part1(
    std::vector<Message>& tx_buff
){
    // Send messages to EndNodes that are stored in an array
    for (size_t i = 0; i < tx_buff.size(); i++) {
        // Continue if message in array contains no content
        if (tx_buff[i].receiver == 0 && tx_buff[i].message == "") {
            continue;
        }

        uint8_t result = tx_buff[i].testResponseInteger;
        if (result != 0){
            //Incrementing number of attempts
            tx_buff[i].tx_attempts++;


            //Checking if number of allowed attempts are exceeded and continuing if not
            if(tx_buff[i].tx_attempts < MAX_NUM_OF_TX_ATTEMPTS)
            {
                throw ComException(
                    "i2c Error: " + std::to_string(result) + " with slave " +  std::to_string(tx_buff[i].receiver)
                );
            }
            else
            {
                throw ComException("Exceeded allowed attempts! Message deleted.");
            }

        }


        // Clearing the message because it was sent successfully
        tx_buff[i].receiver = 0;
        tx_buff[i].message = "";
        tx_buff[i].tx_attempts = 0;
    }
}
