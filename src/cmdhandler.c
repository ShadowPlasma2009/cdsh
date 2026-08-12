#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "include.h"

void cmdhandler(char command[]) {
  // For development purposes:
  //printf("%s\n", command)
  //char *env[] = { NULL};
  int i = 0;

  char *argv[64] = {};
  char *token = strtok(command, " ");
  
  while (token != NULL) {
    argv[i++] = token;
    token = strtok(NULL, " ");
  }

  pid_t pid = fork();

  if (pid < 0) {
    perror("Fork failed");
    exit(1);
  } else if (pid == 0) {
    //error checking
    if (execvp(argv[0], argv) == -1) {
        perror("Command failed");
    }
    _exit(1);
  } else {
    int status;
    waitpid(pid, &status, 0);
  }
  
}
