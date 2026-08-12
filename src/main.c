#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(void) {
  char *user = getlogin();
  char host[128];
  gethostname(host, sizeof(host));

  char cwd[4096];
  getcwd(cwd, sizeof(cwd));
  
  // testing
  printf("%s\n%s\n%s\n", host, user, cwd);

  // testing ps1
  char ps1[1024];
  snprintf(ps1, sizeof(ps1), "%s@%s #> ", user, host);
  printf("%s", ps1);
}
