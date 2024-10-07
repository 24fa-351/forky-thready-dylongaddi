#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
    ./forky <pattern number> <number of processes>

    pattern number: 1,2
    number of processes: 1+
*/

void pattern1_child(int processNumber)
{
  printf("Process %d (%d) beginning\n", processNumber, getpid());

  printf("Process %d (%d) exiting\n", processNumber, getpid());
}

void pattern1_parent(int processCount)
{
  pid_t childPIDs[processCount];

  for (int i = 1; i <= processCount; i++)
  {
    childPIDs[i - 1] = fork();
    if (childPIDs[i - 1] == 0)
    {
      pattern1_child(i);
      exit(0);
    }
    else if (childPIDs[i - 1] < 0)
    {
      printf("Fork failed");
      exit(1);
    }
    else
    {
      printf("Process main (%d) creating Process %d (%d)\n", getpid(), i, childPIDs[i - 1]);
    }
  }

  for (int i = 0; i < processCount; i++)
  {
    waitpid(childPIDs[i], NULL, 0);
  }

  printf("Process main (%d) exiting\n", getpid());
}

void pattern2_child(int processNumber)
{
  printf("Process %d (%d) beginning\n", processNumber, getpid());

  printf("Process %d (%d) exiting\n", processNumber, getpid());
}


void pattern2_parent(int processCount)
{
  for (int i = 1; i <= processCount; i++)
  {
    pid_t childPID = fork();
    if (childPID == 0)
    {

      pattern2_child(i);
      exit(0);
    }
    else if (childPID < 0)
    {
      printf("Fork failed");
      exit(1);
    }
    else
    {
      printf("Process main (%d) creating Process %d (%d)\n", getpid(), i, childPID);
      waitpid(childPID, NULL, 0);
    }
  }

  printf("Process main (%d) exiting\n", getpid());
}

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    printf("Usage: %s [12] <num>\n", argv[0]);
    return 0;
  }

  if (argv[1][0] != '1' && argv[1][0] != '2')
  {
    printf("Invalid pattern number. Must be 1 or 2.\n");
    return 0;
  }

  int forkingPattern = argv[1][0] - '0';
  int processCount;

  if (0 == sscanf(argv[2], "%d", &processCount) || processCount < 1)
  {
    printf("Invalid number of processes (must be 1+).\n");
    return 0;
  }

  printf("Forking pattern: %d, Process count: %d\n", forkingPattern, processCount);

  if (forkingPattern == 1)
  {
    pattern1_parent(processCount);
  }
  else if (forkingPattern == 2)
  {
    pattern2_parent(processCount);
  }

  return 0;
}
