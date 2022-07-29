#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

/*
	Los PIPES son un método de comunicación entre procesos (IPC).
	Consiste en utilizar 2 files descriptors para la comunicación.
	Cada Pipe es unidireccional.
	- fd[0] es de solo lectura.
	- fd[1] es de solo escritura.
*/

int main() {
	int fd[2];          // Array de enteros de tamaño 2 -> 2 file descriptors
	char buffer[30];    // Array de 30 chars (caracteres)

    /* pipe() devuelve 0 en caso de éxito o –1 en caso de error */
    if (pipe(fd) == -1) {
		perror("pipe");
		exit(1);
	}

    printf("Escribiendo en el descriptor de archivo #%d\n", fd[1]);
	write(fd[1], "prueba", 5);
	printf("Leyendo desde el descriptor de archivo #%d\n", fd[0]);
	read(fd[0], buffer, 5);
	printf("Leído \"%s\"\n", buffer);
	return 0;
}
