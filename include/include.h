#ifndef INCLUDE_H
#define INCLUDE_H

void cmdhandler(char *command);
int help(void);
int cd(char *path);
void enable_raw(void);
void disable_raw(void);
void read_input(char *buff, size_t maxlen);

#endif
