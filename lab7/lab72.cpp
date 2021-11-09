#include <fcntl.h>      
#include <iostream>        
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <sys/mman.h>     
#include <semaphore.h>
#include <signal.h>
#include <mqueue.h>
#include <cerrno>
#include <cstdlib>

using namespace std;
 
struct Arg {
    bool flag1;
    mqd_t mq; //объяляем идентификатор очереди сообщений
};

//int result;

static void* f1(void* args){
    Arg* x=(Arg*)args;
    char buf[10];
    cout << "Извлечь сообщение из очереди:" << endl; 
    while(!x->flag1){
	if(mq_receive(x->mq, buf, 20*sizeof(char), 0)!=-1)
		cout << buf << endl;
	else perror("mq_receive");
	sleep(1);
    }

}


int main(){
    cout << "Программа начала работу" << endl;
    Arg arg;
    arg.flag1 = 0;

    struct mq_attr attr; //атрибуты
    mqd_t mqd;  
    
    attr.mq_flags = 0; //flag - 0 or NON_BLOCK
    attr.mq_maxmsg = 20; //max messages on queue 
    attr.mq_msgsize = 15*sizeof(char); //max size of message (byte)
    attr.mq_curmsgs = 0; // current message

    arg.mq = mq_open("/myqueue", O_CREAT|O_RDONLY|O_NONBLOCK, 0644, &attr);
    if(arg.mq==-1) 
        perror("mq_open");  

    mq_getattr(arg.mq, &attr);
    printf("Maximum # of messages on queue: %ld\n", attr.mq_maxmsg);
    printf("Maximum message size: %ld\n", attr.mq_msgsize);


    pthread_t id1;
    pthread_create(&id1, NULL, f1, &arg);

    getchar();

    arg.flag1 = 1;

    pthread_join(id1, NULL);

    //cout << "Flag: " << attr.mq_flags << endl;
    //cout << "Max-message: " << attr.mq_maxmsg << endl;
    //cout << "Size message: " << attr.mq_msgsize << endl;
    //cout << "Current message: " << attr.mq_curmsgs << endl;

    mq_close(arg.mq); //зыкрыть очередь сообщений
    mq_unlink("/myqueue"); //удалить очередь сообщений
    
    cout << "Программа закончила работу" << endl;
    return 0;
}
