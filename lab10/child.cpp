#include <cstdio>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

int main (int argc, char *argv[], char *env[]) {
  cout << "подпрограмма начала работу" << endl;
  //printf("подпрограмма начала работу\n");
 
  for(int i = 0; i < argc; i++) {
    cout << "Аргумент:" << i << argv[i];
    //printf("Аргумент %d : %s\n", i, argv[i]);
    cout << "Env:" << i << env[i];
    //printf("Env %d : %s\n", i, env[i]);
    sleep(1);
  }

  cout << "PID:" << getpid());
  cout << "Parent PID:" << getppid());
  //printf("PID : %d\n", getpid());
  //printf("Parent PID : %d\n", getppid());

  //printf("подпрограмма завершила работу\n");
  cout << "подпрограмма завершила работу" << endl;

  return 5;
}
