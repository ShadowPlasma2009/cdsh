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

      if (strcmp(inputbuff, "exit") == 0) {
        running = false;
        return 0;
      } else if (strcmp(inputbuff, "help") == 0) {
        help();
      } else if (strcmp(inputbuff, "cd") == 0) {
        cd(inputbuff);
      } else { cmdhandler(inputbuff); }
    }
  }
  
}

int help(void) {
  printf("cut-down shell help\n");
  
  printf("\nInbuilt commands:\n");
  printf("  help\n");
  printf("  exit\n");
  printf("  cd <directory>\n");
  return 0;
}

int cd(char path[]) {
  //TODO
}
