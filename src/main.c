#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(void) {

  #DEFINE motdpath ""
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
  
  fflush(stdout);

  char inputbuff[4096];
  fgets(inputbuff, sizeof(inputbuff), stdin);
  inputbuff[strcspn(inputbuff, "\n")] = '\0';

  printf("Testprint input: %s\n", inputbuff);
}
