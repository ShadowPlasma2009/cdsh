#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(void) {
  char host[128];
  char *user = getlogin();
  gethostname(host, sizeof(host));

  char cwd[4096];
  getcwd(cwd, sizeof(cwd));
  
  // testing
  printf("%s\n%s\n%s\n", host, user, cwd);
}
