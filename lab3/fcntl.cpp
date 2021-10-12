#include <iostream>
#include <pthread.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

using namespace std;

bool flag1 = 0; // объявляем флаг завершения потока 1
bool flag2 = 0; // объявляем флаг завершения потока 2

int filedes[2]; // объявляем идентификатор неименованного канала

static void* f1(void* arg){ //функция потока 1
      //cout <<"f1 начал работу\n";
      int buf = 0; // объявляем буфер
      while (*(bool*)arg == 0){ //пока флаг заверш.потока 1 не установлен
            buf++; //генерация нового сообщения
            write(filedes[1], &buf, sizeof(int)); //записать сообщение из буфера в неименованный канал
                 cout << "В канал записано число:" << buf << "\n";
                 sleep(1);
      }
    
      pthread_exit((void*)5);
}

static void* f2(void* arg){ 
      //cout <<"f2 начал работу\n";
      int buf, rv; //буфер для прочитанных сообщений из канала, rv - переменная для обработки ошибок
      while (*(bool*)arg == 0){
            rv = read(filedes[0], &buf, sizeof(int)); //прочитать сообщение из неименованного канала в буфер
            if (rv > 0){
	          cout << "Из канала получено число:" << buf << "\n";
                  sleep(1);
	          }  
            else if (rv == 0) {
	          cout << "Канал пуст. В канале нет данных для чтения";
                  sleep(1);
	          }

            else {
	          perror("Произошла ошибка");
	          sleep(1);
                  }
      }

      pthread_exit((void*)5);
}


int main(){ //основная программа
      cout << "Программа начала работу:\n";

      pthread_t id1; // объявляем идентификатор потока 1
      pthread_t id2; // объявляем идентификатор потока 2

      pipe(filedes); // Создание неименованного канала
      fcntl(filedes[0], F_SETFL, O_NONBLOCK);
      fcntl(filedes[1], F_SETFL, O_NONBLOCK); 

      pthread_create(&id1, NULL, f1, &flag1); //создаем поток из функции 1
      pthread_create(&id2, NULL, f2, &flag2); //создаем поток из функции 2

    
      getchar(); //приостановка потока , ждет нажатие клавиши


      flag1 = 1; // устанавливаем флаг завершения потока 1
      flag2 = 1; // устанавливаем флаг завершения потока 1

      int* exit_code1;
      int* exit_code2;

      pthread_join(id1, (void**)&exit_code1);
      close(filedes[1]);

      pthread_join(id2, (void**)&exit_code2);  
      close(filedes[0]);

      cout << "Программа закончила работу:" << exit_code1 << "," << exit_code2;
      
      return 0;
}
