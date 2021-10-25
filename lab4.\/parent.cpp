#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

using namespace std;

int main(int argc, char *argv[]){
    int pid, status;
    cout << "parent starts.\n";
    cout << "parent ID:" << getppid() << endl;
    
    char buf[argc + 1];
    int count;
    for(count = 0; count < argc; count++ ){
      cout << " argv[" << count << "] " << argv[count] << "\n";
    }

    pid = fork();
    switch(pid){
    case -1:
    perror("fork");
    return 1;
    
    case 0:
    execl("./child", argv[0], argv[1], argv[2], argv[3], NULL);

    default:
    while (waitpid(pid, &status, WNOHANG) == 0){
	    cout << "Ждем...\n";
	    usleep(500000); //задержка в половину секунды
	}
	cout << "parent: child exited with status:" << WEXITSTATUS(status) << endl;
    }
    cout << "parent end.\n";
	
    return 0;
}






