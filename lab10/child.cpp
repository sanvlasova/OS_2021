#include <cstdio>
#include <unistd.h>
#include <stdlib.h>

int main (int argc, char *argv[], char *env[]) {
  printf("child начал работу\n");
 
  for(int i = 0; i < argc; i++) {
    printf("Argument %d = %s\n", i, argv[i]);
    printf("Env %d = %s\n", i, env[i]);
    sleep(1);
  }

  printf("PID = %d\n", getpid());
  printf("Parent PID = %d\n", getppid());

  printf("child завершил работу\n");

  return 123;
}
