#include "types.h"
#include "user.h"

int main(int argc, char *argv[]){
	uint shmid;
	char * mem;


	if((shmid = shmem(1)) == -1){
		printf(1,"Error  on shmid\n");
		exit();
	}

  if((mem = (char *)shmat(shmid)) == 0){
		printf(1,"Error  on shmat\n");
		exit();
	}

	//checkf if already there is some data
	if(mem[0] != 0)
		printf(1,"%s\n",mem);
	else
		strcpy(mem,argv[1]);
    printf(1, "value saved");
	//Check after copying

  //deatach from the shared memory
  shdemat(shmid);
	exit();
}
