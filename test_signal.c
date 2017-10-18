#include "types.h"
#include "user.h"


int value = 1;

void sig_handled(void){

    printf(1,"Signal received \n");
    value = 0;
    printf(1,"value = %d",value);

}

void signal2 (void){
    printf(1,"signalFTW\n");
    value = 2;
}

int main(int argc, char *argv[]){

    int pid;
    int signum;
    int priority;

    if(argc <= 2){
           printf(2,"Error use test_signal signum priority \n");
       exit();
     }

     signum = atoi(argv[1]);
     priority = atoi(argv[2]);
     set_priority(priority);
    //Creating another process

    pid = fork();
    switch(pid){
        case -1: //There was a problem
            printf(2,"Error running fork");
            break;

        case 0: //child
            printf(1,"I am the child: %d\n", getpid());
            printf(1,"Waiting for a signal \n");
            //Setup the signal and wait for it
            signal(signum,signal2);
            while(value){};
            printf(signum,"\nWhile completed\n");
            break;

        default://Parent
            //Wait for the child to start
            sleep(10);
            //Send the signal
            printf(1,"Sending signal %d to %d\n",signum,pid);
            killsignal(pid,signum);
            //Wait for the child to complete
            wait();
            printf(2,"Child completed\n");
    }
    exit();
}
