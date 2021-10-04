#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

static void* f1(void *arg){ //поточная функция 1
      while (*(bool*)arg == 0){ //пока (флаг заверш.потока 1 не устан.)делать
            cout << "1" << flush; //выводить символ "1" на экран
            sleep(1); //задержать на 1сек
      }
      void* exit_code = (void*) 5; //передача параметра из потока
      pthread_exit(exit_code); //завершение потока с выводом параметра
}

static void * f2(void *arg){
      while (*(bool*)arg == 0){
            cout << "2" << flush;
            sleep(1);
      }
      void* exit_code = (void*) 5;
      pthread_exit(exit_code);
}

int main(){ //основная программа
      cout << "Программа начала работу:";
   
      bool flag1 = 0; // объявляем флаг завершения потока 1
      bool flag2 = 0; // объявляем флаг завершения потока 2
      int exit_code1; // объявляем переменную данных потока 1
      int exit_code2; // объявляем переменную данных потока 2
      pthread_t id1; // объявляем идентификатор потока 1
      pthread_t id2; // объявляем идентификатор потока 2

      pthread_create(&id1, NULL, f1, &flag1); //создаем поток из функции 1
      pthread_create(&id2, NULL, f2, &flag2); //создаем поток из функции 2

      
      getchar(); //приостановка потока , ждет нажатие клавиши

      flag1 = 1; // устанавливаем флаг завершения потока 1
      flag2 = 1; // объявляем флаг завершения потока 1


      pthread_join(id1, (void**)&exit_code1); // ждем завершение потока 1
      pthread_join(id2, (void**)&exit_code2); // ждем завершение потока 2
      cout << "Программа закончила работу:"<< exit_code1 << exit_code2;
      
      return 0;
}
