#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char *argv[]){

    cout << "child starts.\n";
    cout << "child ID:" << getpid() << endl;

    int count;
    for(count = 0; count < argc; count++ ) {
       cout << " argv[" << count << "] " << argv[count] << "\n";
    }
    cout << "child end." << endl;
    return 5;
}

