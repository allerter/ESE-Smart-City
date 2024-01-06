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
};

//--------------------Function prototypes--------------------
//Tasks
void task1_communication();
void task2_logic();
void task3_status();

//Helping functions
bool addPacketTX(char location, String message);
void addErrorMsg(String errorMsg);

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
  {20, 'n'},
  {21, 's'},
  {22, 'w'},
  {23, 'e'}
};
const byte numEndNodes = sizeof(endNodes) / sizeof(endNodes[0]);

//--------------------Implementation of the task functions--------------------

//Task 1: used for communication between the TrafficManager and the EndNodes

//Defines for task 1
#define TX_BUFF_SIZE 8
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
    
    if(result != 0)
    {
      Serial.println("i2c Error: " + String(result) + " with add " + String(tx_buff[i].receiver));
      tx_buff[i].tx_attempts++;
      if(tx_buff[i].tx_attempts < MAX_NUM_OF_TX_ATTEMPTS)
      {
        continue; 
      }
    }

    Serial.println("i2c success with add: " + String(tx_buff[i].receiver));
    //Clearing the message because it was sent successfully or exceeded number of allowed attempts
    tx_buff[i].receiver = 0;
    tx_buff[i].message = "";
    tx_buff[i].tx_attempts = 0;
  }
  
  ///ToDo: Poll for the PIRsensors of all EndNodes
}

//Task 2: used for implementing a state machine for the traffic light logic

//Defines for task 2
/* --empty-- */

//Global vars for task 2
/* --empty-- */

void task2_logic()
{
  //Sending test packets
  
  Serial.println("Task 2: Logic");
  if(!addPacketTX('n', "Hello n from TrafficManager"))
  {
    Serial.println("TX msg queueing error n!");
  }

  if(!addPacketTX('s', "Hello s from TrafficManager"))
  {
    Serial.println("TX msg queueing error s!");
  }

  if(!addPacketTX('w', "Hello w from TrafficManager"))
  {
    Serial.println("TX msg queueing error w!");
  }

  if(!addPacketTX('e', "Hello e from TrafficManager"))
  {
    Serial.println("TX msg queueing error w!");
  }
}

//Task 3: used for printing the system's status

//Defines for task 3
/* --empty-- */

//Global vars for task 3
/* --empty-- */

void task3_status()
{
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  Serial.println("Task 3: Status");
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

//Function to add message into the TX buffer. Returns true if successfull and false if buffer was full
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
  for(short i = 0; i < TX_BUFF_SIZE; i++)
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
