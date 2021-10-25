#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main(){
    cout << "lab42 начала работу.\n";
    int pid, status;
    if ((pid = fork()) < 0)
	perror("fork");
    else if (!pid){
	//cout << "lab42 ID Родительского процесса:" << getppid() << endl;
	execl("./lab41", "./lab41", "10", "15", "20", NULL);
        perror("execl");
    }
     else {
	cout << "lab42 ID Текущего процесса:" << getpid() << endl;
	cout << "lab42 ID Дочернего процесса:" << pid << endl;
	cout << "lab42 ID Родительского процесса:" << getppid() << endl;

	while (waitpid(pid, &status, WNOHANG) == 0){
	    cout << "Ждем...\n";
	    usleep(500000); //задержка в половину секунды
	}
	cout << "Код завершения дочернего процесса:" << WEXITSTATUS(status) << endl;
    }
    cout << "lab42 закончила работу.\n";
	
    return 0;
}
