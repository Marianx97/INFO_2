#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_FILE "MIFIFO"

int main(void) {
    FILE *fp;
    char readbuf[80];
    /* Crea el FIFO si no existe */
    mknod(FIFO_FILE, S_IFIFO|0666, 0);
    while(1) {
        fp = fopen(FIFO_FILE, "r"); // Abrir archivo de fifo en modo lectura
        fgets(readbuf, 80, fp);     // Cargar en readbuf 80 chars leidos de fp (del archivo fifo)

        printf("Cadena recibida: %s\n", readbuf);

        fclose(fp);                 // Cerrar el archivo. No lo elimina, solo cierra el acceso desde este proceso
    }
    return(0);
}
