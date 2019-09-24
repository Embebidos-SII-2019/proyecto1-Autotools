#include <gpio.h>
#include <stdio.h>
#include <stdlib.h>

#define ERROR -1

#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1

#define PICTURE 0
#define LIGHT1 1
#define LIGHT2 2
#define LIGHT3 3
#define LIGHT4 4
#define LIGHT5 5

#define PLIGHT1 17
#define PLIGHT2 27
#define PLIGHT3 22
#define PLIGHT4 23
#define PLIGHT5 24

static int lightsS[5];

/**
	method to change the state of a light.
*/
int turnLight(int pin, int light){
	int tt=lightsS[light-1], deb;
	if(tt==HIGH){
		//escribiendo LOW=0 en pin
		deb=digitalWrite(pin, LOW);
		if(deb==ERROR){
			printf("ERROR: pin%d can't be written\n",pin);
		}
		deb=unexport(pin);
		if(deb==ERROR){
			printf("ERROR: pin%d can't be release\n",pin);
		}
		lightsS[light-1]=LOW;
		return 0;
	}
	//habilitando los puertos
	deb=export(pin);
	//revisando errores
	if(deb==ERROR){
		return(1);
	}
	//estableciendo los puertos
	deb=pinMode(pin, OUT);
	if(deb==ERROR){
		return(1);
	}
	//escribiendo HIGH=1 en pin
	deb=digitalWrite(pin, HIGH);
	lightsS[light-1]=HIGH;
}

/**
	method to process the action made by the control.
*/
void* processAction(int action){
	if(action==PICTURE){
		takePicture();
		return readImg();
	}
	else if(action==LIGHT1){
		turnLight(PLIGHT1,LIGHT1);
		return NULL;
	}
	else if(action==LIGHT2){
		turnLight(PLIGHT2,LIGHT2);
		return NULL;
	}
	else if(action==LIGHT3){
		turnLight(PLIGHT3,LIGHT3);
		return NULL;
	}
	else if(action==LIGHT4){
		turnLight(PLIGHT4,LIGHT4);
		return NULL;
	}
	else if(action==LIGHT5){
		turnLight(PLIGHT5,LIGHT5);
		return NULL;
	}
}

