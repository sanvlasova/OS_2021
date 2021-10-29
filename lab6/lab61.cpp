#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/mman.h>

using namespace std;

bool flag1 = 0; 
sem_t *semWrite; //объявляем идентификатор семафора записи
sem_t *semRead;  //объявляем идентификатор семафора чтения
int fd, *adr;  //объявляем индентификатор разделяемой памяти и локального адреса

static void* f1(void* arg){ 
      while (*(bool*)arg == 0){ 
           int count;
           count = rand(); 
           count = *adr; 
           cout << "Записать в разделяемую память:" << endl;
           cout<< count << flush;
           cout << endl;
           sem_post(semWrite); //освободить семафор записи
           sem_wait(semRead); //ждать семафора чтения
           sleep(1); 
      }  
}


int main(){ //основная программа
      //cout << "Программа1 начала работу:" << endl;

      pthread_t id1; 
      adr = 0;

      shm_unlink("/mem");
      fd = shm_open("/mem", O_CREAT|O_RDWR, 0644); //создать разделяемую память
      ftruncate(fd, sizeof(int)); //установка рамера
      adr = (int*)mmap(0,sizeof(int), PROT_WRITE|PROT_READ, MAP_SHARED, fd, 0); //отобразить разделяемую память на локальный адрес
      
      semWrite = sem_open("/semwrite", O_CREAT, 0644, 0);
      semRead = sem_open("/semread", O_CREAT, 0644, 0);

      pthread_create(&id1, NULL, f1, &flag1); 
    
      getchar(); 

      flag1 = 1; 

      pthread_join(id1, NULL);

      sem_close(semWrite); 
      sem_close(semRead);
      sem_unlink("/semwite"); 
      sem_unlink("/semread"); 

      munmap(adr, sizeof(int));
      close(fd);
      shm_unlink("/mem");
      
      return 0;
}
