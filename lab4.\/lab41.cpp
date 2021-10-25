#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char *argv[]){

    cout << "lab41 начала работу.\n";
    cout << "lab41 ID Текущего процесса:" << getpid() << endl;
    cout << "lab41 ID Родительского процесса:" << getppid() << endl;
    cout << argc << "\n";


    for (int i = 0; i < argc; ++i){
	cout << argv[i] << endl;
	
	if (i == argc - 1)
	    cout << endl;
	else
	    sleep(1);
    }

    cout << "lab41 закончила работу.\n";

    exit(5);

}


