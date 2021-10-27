#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

using namespace std;

bool flag1 = 0; 
sem_t *sem;  // объявляем идентификатор именованного семафора
FILE  *file; // объявляем дескриптор файла



static void* f1(void* arg){ 
      char a = '1';
      while (*(bool*)arg == 0){ 
           sem_wait(sem); //захватить именованный семафор
           for (int i=0; i<5; i++) {     
                 fputs("1", file); //выводить символ в файл
                 cout << "1" << flush; //выводить символ "1" на экран
                 fflush(file);
                 sleep(1); 
            }

            sem_post(sem); //свободить именованный семафор
            sleep(1);
      }
   
      //pthread_exit((void*)5); //завершения потока с кодом возварата
}


int main(){ //основная программа
      //cout << "Программа1 начала работу:" << endl;

      pthread_t id1; 

      file = fopen("result.txt", "a");
      sem = sem_open("/sem", O_CREAT, 0644, 1);

      pthread_create(&id1, NULL, f1, &flag1); 
    
      getchar(); 

      flag1 = 1; 

      //int* exit_code1; // объявляем указатели данных потока 1

      //pthread_join(id1, (void**)&exit_code1); // ждем завершение потока 1
      pthread_join(id1, NULL);

      sem_close(sem); //закрыть именованный семафор
      sem_unlink("/sem"); //удалить именованный семафор

      fclose(file);

      //cout << "Программа закончила работу:"<< exit_code1 << endl;
      
      return 0;
}
