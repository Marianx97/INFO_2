// #include <sys/types.h>
// #include <sys/ipc.h>

typedef struct {} key_t;
typedef struct {} msqid_ds;

typedef struct  {
    long mtype;     // tipo de mensaje, debe ser > 0
    char mtext[1];  // datos del mensaje, mínimo 1 byte
} msgbuf;

/*
    Se puede usar cualquier tipo de estructura para trasnferir datos.
    Es importante que el primer elemento sea un long:

    typedef struct  {
        long mtype;
        char Apenom[30];
        long legajoUTN;
        float promedio;
    } msgbuf;
*/

// genera un identificador a partir de 2 argumentos
// - path: archivo que el proceso pueda leer
// - id: un char escogido arbitrariamente
key_t ftock(const char * path, int id);
// Lo importante es que los procesos que vayan a usar la misma cola,
// se conecten usando el mismo identificador

// Conecta con una cola o la crea si no existe, retorna el identificador (msqid) de la misma
// - key: identificador unívoco de la cola.
// - msgflg: flag que indica qué hacer con la cola. Para crear IPC_CREAT
int msgget(key_t key, int msgflg);

// envía mensaje a la cola
// - msqid: id de la cola
// - msgp: puntero a la estructura donde se encuentra el mensaje
// - msgsz: tamaño del mensaje
// - msgflg: flags para tratar el mensaje en la cola
int msgsnd(int msqid, msgbuf * msgp, int msgsz, int msgflg);

// recibe mensaje de la cola
// - msqid: id de la cola
// - msgp: puntero a la estructura donde se almacenará el mensaje
// - msgsz: tamaño del mensaje
// - msgtype: permite recuperar mensajes según prioridades
//    * = 0: Lee el próximo mensaje de la cola, independiente del mtype
//    * > 0: Lee el próximo mensaje de la cola cuyo mtype es igual al seleccionado
//    * < 0: Lee el primer mensaje con mtype menor o igual al valor absoluto de msgtype
// - msgflg: flags para tratar el mensaje de la cola
int msgrcv(int msqid, msgbuf * msgp, int msgsz, long msgtype, int msgflg);

// elimina la cola
// - msqid: id de la cola
// - cmd: le indica qué hacer a msgctl (se utiliza generalmente IPC_RMID, remover id)
// - buf: puede quedar como NULL si se utiliza IPC_RMID
int msgctl(int msqid, int cmd, msqid_ds * buf);

/* -------------------------------------------------------------------------------------- */

/*
    Ejemplo:
    key = ftok("/home/info2/archivo.txt", "b");
    msqid = msgget(key, 0666 | IPC_CREAT);
    msgsnd(msqid, &msgp, sizeof(msgp), 0) // añadir a la cola
    ...
    msgrcv(msqid, &msgp, sizeof(msgp), 0) // añadir a la cola
    ...
    msgctl(msqid, IPC_RMID, NULL);
*/
