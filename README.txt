Instituto Tecnologico de Costa Rica
CE-5303 Introducción a los Sistemas Embebidos
II Sem 2019

Ellioth Ramirez Trejos /2014057732
Cristian Roberto Castillo Mcquiddy / 2014061245

Depedencias
  #Distribucion de linux armado con yocto, para arm
  #gcc version 7.4.0
  #fswebcam 
 
Instalacion y ejecución
    Comandos para crosscompilar
    
    Se ejecuta en terminal, dentro de la carpeta principal del proyecto 
        #$ autoreconf --install
    
    Para limpiar el compilado se hace ejecuta el comando 
        #$ sudo rm -rf *
        
    Ahora se se crea una carpeta build dentro de la principal, se accede a build y se crea una llamada usr, 
        #$ mkdir build && cd build
        #$ mkdir usr

    Ahora se va a definir en que plataforma y en que lugar se va a guardar el compilado( puede ser en una carpeta 
    diferente que la del proyecto)
        #$ ../configure --host=arm-poky-linux -gnueabi --prefix=”lugar donde queremos instalarlo”/build/usr

    Sin salirse de la carpeta build del proyecto, ejecutamos el comando
        #$ make -j# cantidad de procesadores que posee la compu para que lo ejecute más rápido

    Ahora se hace la instalación completa
        #$ make install

    Y en la carpeta usr ya tenemos los compilados que queremos para la rasp
    Ahora es necesario mover las carpetas bin, lib y include dentro de la sd, la cual ya debe de tener la 
    imagen instalada,
    Ahora nos trasladamos a la carpeta de los compilados, myInstalacion/build/usr
    Dentro de esta carpeta, vamos a ejecutar el comando 
        #$ sudo cp -r * /media/myUser/idDeSd/
        
    Esto lo que va a hacer es copiar y unir los contenidos de las carpetas bin, lib y include en las nativas de 
    la rasp, es necesario escoger la dirección que tiene las carpetas especificadas.
    Se desmonta la tarjeta de la computadora, se monta en la rasp, se enciende, una vez que haya terminado de cargar, 
    llegó a la terminal, va a pedir el usuario, se escribe el nombre 
        #$ root 

    deja pasar y se escribe el comando
        #$ cd /

    Esto va a hacer que el vaya a la carpeta root de la rasp, ahora se escribe el siguiente comando,
        #$ ./bin/ExeName

    En donde dice ExeName debe ser igual al que establecimos en el Makefile.am de la carpeta src.

    Cuando se usa una tercera biblioteca externa, es mejor, que al momento de llamarla, se copien 
    los archivos de las carpetas lib y src y sean copiadas en la carpeta principal luego se compila.
