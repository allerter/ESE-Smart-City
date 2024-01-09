#include <Wire.h>

//--------------------Definition of the structs--------------------

//Struct for a task
struct Task {
    byte id;
    unsigned int period;  //Time period in ms
    unsigned int executionTime;  //Last execution duration in us
    unsigned long lastExecutionTime;  //Last execution time in ms
    void (*taskFunction)();  //Function that should be executed in the task
};

//Struct for a message
struct Message {
  byte receiver; //i2c address of the receiving node
  String message; //String that contains the message to send
  byte tx_attempts; //Counter to count TX attempts
};

//Struct for EndNodes
struct EndNode {
  byte i2c_address;
  char location;
  bool emergencyApproaching;
};


//--------------------Function prototypes--------------------

//Tasks
void task1_communication();
void task2_logic();
void task3_status();

//Helping functions
bool addPacketTX(char location, String message);
bool addErrorMsg(String errorMsg);

//RMS related functions
void executeTask(Task* task);
void runRMS(Task tasks[], byte num_of_tasks);



//--------------------General global variables--------------------

//Definition of the task set
Task taskSet[] = {
    {1, 100, 0, 0, task1_communication},
    {2, 1000, 0, 0, task2_logic},
    {3, 2000, 0, 0, task3_status}
};
const byte numTasks = sizeof(taskSet) / sizeof(taskSet[0]);

//Definition of EndNodes with i2c address and location
EndNode endNodes[] = {
  {20, 'n', false},
  {21, 's', false},
  {22, 'w', false},
  {23, 'e', false}
};
const byte numEndNodes = sizeof(endNodes) / sizeof(endNodes[0]);


//--------------------Implementation of the task functions--------------------

//Task 1: used for communication between the TrafficManager and the EndNodes

//Defines for task 1
#define TX_BUFF_SIZE 16
#define RX_BUFF_SIZE 8
#define MAX_NUM_OF_TX_ATTEMPTS 2

//Global vars for task 1
Message tx_buff[TX_BUFF_SIZE];
Message rx_buff[RX_BUFF_SIZE];

void task1_communication()
{
  //Send messages to EndNodes that are stored in an array
  for(byte i = 0; i < TX_BUFF_SIZE; i++)
  {
    //Continue if message in array contains no content
    if(tx_buff[i].receiver == 0 && tx_buff[i].message == "")
    {
      continue;
    }
    
    //Starting transmission to EndNode specified in the message
    Wire.beginTransmission(tx_buff[i].receiver);

    //Writing the message to the buffer
    Wire.write(tx_buff[i].message.c_str());
    
    //Beginning real transmission and checking if successfully transmitted. If not successfully, and number of retransmissions below threshold keep in buffer and increment number of attempts
    byte result = Wire.endTransmission();

    //Handle i2c TX error
    if(result != 0)
    {
      //Adding error msg
      addErrorMsg("i2c Error: " + String(result) + " with slave " + String(tx_buff[i].receiver));

      //Incrementing number of attempts
      tx_buff[i].tx_attempts++;

      //Checking if number of allowed attempts are exceeded and continuing if not
      if(tx_buff[i].tx_attempts < MAX_NUM_OF_TX_ATTEMPTS)
      {
        continue; 
      }
      else
      {
        addErrorMsg("Exceeded allowed attempts! Message deleted.");
      }
    }
    
    //Clearing the message because it was sent successfully or exceeded number of allowed attempts
    tx_buff[i].receiver = 0;
    tx_buff[i].message = "";
    tx_buff[i].tx_attempts = 0;
  }
  
  //Polling if there are emergency vehicles apporaching
  for(byte i = 0; i < numEndNodes; i++)
  {
    byte result_size = Wire.requestFrom((int)endNodes[i].i2c_address, 1);
    
    //If something went wrong
    if(result_size != 1 || Wire.available() != 1)
    {
      addErrorMsg("Polling error with slave " + String(endNodes[i].i2c_address));
      continue;
    }

    //Read in char
    char rx = Wire.read();
    
    //Write result to end node
    if(rx == '1')
    {
      endNodes[i].emergencyApproaching = true;
    }
    else if(rx == '0')
    {
      
    }
    else
    {
      addErrorMsg("Result of polling slave " + String(endNodes[i].i2c_address) + " returned forbidden value");
    }
  }
}


//Task 2: used for implementing a state machine for the traffic light logic

//Defines for task 2
/* --empty-- */

//Global vars for task 2
byte time_cntr = 0;
byte state = 0;
char emergencyNorthSouth = ' ';
char emergencyEastWest = ' ';

void task2_logic()
{
  //Incrementing timebase
  time_cntr = time_cntr + 1;

  //Checking emergency vehicles
  for(byte i = 0; i < numEndNodes; i++)
  {
    if(!endNodes[i].emergencyApproaching)
    {
      continue;
    }
    
    if(endNodes[i].location == 'n' || endNodes[i].location == 's')
    {
      if(emergencyNorthSouth == ' ')
      {
          emergencyNorthSouth = endNodes[i].location;
          Serial.println("Set emergencyNS to: " + String(emergencyNorthSouth));
      }
      else
      {
        if(emergencyNorthSouth != endNodes[i].location)
        {
          emergencyNorthSouth = ' ';
          Serial.println("Reset emergencyNS by: " + String(endNodes[i].location));
        }
      }
    }
    else if (endNodes[i].location == 'e' || endNodes[i].location == 'w')
    {
      if(emergencyEastWest == ' ')
      {
          emergencyEastWest = endNodes[i].location;
          Serial.println("Set emergencyEW to: " + String(emergencyEastWest));
      }
      else
      {
        if(emergencyEastWest != endNodes[i].location)
        {
          emergencyEastWest = ' ';
          Serial.println("Reset emergencyEW by: " + String(endNodes[i].location));
        }
      }
    }
    endNodes[i].emergencyApproaching = false;
  }
  
  switch(state)
  {
    //Initial state
    case 0:
      state = 1;
      time_cntr = 0;
    break;

    //State with every light off
    //Used for starting and yellow blinking
    case 1:
      addPacketTX('n', "off");
      addPacketTX('s', "off");
      addPacketTX('e', "off");
      addPacketTX('w', "off");

      state = 2;
    break;

    //State with every yellow light on.
    //Used for yellow blinking and transition to all directions red
    case 2:
      if(time_cntr <= 6)
      {
        addPacketTX('n', "y");
        addPacketTX('s', "y");
        addPacketTX('e', "y");
        addPacketTX('w', "y");
      }
      
      if(time_cntr < 6)
      {
        state = 1;
      }
      else if(time_cntr >= 8)
      {
        state = 3;
        time_cntr = 0;
      }
    break;

    //State for every direction blocked
    case 3:
      if(time_cntr == 1)
      {
        addPacketTX('n', "r");
        addPacketTX('s', "r");
        addPacketTX('e', "r");
        addPacketTX('w', "r"); 
      }
      else if(time_cntr >= 3)
      {
        state = 4;
        time_cntr = 0;
      }
    break;

    //State for showing red-yellow in direction north-south
    case 4:
      if(time_cntr == 1)
      {
        addPacketTX('n', "r_y");
        addPacketTX('s', "r_y");
      }
      else if(time_cntr >= 1)
      {
        state = 5;
        time_cntr = 0;
      }
    break;

    //State for showing green in direction north-south
    case 5:
      if(time_cntr == 1)
      {
        addPacketTX('n', "g");
        addPacketTX('s', "g");
      }

      if((time_cntr >= 15 && emergencyNorthSouth == ' ') || emergencyEastWest != ' ')
      {
          state = 6;
          time_cntr = 0;
      }
    break;

    //State for showing yellow in direction north-south
    case 6:
      if(time_cntr == 1)
      {
        addPacketTX('n', "y");
        addPacketTX('s', "y");
      }
      else if(time_cntr >= 2)
      {
        state = 7;
        time_cntr = 0; 
      }
    break;

    //State for showing red in direction north-south
    case 7:
      if(time_cntr == 1)
      {
        addPacketTX('n', "r");
        addPacketTX('s', "r");
      }
      else if(time_cntr >= 2)
      {
        state = 8;
        time_cntr = 0; 
      }
    break;

    //State for showing red-yellow in direction west-east
    case 8:
      if(time_cntr == 1)
      {
        addPacketTX('w', "r_y");
        addPacketTX('e', "r_y");
      }
      else if(time_cntr >= 1)
      {
        state = 9;
        time_cntr = 0; 
      }
    break;

    //State for showing green in direction west-east
    case 9:
      if(time_cntr == 1)
      {
        addPacketTX('w', "g");
        addPacketTX('e', "g");
      }
      else if((time_cntr >= 15 && emergencyEastWest == ' ') || emergencyNorthSouth != ' ')
      {
        state = 10;
        time_cntr = 0; 
      }
    break;

    //State for showing yellow in direction west-east
    case 10:
      if(time_cntr == 1)
      {
        addPacketTX('w', "y");
        addPacketTX('e', "y");
      }
      else if(time_cntr >= 2)
      {
        state = 3;
        time_cntr = 0; 
      }
    break;
    
    default:
    break;
  }
}


//Task 3: used for printing the system's status

//Defines for task 3
#define ERROR_MSG_BUFFER_SIZE 16

//Global vars for task 3
String error_msg_buff[ERROR_MSG_BUFFER_SIZE];

void task3_status()
{
  //Iterating over the msg buffer
  for(byte i = 0; i < ERROR_MSG_BUFFER_SIZE; i++)
  {
    //Exiting if buffer is empty
    if(error_msg_buff[i] == "")
    {
      break;
    }

    //Printing and clearing msg buffer
    Serial.println(error_msg_buff[i]);
    error_msg_buff[i] = "";
  }
  
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}


//--------------------Setup and endless loop--------------------
void setup()
{
  //Setting the pin of the built in LED to be an output
  pinMode(LED_BUILTIN, OUTPUT);

  //Setting up serial communication with 115200 baud
  Serial.begin(115200);

  //Setting up i2c communication as a master
  Wire.begin();
}

void loop()
{
  //Calling the scheduler with the task set
  runRMS(taskSet, numTasks);
}


//--------------------Helping functions--------------------

//Function to add message into the TX buffer. Returns true if successful or false if buffer was full
bool addPacketTX(char location, String message)
{
  short indexOfdestNode = -1;
  for(byte i = 0; i < numEndNodes; i++)
  {
    if(endNodes[i].location == location)
    {
      indexOfdestNode = i;
      break;
    }
  }

  //If no node was found return false
  if(indexOfdestNode == -1)
  {
    return false;
  }
  
  //Iterating over the buffer
  for(byte i = 0; i < TX_BUFF_SIZE; i++)
  {
    //Continue if message in array contains content
    if(tx_buff[i].receiver != 0 || tx_buff[i].message != "")
    {
      continue;
    }

    //Clearing the message because it was sent successfully or exceeded number of allowed attempts
    tx_buff[i].receiver = endNodes[indexOfdestNode].i2c_address;
    tx_buff[i].message = message;

    //Returning true if message was put into the buffer successfully
    return true;
  }

  //Returning false otherwise
  return false;
}


//Function to add a message to the buffer. Returns true if successful or false if buffer was full
bool addErrorMsg(String errorMsg)
{
  //Iterating over the msg buffer
  for(byte i = 0; i < ERROR_MSG_BUFFER_SIZE; i++)
  {
    //Continue if memory is not empty
    if(error_msg_buff[i] != "")
    {
      continue;
    }

    //Otherwise put msg into buffer and return true
    error_msg_buff[i] = errorMsg;
    return true;
  }

  //Return false if buffer was full
  return false;
}


//--------------------RMS-Functions--------------------

//Method to run the scheduler with a task set.
//This needs to be called in the endless loop.
void runRMS(Task tasks[], byte num_of_tasks)
{
  Task* task_to_be_executed_ptr = nullptr;

  //Getting the current time
  unsigned long current_time = millis();

  //Iterating over the tasks, finding the task that needs to be executed with the highest priority
  for(byte i = 0; i < num_of_tasks; i++)
  {
    //If tasks do not need to be executed --> skip
    if(current_time - tasks[i].lastExecutionTime < tasks[i].period)
    {
      continue;
    }

    if(task_to_be_executed_ptr == nullptr)
    {
      //If there was no task found that needs to be executed, store the pointer
      task_to_be_executed_ptr = &tasks[i];
    }
    else
    {
      //Replace the pointer if the period is shorter (--> RMS)
      if(tasks[i].period < task_to_be_executed_ptr->period)
      {
        task_to_be_executed_ptr = &tasks[i];
      }
    }
  }

  //If a task that needs to be executed was found, execute it
  if(task_to_be_executed_ptr != nullptr)
  {
    task_to_be_executed_ptr->lastExecutionTime = current_time;
    executeTask(task_to_be_executed_ptr);
  }
}

//Method to execute the function of a task
void executeTask(Task* task)
{
  Serial.println("Executing task " + String(task->id));

  //Getting the time before execution (for measuring execution time)
  unsigned long time_before_exec = micros();

  //Executing the task
  task->taskFunction();

  //Getting the time after execution (for measuring execution time)
  unsigned long time_after_exec = micros();

  //Calculating the execution duration and storing it
  task->executionTime = time_after_exec - time_before_exec;

  Serial.println("Completed execution of task " + String(task->id) + " (took " + String(task->executionTime) + "us)");
}
