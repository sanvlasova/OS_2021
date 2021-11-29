#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <netdb.h>
#include <cstring>
#include <mqueue.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <vector>
#include <signal.h>

using namespace std;

struct sockaddr_in listenSockAddr; 
int serverSocket; //объявить идентификатор сокета для работы с клиентом;
int listenSocket; //объявить идентификатор «слушающего» сокета;
struct sockaddr_in serverSockAddr; 
vector <string> msglist; //объявить идентификатор очереди запросов на обработку;

pthread_t id_accept_require, id_send_answer, id_expect_connection;

typedef struct {
    int flag_accept_require = 0; //объявить флаг завершения потока приема запросов;
    int flag_send_answer = 0; //объявить идентификатор потока обработки запросов и передачи ответов;
    int flag_expect_connection = 0; //объявить флаг завершения потока ожидания соединений;
    pthread_mutex_t mutex_p;
} args_s;

args_s arg1;

void sig_handler(int signo)
{
    printf("SIGPIPE received\n");
}


void * accept_require(void *arg) // функция приема запросов()
{
    int k = 0;
    args_s *args = (args_s*) arg;
    char rcvbuf[256];

    printf("Поток accept_require начал работу\n"); 

    while(args->flag_accept_require != 1) //пока (флаг завершения потока приема не установлен) 
    {
        int reccount = recv(serverSocket, rcvbuf, 256, 0); //принять запрос из сокета;
        if (reccount == -1) {
            perror("recv error");
            sleep(1);
        }else if (reccount == 0) {
            //разъединение;
            sleep(1);
        }else{
        //здесь запрос надо положить в очередь и учесть, что эта очередь – общий ресурс с потоком передачи ответов, т.е. нужен мьютекс
        k = pthread_mutex_trylock(&(args->mutex_p));
        while(k != 0) {
            perror("pthread_mutex_trylock");
            k = pthread_mutex_trylock(&(args->mutex_p));
            sleep(1);
        } // мьютекс захватить;
        msglist.push_back(string(rcvbuf)); //    поместить запрос в очередь;
        cout << "Accept is ok, number of request is: " << rcvbuf << endl; //принято, кол-во запросов:
        pthread_mutex_unlock(&(args->mutex_p)); // мьютекс освободить;
        }
    }
    pthread_exit((void*)5);
}

void * send_answer(void *arg) // функция обработк запросов и передачи ответов()
{
    int k;
    args_s *args = (args_s*) arg;

    int res, s;
    struct addrinfo *result;
    char sndbuf[256];

    /*s = getaddrinfo("localhost", NULL, NULL, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    res = result->ai_protocol;

    freeaddrinfo(result);*/

    struct sockaddr_in sin;
    socklen_t len = sizeof(sin);
    if (getsockname(serverSocket, (struct sockaddr *)&sin, &len) == -1)
        perror("getsockname");
    else
        res = ntohs(sin.sin_port);

    printf("Поток send_answer начал работу\n");

    while(args->flag_send_answer != 1){ // пока (флаг завершения потока передачи не установлен)
        k = pthread_mutex_trylock(&(args->mutex_p));
        while(k != 0) {
            perror("pthread_mutex_trylock");
            k = pthread_mutex_trylock(&(args->mutex_p));
            sleep(1);
        } // мьютекс захватить;
        if (!msglist.empty()) {//очередь запросов не пуста
            string S = msglist.back(); //получаете первый в очереди запрос // прочитать запрос из очереди на передачу;
            msglist.pop_back();//удаляете его из очереди
            pthread_mutex_unlock(&(args->mutex_p)); // мьютекс освободить;

            len = sprintf(sndbuf, "%d", res);
        //выполняем функцию, которую требует задание;
        //Например, uname. 
        //Функция возвращает структуру из нескольких полей. 
        //Берем любое поле, превращаете его в массив символов.
        //Назовем массив sndbuf.
        //Добавляете к нему запрос (проверка очередности запрос-ответ).
        //Передаете его вызовом:

            int sentcount = send(serverSocket, sndbuf, len, 0); // передать ответ в сокет;
            if (sentcount == -1) {
                perror("send error");
            }else{
                sleep(1);
                cout << "Send is ok, client port: " << S << ", server port: " << sndbuf << endl;
                    //send OK
            }

        }else{//очередь пуста
            printf("QUEUE IS EMPTY\n");
            pthread_mutex_unlock(&(args->mutex_p)); // мьютекс освободить;
            sleep(1);
        }
        res++;
    }
    pthread_exit((void*)4);
}


void * expect_connection(void *arg) //функция ожидания соединений()
{   
    args_s *args = (args_s*) arg;

    printf("Поток expect_connection начал работу\n");

    while(args->flag_expect_connection != 1) //пока (флаг завершения потока ожидания соединений не установлен) 
    {
        socklen_t addrLen = (socklen_t)sizeof(serverSockAddr);
        serverSocket = accept(listenSocket, (struct sockaddr*)&serverSockAddr, &addrLen); //прием соединения от клиента
        if (serverSocket == -1) {
            perror("accept error");
            sleep(1);
        }else{ //если соединение принято
        
            //соединение установлено
            //создаем два потока:
            //для приема запросов от клиента
            //для передачи ответов клиенту
            //завершаем текущий поток
            //для упрощения обработку запросов включим в поток передачи ответов
        
            int err = 0;
            err = pthread_create(&id_accept_require, NULL, accept_require, &arg1);
            if(err != 0)
            {
                perror("pthread_create");   
            } //создать поток приема запросов;

            err = pthread_create(&id_send_answer, NULL, send_answer, &arg1);
            if(err != 0)
            {
                perror("pthread_create");
            } //создать поток обработки запросов и передачи ответов;
            
            pthread_exit((void*)3); //завершить работу потока ожидания соединений;
        }
    }
    pthread_exit((void*)1);
}

int main() //основная программа()
{

    signal(SIGPIPE, sig_handler);
    int exit = 0, exit1 = 0, exit2 = 0, err = 0;

    printf("Программа начала работу\n"); 

    listenSocket = socket(AF_INET, SOCK_STREAM, 0);  //создать «слушающий» сокет;
    fcntl(listenSocket, F_SETFL, O_NONBLOCK);
    listenSockAddr.sin_family = AF_INET;
    listenSockAddr.sin_port = htons(7000); 
    listenSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(listenSocket, (struct sockaddr*)&listenSockAddr, sizeof(listenSockAddr)); //привязать «слушающий» сокет к адресу;
    int optval = 1;
    setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    listen(listenSocket, SOMAXCONN); //перевести сокет в состояние прослушивания;
    //создать очередь запросов на обработку;

    err = pthread_create(&id_expect_connection, NULL, expect_connection, &arg1);
    if(err != 0)
    {
        perror("pthread_create");
        return 1;    
    } //создать поток ожидания соединений;
    printf("Программа ждет нажатия клавиши\n");

    getchar(); 

    printf("Клавиша нажата\n"); 
    
    arg1.flag_accept_require = 1;
    arg1.flag_send_answer = 1;
    arg1.flag_expect_connection = 1;
    //установить флаг завершения потока приема запросов;
    //установить флаг завершения потока обработки запросов и передачи ответов;
    //установить флаг завершения потока ожидания соединений;

    pthread_join(id_expect_connection, (void**)&exit);
    printf("Поток expect_connection закончил работу\n");

    if(exit == 3){
        pthread_join(id_accept_require, (void**)&exit1);
        printf("Поток accept_require закончил работу\n");
        pthread_join(id_send_answer, (void**)&exit2);
        printf("Поток send_answer закончил работу\n");
    }
    //ждать завершения потока приема запросов;
    //ждать завершения потока обработки запросов и передачи ответов;
    //ждать завершения потока ожидания соединений; 

    shutdown(serverSocket, 2); //закрыть соединение с клиентом;
    close(serverSocket); //закрыть сокет для работы с клиентом;
    close(listenSocket); //закрыть «слушающий» сокет;

    printf("Программа server закончила работу\n");

    return 0;
}
