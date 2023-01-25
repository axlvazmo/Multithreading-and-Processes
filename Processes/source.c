/***** Assignemnt 1 *****/
/* Axel Vazquez Montano */

/* importing libraries */
#include <stdio.h>
#include <unistd.h>     //system call lybrary
#include <sys/wait.h>   //for wait(function)
#include <signal.h>     //signal handler library

int main(){

  pid_t pid1, pid2, pid3, pid4; //declaring child processes

  pid1 = fork();  //creating first process
  if(pid1 < 0){
    fprintf(stderr, "Error forking a process");  //if error, then returns -1
    return -1;
  }
  else if(pid1 == 0){   //child process
    execlp("cat", "cat", "/proc/meminfo", NULL);  //outputing the content of the meminfo file
  }
  else{   //parent process
    pid2 = fork();  //creating second child process
    if(pid2 < 0){
      fprintf(stderr, "Error forking a process"); //if error, then returns -1
      return -1;
    }
    else if(pid2 == 0){   //child process
      execlp("uname", "uname", "-a", NULL);   //getting system identification
    }

    else{   //parent process
      pid3 = fork();  //creating third child process
      if(pid3 < 0){
      fprintf(stderr, "Error forking a process"); //if error, then returns -1
      }
      else if(pid3 == 0){   //child process
        execlp("ls", "ls", "-l", NULL);   //listing contents of current directory
      }
      else{
        /* parent process, waiting for child processes to exit */
        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);
        waitpid(pid3, NULL, 0);
      }
    }
  }
  /* creating fourth child process */
  pid4 = fork();
  if(pid4 < 0){
    fprintf(stderr, "Error forking a process"); //if error, then returns -1
    return -1;
  }
  else if(pid4 == 0){   //child process
    execlp("echo", "echo", "4th process", NULL);  //outputing the process number
  }
  printf("Goodbye!");   //printing the goodbye feedback
  return 0;   //invoking the exit() function call
}
