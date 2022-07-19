#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define FIFO_FILE "MIFIFO"

int main(int argc, char *argv[]) {
	FILE *fp;
	if ( argc != 2 ) {
		printf("USO: fifoclient [cadena]\n");
		exit(1);
	}
    /* Abrir archivo de fifo en modo escritura */
	if((fp = fopen(FIFO_FILE, "w")) == NULL) {
        // Si fp = fopen(FIFO_FILE, "w") es NULL,
        // entonces hubo un error al abrir el archivo
		perror("fopen");
		exit(1);
	}
	fputs(argv[1], fp); // Escribo el argumento con el que se ejecutó el programa en el archivo fifo
	fclose(fp);         // Cierro el archivo.
	return(0);
}
