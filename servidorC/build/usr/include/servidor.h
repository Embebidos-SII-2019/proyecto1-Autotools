#ifndef _SERVIDOR_H
#define _SERVIDOR_H

//public
void * severLoop(void* pPort);
void takePicture(void);
char * readImg();
void* processAction(int action);

//private
void * checkingDoors();
void sendToControl(void * data, int lenght);
int lenData(char * data);
long int findSize(const char *file_name);

#endif
