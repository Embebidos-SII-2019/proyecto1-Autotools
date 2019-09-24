#include <unistd.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h> 
#include <netdb.h>
#include <gpio.h>

//errores
#define ERROR1 "ERROR opening socket\0"
#define ERROR2 "ERROR on binding\0"
#define ERROR3 "ERROR on accept\0"
#define ERROR4 "ERROR on fork\0"
#define ERROR5 "ERROR writing to socket\0"
#define ERROR6 "ERROR reading from socket\0"
#define LEN_MSG 3

#define NAME_IMG "/home/images/img.jpg"

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

#define PDOOR1 25
#define PDOOR2 5
#define PDOOR3 6
#define PDOOR4 16
#define PDOOR5 26

#define ERROR -1

#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1

//bloqueo mutex
static pthread_mutex_t tMsg=PTHREAD_MUTEX_INITIALIZER;
//arrays to control the state of the door in the n-1 time 
//and the n time.
static int doors[5];

static int lightsS[5];
//datos propios de la instanciacion del servidor
int _sockfd, _newsockfd, _portno, _n;// _codigos;
socklen_t _clilen;
struct sockaddr_in _serv_addr, _cli_addr;


/**
	method to inyect in the console a commad,
	this one takes a picture a saves on the 
	path home/images/img.jpg
*/
void takePicture(void){
	char cmmd[500];
	sprintf(cmmd,"./home/images/takePicture.sh");
	system(cmmd);	
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


/**
	method to change the state of a light.
*/
int turnLight(int pin, int light){
	int tt=lightsS[light-1], deb;
	if(tt==HIGH){
		printf("apagando luz %d \n", light);
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
		return(-1);
	}
	//estableciendo los puertos
	deb=pinMode(pin, OUT);
	if(deb==ERROR){
		return(-1);
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


/*
	method to print the error
*/
void error(const char* msg){
	perror(msg);
    exit(1);
}

/**
	method to send data to the control
*/
void sendToControl(void * data, int lenght){
	printf("enviando dato a cliente \n");
	pthread_mutex_lock(&tMsg);
	_n=send(_newsockfd, data, lenght,0);
	pthread_mutex_unlock(&tMsg);
	if (_n < 0) 
		error(ERROR5);
}

/**
	method to calculate the len of a 
	string, it need to be finish with a 
	\n
*/
int lenData(char * data){
	int i=0;
	for(; data[i]!='\n'; i++){}
}

/**
	method to check the state of the doors in case 
	that someone opens it 
*/
void * checkingDoors(){
	int deb=export(PDOOR1), deb5=pinMode(PDOOR1, IN);
	int deb1=export(PDOOR2), deb6=pinMode(PDOOR2, IN);
	int deb2=export(PDOOR3), deb7=pinMode(PDOOR3, IN);
	int deb3=export(PDOOR4), deb8=pinMode(PDOOR4, IN);
	int deb4=export(PDOOR5), deb9=pinMode(PDOOR5, IN);
	if(deb==ERROR && deb1==ERROR && deb2==ERROR && deb3==ERROR && deb4==ERROR){
		printf("ERROR: can't be exported the port\n");
		return(NULL);
	}
	if(deb5==ERROR && deb6==ERROR && deb7==ERROR && deb8==ERROR && deb9==ERROR){
		printf("ERROR: can't be set the mode of the pin\n");
		return(NULL);
	}
	doors[0]=digitalRead(PDOOR1);
	doors[1]=digitalRead(PDOOR2);
	doors[2]=digitalRead(PDOOR3);
	doors[3]=digitalRead(PDOOR4);
	doors[4]=digitalRead(PDOOR5);
    while(1<2){
		deb5=digitalRead(PDOOR1);
		deb6=digitalRead(PDOOR2);
		deb7=digitalRead(PDOOR3);
		deb8=digitalRead(PDOOR4);
		deb9=digitalRead(PDOOR5);
		if(doors[0]!=deb5){
			printf("se ha cambiado de estado la puerta 1\n");
			char * tt="P1\n";
			sendToControl(tt,3);
			doors[0]=deb5;
		}
		if(doors[1]!=deb6){
			printf("se ha cambiado de estado la puerta 2\n");
			char * tt="P2\n";
			sendToControl(tt,3);
			doors[1]=deb6;
		}
		if(doors[2]!=deb7){
			printf("se ha cambiado de estado la puerta 3\n");
			char * tt="P3\n";
			sendToControl(tt,3);
			doors[2]=deb7;
		}
		if(doors[3]!=deb8){
			printf("se ha cambiado de estado la puerta 4\n");
			char * tt="P4\n";
			sendToControl(tt,3);
			doors[3]=deb8;
		}
		if(doors[4]!=deb9){
			printf("se ha cambiado de estado la puerta 5\n");
			char * tt="P5\n";
			sendToControl(tt,3);
			doors[4]=deb9;
		}
	}
    pthread_exit(NULL);
}

/*
	metho to thread the messages comming form the client.
*/
void * listenClient(){
	char * almacenador= calloc(LEN_MSG,1);
    while(1<2){
		_n = recv(_newsockfd,almacenador,LEN_MSG,0);
		if (_n < 0)
			error(ERROR6);
		int tt; 
		sscanf((const char*)almacenador, "%d", &tt); 
		printf("mensaje recibido de control: %d\n",tt);
		//manejar mensajes del cliente
		if(tt==PICTURE){
			printf("tomando foto\n");
			char * msg=processAction(tt);
			int len=lenData(msg);
			sendToControl(msg,len);
			continue;
		}
		//the action most be turn off/on a light
		//then when don't need to send back anything
		processAction(tt);
    }
    free(almacenador);
    pthread_exit(NULL);
	
}

/*
	method to make the main loop to wait for connections 
	form the remote device
*/
void * severLoop(void* pPort){
	_portno=*((int*)pPort);
	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    //resivision de si la conexion del socket 
    // fue positiva o fallida.
    if (_sockfd <0)
        error(ERROR1);
    //se escibe 0s en la variables puesta.
    //esto garantiza que no se use memoria sucia.
    bzero((char *) &_serv_addr, sizeof(_serv_addr));
    //establecemos los datos que se van a utilizar 
    //en el socket.
    _serv_addr.sin_family = AF_INET;
    _serv_addr.sin_addr.s_addr = INADDR_ANY;
    _serv_addr.sin_port = htons(_portno);
    if (bind(_sockfd, (struct sockaddr *) &_serv_addr, sizeof(_serv_addr))<0)
        error(ERROR2);
    //se pone a escuchar en el socket si hay nuevas conexiones.
    listen(_sockfd,5);
    _clilen = sizeof(_cli_addr);
	printf("esperando nueva conexion\n");
	_newsockfd = accept(_sockfd, (struct sockaddr*) &_cli_addr, &_clilen);
	if (_newsockfd < 0){
		error(ERROR3);
	}
	//creacion del hilo para el cliente.
	pthread_t tControl;
	int iter=pthread_create(&tControl,NULL,listenClient,NULL);
	if(iter){
		error(ERROR4);
	}
	printf("servidor: got connection from %s port %d\n",inet_ntoa(_cli_addr.sin_addr), ntohs(_cli_addr.sin_port));
	
	//creacion del hilo para avisar de las puertas.
	pthread_t tDoor;
	iter=pthread_create(&tDoor,NULL,checkingDoors, NULL);
	if(iter){
		error(ERROR4);
	}
	pthread_join(tControl, NULL);
	pthread_join(tDoor, NULL);
	
    close(_sockfd);
    pthread_exit(NULL);
}
