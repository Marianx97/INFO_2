#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/*
    Al utilizar pipe() en conjunto con fork(), se puede establecer una
    via de comunicación entre 2 procesos.
    Debido a que el fork() duplica en el proceso hijo (PH) los recursos del padre (PP),
    el PH posee la variable fd y por ende los file descriptors para comunicarse con el PP.
*/

int main() {
    int fd[2];
    char buffer[30];

    pipe(fd);

    if (!fork()) {
        printf(" HIJO: Escribiendo en el pipe\n");
        write(fd[1], "hola", 5);
        printf(" HIJO: Chau...\n");
        exit(0);
    } else {
        printf("PADRE: Leyendo desde el pipe\n");
        read(fd[0], buffer, 5);
        printf("PADRE: Lei \"%s\"\n", buffer);
        wait(NULL);
    }
    return 0;
}
