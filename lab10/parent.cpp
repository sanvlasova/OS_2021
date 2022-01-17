#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ev.h>

static void child_callback (EV_P_ ev_child *w, int revents) {
  ev_child_stop (EV_A_ w);
  ev_break(EV_A_ EVBREAK_ALL);

  printf("подпроцесс вернул код: %d\n", WEXITSTATUS(w->rstatus));
}

static void stdin_callback (EV_P_ ev_io *w, int revents) {
  ev_io_stop(EV_A_ w);
  ev_break(EV_A_ EVBREAK_ALL);

  printf("\nклавиша нажата\n");
}

int main (int argc, char *argv[]) {
  printf("мастер программа начала работу\n");

  const char *arguments[5] = { "Param1", "Param2", "Param3", "Param4", NULL };
  const char *env[5] = { "env1", "env2", "env3", "env4", NULL };

  ev_child child_watcher;
  ev_io	stdin_watcher;
  pid_t	pid	=	fork();
  int slaveStatus;

  if (pid == 0) {
    if(execve("./slave", const_cast<char* const *>(arguments), const_cast<char* const *>(env)) == -1){
      perror("execve");
    }
  } else if (pid > 0) {
    printf("PID = %d\n", getpid());
    printf("Parent PID = %d\n", getppid());
    printf("Slave PID = %d\n", pid);

    ev_child_init (&child_watcher, child_callback, pid, 0);
    ev_child_start (EV_DEFAULT_ &child_watcher);
    ev_run (EV_DEFAULT_ 0);
  } else {
    perror("fork");
  }

  printf("мастер программа ждет нажатия клавиши\n");

  ev_io_init(&stdin_watcher, stdin_callback, STDIN_FILENO, EV_READ);
  ev_io_start(EV_DEFAULT_ &stdin_watcher);
  ev_run (EV_DEFAULT_ 0);
  

  printf("мастер программа завершила работу\n");
}
