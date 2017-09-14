#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

void sigint_handler(int sig)
{
    printf("Ahhh! SIGINT!\n");
    //write(0, "Ahhh! SIGINT!\n", 14);
}

int main(void)
{
    //void sigint_handler(int sig); /* prototype */
    char s[200];
    //struct solo tiene atributos, no métodos. El numero de funcion que se ejecutará, el nombre...
    //mi objeto se llama sa.
    struct sigaction sa;

    //recibimos el nombre de la función de arriba, que voy a ejecutar. Es void y recibe un enter.
    sa.sa_handler = sigint_handler;
    sa.sa_flags = 0; // or SA_RESTART
    //mientras esté en una interrupción, no me interrumpas. Si mi mask está en cero, nadie me interrumpa.
    sigemptyset(&sa.sa_mask);

    //SIGINT ES la interrupción que queremos.
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("Enter a string:\n");

    if (fgets(s, sizeof(s), stdin) == NULL)
        perror("fgets");
    else
        printf("You entered: %s\n", s);

    return 0;
}
