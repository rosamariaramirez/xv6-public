#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char *argv[])
{
  int fdOriginal;
  int fdNew;
  char buf[512];
  int n;

  if(argc != 3){
    printf(2, "Usage cp original new");
    return 1;
  }
  fdOriginal = open(argv[1], O_RDONLY);
  //agregamos permiso de write. o readonlywriteonly.
  fdNew = open(argv[2], O_CREATE |O_WRONLY);

  //ahora tengo que leer el original, guardarlo en buffer y copiarlo al nuevo.
  //n es el tamaño de bytes que leyo. Cuando es cero, es que ya leí todo.
  //si el archivo no existe, se crea.
  while((n = read(fdOriginal, buf, sizeof(buf))) > 0) {
    write(fdNew, buf, n);
  }

  close(fdOriginal);
  close(fdNew);

  exit();
};
