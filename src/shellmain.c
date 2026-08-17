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
    memset(inputbuff, '\0', sizeof(inputbuff));

    read_input(inputbuff, sizeof(inputbuff));
    disable_raw();
    
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
  size_t crsr_pos = 0;
  size_t i = 0;
  char c;

  enable_raw();

  while (i < max_len - 1) {
    if (read(STDIN_FILENO, &c, 1) <= 0) break;

    if (c == '\x1b') {
      char seq[2];
      if (read(STDIN_FILENO, &seq[0], 1) == 1 && read(STDIN_FILENO, &seq[1], 1) == 1) {
        switch (seq[1]) {
          case 'A':
            break;
          case 'B':
            break;
          case 'C':
            // right arrow key handling
            if (crsr_pos < i) {
              crsr_pos++;
              write(STDOUT_FILENO, "\x1b[C", 3);
            }
          break;
          case 'D':
            // left arrow key handling
            if (crsr_pos > 0) {
              crsr_pos--;
              write(STDOUT_FILENO, "\x1b[D", 3);
            }
        }
      } continue;
    }

    if (c == '\n' || c == '\r') {
      write(STDOUT_FILENO, "\n", 1);
      break;
    } else if (c == 127 || c == '\b') {
      if (crsr_pos > 0) {
        memmove(&buff[crsr_pos - 1], &buff[crsr_pos], i - crsr_pos + 1);
        crsr_pos--;
        i--;

        write(STDOUT_FILENO, "\b", 1);
        write(STDOUT_FILENO, &buff[crsr_pos], i - crsr_pos);
        write(STDOUT_FILENO, " ", 1);

        for (size_t k = 0; k <= (i - crsr_pos); k++) {
          write(STDOUT_FILENO, "\x1b[D", 3);
        }
      }
    } else if (c == 4) {
      if (i == 0) exit(0);
    } else {
      memmove(&buff[crsr_pos + 1], &buff[crsr_pos], i - crsr_pos);
      buff[crsr_pos] = c;
      crsr_pos++;
      i++;
      write(STDOUT_FILENO, &buff[crsr_pos - 1], i - crsr_pos + 1);
      for (size_t k = 0; k < i - crsr_pos; k++) {
        write(STDOUT_FILENO, "\x1b[D", 3);
      }
    }

    buff[i] = '\0';
  }
}
