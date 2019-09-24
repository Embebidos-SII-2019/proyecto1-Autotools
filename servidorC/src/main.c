#include <servidor.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define PORT 5000

int main(){
	
	//arrancar servidor
	int pp=PORT;
	pthread_t tServ;
	int iter=pthread_create(&tServ,NULL,severLoop, &pp);
	if(iter){
		fprintf(stderr,"Error - pthread_create() return code: %d\n",iter);
		exit(EXIT_FAILURE);
	}
	pthread_join(tServ,NULL);
	return 0;
}