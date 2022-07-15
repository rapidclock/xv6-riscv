#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

#define BUFFER_SIZE 256

// void subcommand(char buf[], char *argv[], int argc) {
//   int i = 0;
//   if (fork() == 0) {
//     printf("Buffer Contents : %s\n", buf);
//
//     for (i = 0; i < argc; i++) {
//       printf("[Debug] Inside sub-fn, args [%d] : %s\n", i, argv[i]);
//     }
//
//     exec(argv[0], argv);
//   } else {
//     wait((int*) 0);
//   }
// }

void subcommand(char *argv[], int argc) {
  int i = 0;
  if (fork() == 0) {
    for (i = 0; i < argc; i++) {
      printf("[Debug] Inside sub-fn, args [%d] : %s\n", i, argv[i]);
    }
    exec(argv[0], argv);
  } else {
    wait((int *)0);
  }
}

int main(int argc, char *argv[]) {
  char *args[MAXARG] = {0};
  char *cur, *word;
  char inp_buf[BUFFER_SIZE];
  int i, sub_argc;
  int d_cnt = 0;

  cur = word = inp_buf;
  sub_argc = argc - 1;

  // Prep argv for child
  for (i = 1; i < argc; i++) {
    args[i - 1] = argv[i];
  }

  for (i = 0; i < argc - 1; i++) {
    printf("[Debug] args [%d] : %s\n", i, args[i]);
  }

  // read char by char and process the args at the same time.
  // terminate every space with a /0 and break to the subshell
  // when we encounter a /n (newline).
  while (read(0, cur, 1) > 0) {
    d_cnt++;
    //    printf("[Debug] Count : %d\n", d_cnt);
    //    printf("[Debug] sub_argc : %d\n", sub_argc);
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
        subcommand(args, sub_argc);
        sub_argc = argc - 1;  // reset to initial value for next call.
        break;
      case '\0':
        subcommand(args, sub_argc);
        break;
      default:
        cur++;
        break;
    }
  }
  exit(0);
}
