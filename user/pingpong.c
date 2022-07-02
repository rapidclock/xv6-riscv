#include "kernel/types.h"
#include "user/user.h"

int main() {
  int p[2], pid, cur_pid;
  char data = 'b';
  char buf;
  pipe(p);

  pid = fork();
  cur_pid = getpid();

  if (pid == 0) {
    // Child
    read(p[0], &buf, 1);
    if (buf == data) {
      fprintf(1, "%d : received ping\n", cur_pid);
      write(p[1], &data, 1);
    } else {
      fprintf(2, "Ping failed\n");
      exit(1);
    }
  } else {
    // Parent
    write(p[1], &data, 1);
    read(p[0], &buf, 1);
    if (buf == data) {
      fprintf(1, "%d : received pong\n", cur_pid);
      write(p[1], &data, 1);
    } else {
      fprintf(2, "Pong failed\n");
      exit(1);
    }
  }
  exit(0);
}