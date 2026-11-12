#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int main(int argc, char **argv) {
    int fd;
    char buffer[1024];
    const char* fifo = "/tmp/fifo_tenis";
    // borrar FIFO por si existiera previamente
    unlink(fifo);

    /* crear el FIFO */
    if (mkfifo(fifo, 0666)<0) {
        perror("No puede crearse el FIFO");
        return(1);
    }
    /* Abrir el FIFO */
    fd = open(fifo, O_RDONLY);
    if (fd<0) {
        perror("No puede abrirse el FIFO");
        return(1);
    }
    /* Leer de la FIFO */
    int bytes_leer;
    while ((bytes_leer = read(fd,buffer,sizeof(buffer)-1))>0) {
	buffer[bytes_leer] = '\0';
	printf("%s", buffer);
        fflush(stdout);
    }
    close(fd);
    unlink(fifo);
    return(0);
}
