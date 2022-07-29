typedef struct {} key_t;
typedef struct {} shmid_ds;

// Asigna un segmento de memoria compartida (crea o comprueba su existencia)
// - key: identificador del segmento de memoria compartida
// - size: tamaño del segmento de memoria compartida
// - shmflg: indicador de las operaciones a realizar
//   * 0: operaciones de lectura/escritura
//   * SHM_RDONLY: operaciones de solo lectura
int shmget(key_t key, int size, int shmflg);

// Se enlaza con un segmento de memoria compartida
// - shmid: id del segmento de memoria compartida
// - shmaddr: ???
// - shmflg: indicador de las operaciones a realizar
void * scmat(int shmid, const void * shmaddr, int shmflg);

// Se desenlaza de un segmento de memoria compartida
// - shmaddr: ???
int shmdt(const void * shmaddr);

// Operaciones de control sobre un segmento de memoria compartida
// - shmid: id del segmento de memoria compartida
// - cmd: comandos de control
//   * IPC_SET: modificar
//   * IPC_STAT: consultar
//   * IPC_RMID: remover (borrar)
// - buf: ???
int shmctl(int shmid, int cmd, shmid_ds * buf);
