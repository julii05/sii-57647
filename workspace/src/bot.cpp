#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include "DatosMemCompartida.h"

using namespace std;

int main()
{
    DatosMemCompartida* datos;

    // Abrir el archivo de memoria compartida
    int fd = open("/tmp/datos_tenis", O_RDWR);

    if (fd == -1) {
        perror("Error al abrir memoria compartida");
        return 1;
    }

    // Mapear el archivo en memoria
    void* memoria = mmap(NULL, sizeof(DatosMemCompartida),
                         PROT_READ | PROT_WRITE,
                         MAP_SHARED, fd, 0);

    if (memoria == MAP_FAILED) {
        perror("Error al mapear memoria compartida");
        close(fd);
        return 1;
    }

    datos = (DatosMemCompartida*)memoria;

    // Cerrar el descriptor
    close(fd);

    // Control automático de la raqueta
    while (true) {
        float pelotaY = datos->esfera.centro.y;
        float raquetaCentroY = (datos->raqueta1.y1 + datos->raqueta1.y2) / 2.0f;

        if (pelotaY > raquetaCentroY + 0.2f) {
            datos->accion = 1;
        }
        else if (pelotaY < raquetaCentroY - 0.2f) {
            datos->accion = -1;
        }
        else {
            datos->accion = 0;
        }

        usleep(25000);
    }

    munmap(memoria, sizeof(DatosMemCompartida));

    return 0;
}
