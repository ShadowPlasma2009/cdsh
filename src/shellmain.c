#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <termios.h>
#include "include.h"
#define MOTDPATH "placeholder"

struct termios old_term;
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

    read_input(inputbuff, sizeof(inputbuff));
    
    char tempbuff[sizeof(inputbuff)];
    strcpy(tempbuff, inputbuff);
    char *cmd = strtok(tempbuff, " ");

    if (strlen(inputbuff) == 0) continue;

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


void disable_raw(void) {
  tcgetattr(STDIN_FILENO, &old_term);
}

void enable_raw(void) {
  // Save original terminal state
  // Turn off raw mode when program exits
  // Turn of canonical mode
  // apply mofifications
  tcgetattr(STDIN_FILENO, &old_term);
  atexit(disable_raw);

  struct termios raw_term = old_term;

  raw_term.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_term);
}

void read_input(char *buff, size_t max_len) {
  size_t i = 0;
  char c;

  enable_raw();

  while (i < max_len) {
    if (read(STDIN_FILENO, &c, 1) <= 0) break;

    if (c == '\n' || c == '\r') {
      write(STDOUT_FILENO, "\n", 1);
      break;
    } else if (c == 127 || c == '\b') {
      if (i > 0) {
        i--;
        write(STDOUT_FILENO, "\b \b", 3);
      }
    } else if (c == 4) {
      if (i == 0) exit(0);
    } else {
      buff[i++] = c;
      write(STDOUT_FILENO, &c, 1);
    }

    buff[i] = '\0';
    disable_raw();
  }
}
