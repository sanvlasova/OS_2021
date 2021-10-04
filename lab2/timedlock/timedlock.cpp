#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

using namespace std;

bool flag1 = 0; // объявляем флаг завершения потока 1
bool flag2 = 0; // объявляем флаг завершения потока 2

pthread_mutex_t mutex; // объявляем идентификатор мьютекса

static void* f1(void* arg){ //функция потока 1
      while (*(bool*)arg == 0){ //пока флаг заверш.потока 1 не установлен
            struct timespec tp; // объявляем переменную
            clock_gettime(CLOCK_REALTIME, &tp);//получаем время в структуру tp
            tp.tv_sec += 1; 

            int rv = pthread_mutex_timedlock(&mutex, &tp); //передаем модифицированное значение tp в функцию
            
            if (rv != 0) { //проверяем занят ресурс или свободен
                cout <<  strerror(rv) << "\n";
                continue; 
            
            }

            for (int i=0; i<6; i++) { //в цикле несколько раз выполнять
                 cout << "1\n" << flush; //выводить символ "1" на экран
                 sleep(1); 
            }

            pthread_mutex_unlock(&mutex); //освободить мьютекс
            sleep(1); 
      }
   
      pthread_exit((void*)5); //завершения потока с кодом возварата
}

static void* f2(void* arg){ 
      while (*(bool*)arg == 0){
            struct timespec tp;
            clock_gettime(CLOCK_REALTIME, &tp);
            tp.tv_sec += 1;

            int rv = pthread_mutex_timedlock(&mutex, &tp); //захватить мьютекс
            
            if (rv != 0) {
                cout <<  strerror(rv) << "\n";
                continue; 
            
            }

            for (int i=0; i<6; i++) {
                 cout << "2\n" << flush; 
                 sleep(1); 
            }

            pthread_mutex_unlock(&mutex); 
            sleep(1); 
      }
      
      pthread_exit((void*)5);

}

int main(){ //основная программа
      cout << "Программа начала работу:\n";

      pthread_t id1; // объявляем идентификатор потока 1
      pthread_t id2; // объявляем идентификатор потока 2

      pthread_mutex_init(&mutex,NULL); //инициализируем мьютекс

      pthread_create(&id1, NULL, f1, &flag1); //создаем поток из функции 1
      pthread_create(&id2, NULL, f2, &flag2); //создаем поток из функции 2
    
      getchar(); //приостановка потока , ждет нажатие клавиши

      flag1 = 1; // устанавливаем флаг завершения потока 1
      flag2 = 1; // устанавливаем флаг завершения потока 1

      int* exit_code1; // объявляем указатели данных потока 1
      int* exit_code2; // объявляем указатели данных потока 2

      pthread_join(id1, (void**)&exit_code1); // ждем завершение потока 1
      pthread_join(id2, (void**)&exit_code2); // ждем завершение потока 2

      pthread_mutex_destroy(&mutex); //удаляем мьютекс

      cout << "Программа закончила работу:"<< exit_code1 << "," << exit_code2;
      
      return 0;
}
