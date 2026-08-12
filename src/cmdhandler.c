#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "include.h"

void cmdhandler(char command[]) {
  // For development purposes:
  printf("%s\n", command);

  system(command);
}
