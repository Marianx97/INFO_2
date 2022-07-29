int main() {
    key_t Clave;
    int Id_Memoria;
    int *Memoria = NULL;
    int i,j;

    /*
        Igual que en el programa anterior, obtenemos una clave para
        la memoria compartida
    */

    Clave = ftok ("/bin/ls", 33);
    if (Clave == -1) {
        printf("No consigo clave para memoria compartida");
        exit(0);
    }

    /*
        Obtenemos el id de la memoria. Al no poner el flag IPC_CREAT,
        estamos suponiendo que dicha memoria ya está creada.
    */

    Id_Memoria = shmget (Clave, sizeof(int)*100, 0777 );
    if (Id_Memoria == -1) {
        printf("No consigo Id para memoria compartida");
        exit (0);
    }

    /*
        Asignamos el puntero a la memoria compartida
    */

    Memoria = (int *)shmat (Id_Memoria, (char *)0, 0);
    if (Memoria == NULL) {
        printf("No consigo memoria compartida");
        exit (0);
    }

    /*
        Leemos el valor de la memoria con demora de un segundo
        y mostramos en pantalla dicho valor. Debería ir cambiando
        según el otro proceso lo va modificando.
    */

    for (i=0; i<10; i++) {
        printf("Leido %d", Memoria[i]);
        sleep (1);
    }

    /*
        Desvinculamos el puntero de la memoria compartida.
        Suponemos que el proceso que la ha creado, la liberará.
    */

    if (Id_Memoria != -1) {
        shmdt ((char *)Memoria);
    }
    return 0;
}
