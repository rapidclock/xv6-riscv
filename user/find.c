#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(2, "Usage : find START_DIR FILE_TO_FIND\n");
  }
  exit(0);
}