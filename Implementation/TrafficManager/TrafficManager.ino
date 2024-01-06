#include <Arduino.h>
#include <Wire.h>

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
  byte receiver;
  String message;
  byte tx_attempts;
};

//Struct for EndNodes
struct EndNode {
  byte i2c_address;
  char location;
};

//Function prototypes
void task1_communication();
void task2_logic();
void task3_status();

bool addPacketTX(EndNode nodes[], byte numNodes, char location, String message);
void executeTask(Task* task);
void runRMS(Task tasks[], byte num_of_tasks);

//Definition of the task set
Task taskSet[] = {
    {1, 100, 0, 0, task1_communication},
    {2, 1000, 0, 0, task2_logic},
    {3, 2000, 0, 0, task3_status}
};
const byte numTasks = sizeof(taskSet) / sizeof(taskSet[0]);

//Definition of EndNodes with i2c address and location
EndNode endNodes[] = {
  {1, 'n'},
  {2, 's'},
  {3, 'w'},
  {4, 'e'}
};
const byte numEndNodes = sizeof(endNodes) / sizeof(endNodes[0]);

//--------------------Implementation of the task functions--------------------

//Task 1: used for communication between the TrafficManager and the EndNodes

#define TX_BUFF_SIZE 16
#define RX_BUFF_SIZE 8
#define MAX_NUM_OF_TX_ATTEMPTS 2
Message tx_buff[TX_BUFF_SIZE];
Message rx_buff[RX_BUFF_SIZE];

void task1_communication()
{
  //Send messages to EndNodes that are stored in an array
  for(short i = 0; i <= (TX_BUFF_SIZE - 1); i++)
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
    if(Wire.endTransmission(true) != 0 && tx_buff[i].tx_attempts <= MAX_NUM_OF_TX_ATTEMPTS)
    {
      tx_buff[i].tx_attempts++;
      continue;
    }

    //Clearing the message because it was sent successfully or exceeded number of allowed attempts
    tx_buff[i].receiver = 0;
    tx_buff[i].message = "";
    tx_buff[i].tx_attempts = 0;
  }
  
  ///ToDo: Poll for the PIRsensors of all EndNodes
  
  Serial.println("Task 1: Comms");
}

//Task 2: used for implementing a state machine for the traffic light logic
void task2_logic()
{
  Serial.println("Task 2: Logic");
  if(!addPacketTX(endNodes, numEndNodes, 'n', "10"))
  {
    Serial.println("TX msg queueing error!");
  }
  
}

//Task 3: used for printing the system's status
void task3_status()
{
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  Serial.println("Task 3: Status");
}

void setup()
{
  //Setting the pin of the built in LED to be an output
  pinMode(LED_BUILTIN, OUTPUT);

  //Setting up serial communication with 115200 baud
  Serial.begin(115200);

  //Setting up i2c communication as a master with timeout of 1ms and no reset when timeout occurs
  Wire.begin();
  Wire.setWireTimeout(1000, false);
}

void loop()
{
  //Calling the scheduler with the task set
  runRMS(taskSet, numTasks);
}


//Function to add message into the TX buffer. Returns true if successfull and false if buffer was full
bool addPacketTX(EndNode nodes[], byte numNodes, char location, String message)
{
  EndNode* destNode;
  for(short i = 0; i <= (numNodes - 1); i++)
  {
    if(nodes[i].location == location)
    {
      destNode = &nodes[i];
      break;
    }
  }

  //If no node was found return false
  if(destNode == nullptr)
  {
    return false;
  }
  
  //Put message
  for(short i = 0; i <= (TX_BUFF_SIZE - 1); i++)
  {
    //Continue if message in array contains content
    if(tx_buff[i].receiver != 0 || tx_buff[i].message != "")
    {
      continue;
    }

    //Clearing the message because it was sent successfully or exceeded number of allowed attempts
    tx_buff[i].receiver = destNode->i2c_address;
    tx_buff[i].message = message;

    return true;
  }

  return false;
}



//--------------------RMS-Functions--------------------

//Function to run the scheduler with a task set
//This needs to be called in the endless loop
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

//Function to execute the function of a task
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
