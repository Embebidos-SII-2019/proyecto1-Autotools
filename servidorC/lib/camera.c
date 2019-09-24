#include <stdio.h>
#include <stdlib.h>

#define NAME_IMG "/home/images/img.jpg"

/**
	method to inyect in the console a commad,
	this one takes a picture a saves on the 
	path home/images/img.jpg
*/
void takePicture(void){
	system("fswebcam -S 20 --fps 15 -r 640x480 --no-banner /home/images/img.jpg");	
}

/**
	function to get size of the file.
*/
long int findSize(const char *file_name){
	
    long int size=0;
	FILE *fp;
    /*Open file in Read Mode*/
    fp=fopen(file_name,"r");
    /*Move file point at the end of file.*/
    fseek(fp,0,SEEK_END);
    /*Get the current position of the file pointer.*/
    size=ftell(fp);
    fclose(fp);
    if(size!=-1){
        printf("File size is: %ld\n",size);
		return size;
	}
	printf("There is some ERROR.\n");
	return size;
	
}

/**
	method to load in the memory a
	picture that hace been recently 
	taken, the path is home/images/img.jpg
*/
char * readImg(){
	long int imgSize=findSize(NAME_IMG);
	char *file_name=calloc(sizeof(char), imgSize+1);
   	FILE *fp;
	/*Open file in write mode*/
    fp=fopen(NAME_IMG,"r");
	fgets(file_name, imgSize, (FILE*)fp);
	file_name[imgSize]='\n';
	return file_name;
}