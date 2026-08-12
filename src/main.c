#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "include.h"
#define MOTDPATH "placeholder"

bool running = true;
int main(void) {
  
  char *user = getlogin();
  
  char host[128];
  gethostname(host, sizeof(host));

  char cwd[4096];
  getcwd(cwd, sizeof(cwd));
  
  // testing
  printf("%s\n%s\n%s\n", host, user, cwd);

  printf("\nWelcome to cut-down shell.\n");

  while (running) {
    // testing ps1
    char ps1[1024];
    snprintf(ps1, sizeof(ps1), "%s@%s #> ", user, host);
    printf("%s", ps1);
    
    fflush(stdout);

    char inputbuff[4096];
    
    if (fgets(inputbuff, sizeof(inputbuff), stdin) != NULL) {
      inputbuff[strcspn(inputbuff, "\n")] = '\0';

      char tempbuff[sizeof(inputbuff)];
      strcpy(tempbuff, inputbuff);
      char *cmd = strtok(tempbuff, " ");

      if (strcmp(inputbuff, "exit") == 0) {
        running = false;
        return 0;
      } else if (strcmp(inputbuff, "help") == 0) {
        help();
      } else if (strcmp(cmd, "cd") == 0) {
        cd(inputbuff);
      } else if (strcmp(inputbuff, "pwd") == 0) {
        char cwd[4096];
        getcwd(cwd, sizeof(cwd));
        printf("%s\n", cwd);
      } else { cmdhandler(inputbuff); }
    }
  }
}

int help(void) {
  printf("cut-down shell help\n");
  
  printf("\nInbuilt commands:\n");
  printf("  cd <directory>\n");
  printf("  pwd\n");
  printf("  help\n");
  printf("  exit\n");
  return 0;
}

int cd(char path[]) {
  strtok(path, " ");
  char *pathtok = strtok(NULL, " ");

  if (chdir(pathtok) != 0) {
    perror("chdir");
    return 1;
  }

  return 0;
}
