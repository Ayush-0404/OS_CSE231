#include<stdio.h>
#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(){
    printf("This is the runfile! \n");
    int r= fork();
    
    if (r<0){
        fprintf(stderr,"fork failed\n");
        exit(1);
    }

    if (r== 0) { // child (new process)
      printf("hello, I am child (pid:%d)\n", (int) getpid());
      char *args[]={"./EXEC",NULL};
      execvp(args[0],args);
      printf("this shouldn’t print out\n");
    }

    else {
      int w = wait(NULL);
      printf("hello,I am parent of %d (w:%d) (pid:%d)\n",r ,w,(int) getpid());
    }
    return 0;
}



// t=TRUE;
// while t==true{



//  }
