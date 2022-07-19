/* signal.c - Contar el número de CTRL-C en 15 segundos */
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int numcortes = 0;      /* Contador de CTRL-C */
int bucle = 1;          /* Controlador de salida del bucle de espera */
void alarma(void);      /* Captura la señal de alarma SIGALRM */
void cortar(void);      /* Captura la señal de interrupción SIGINT */

int main () {
    /* Definimos qué funciones actuaran de handler para cada señal */
    signal(SIGINT, cortar);
    signal(SIGALRM, alarma);

    printf("Ejemplo de signal.\n");
    printf("Pulsa varias veces CTRL-C durante 15 segundos.\n");
    alarm(15);
    while(bucle);
    signal(SIGINT, SIG_IGN); // Indicamos que ignore la señal de interrupción
    printf("Intentaste finalizar %d veces.\n", numcortes);
    printf("Chau!!!.\n");
    exit(0);
}

void alarma () {
    signal(SIGALRM, SIG_IGN);
    bucle = 0;    /* Salir del bucle */
    printf("¡Alarma!\n");
}

void cortar (){
    signal(SIGINT, SIG_IGN);
    printf("Pulsaste CTRL-C\n");
    numcortes++;
    signal(SIGINT, cortar);
}
