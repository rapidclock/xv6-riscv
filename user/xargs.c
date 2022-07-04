#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

#define BUFFER_SIZE 256

void subcommand(char *argv[]) {
  if (fork() == 0) {
    exec(argv[0], argv);
  } else {
    wait((int*) 0);
  }
}

int main(int argc, char *argv[]) {
  char *args[MAXARG];
  char *cur, *word;
  char inp_buf[BUFFER_SIZE];
  int i, sub_argc;

  cur = word = inp_buf;
  sub_argc = argc - 1;

  // Prep argv for child
  for (i = 1; i < argc; i++) {
    args[i - 1] = argv[i];
  }

  // read char by char and process the args at the same time.
  // terminate every space with a /0 and break to the subshell
  // when we encounter a /n (newline).
  while (read(0, &cur, 1) > 0) {
    switch (*cur) {
      case ' ':
        *cur = '\0';
        args[sub_argc] = word;
        sub_argc++;
        cur++;
        word = cur;
        break;
      case '\n':
        *cur = '\0';
        subcommand(args);
        sub_argc = argc - 1; // reset to initial value for next call.
        break;
      case '\0':
        subcommand(args);
        break;
      default:
        break;
    }
  }
  exit(0);
}
