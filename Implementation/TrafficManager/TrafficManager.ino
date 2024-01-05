#include <Arduino.h>

//Struct for a task
struct Task {
    byte id;
    unsigned int period;  //Time period in ms
    unsigned int executionTime;  //Last execution duration in us
    unsigned long lastExecutionTime;  //Last execution time in ms
    void (*taskFunction)();  //Function that should be executed in the task
};

//Function prototypes
void task1_communication();
void task2_logic();
void task3_status();
void executeTask(Task* task);
void runRMS(Task tasks[], byte num_of_tasks);

//Definition of the task set
Task taskSet[] = {
    {1, 100, 0, 0, task1_communication},
    {2, 1000, 0, 0, task2_logic},
    {3, 2000, 0, 0, task3_status}
};
const byte numTasks = sizeof(taskSet) / sizeof(taskSet[0]);

//Implementation of the tasks
void task1_communication() {
    Serial.println("Task 1: Comms");
}

void task2_logic() {
    Serial.println("Task 2: Logic");
}

void task3_status() {
  digitalWrite(13, !digitalRead(13));
  Serial.println("Task 3: Status");
}

void setup() {
    pinMode(13, OUTPUT);
    Serial.begin(115200);
}

void loop() {
    runRMS(taskSet, numTasks);
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
