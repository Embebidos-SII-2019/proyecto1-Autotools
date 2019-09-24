/* blink.c
 *
 * Raspberry Pi GPIO example using sysfs interface.
 * Guillermo A. Amaral B. <g@maral.me>
 *
 * This file blinks GPIO 4 (P1-07) while reading GPIO 24 (P1_18).
 * This library use the BCM format for the pins
 */

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1

#define ERROR -1
#define PASS 0

/**
	method to export and set avaliable a BCM
	port on the board.
*/
int export(int pin){
#define BUFFER_MAX 3
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (ERROR == fd) {
		fprintf(stderr, "Failed to open export for writing!\n");
		return(ERROR);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(PASS);
}

/**
	method to unexport and set free a BCM
	port on the board.
*/
int unexport(int pin){
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (ERROR == fd) {
		fprintf(stderr, "Failed to open unexport for writing!\n");
		return(ERROR);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(PASS);
}

/**
	method to set the mode over the pin, if it's IN=0 
	or OUT=1.
*/
int pinMode(int pin, int mode){
	const char s_directions_str[]  = "in\0out";
	#define DIRECTION_MAX 35
	char path[DIRECTION_MAX];
	int fd;

	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if (ERROR == fd) {
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		return(ERROR);
	}

	if (ERROR == write(fd, &s_directions_str[IN == mode ? 0 : 3], IN == mode ? 2 : 3)) {
		fprintf(stderr, "Failed to set direction!\n");
		return(ERROR);
	}

	close(fd);
	return(PASS);
}

/**
	method to read the value on the 
	BCM port .
*/
int digitalRead(int pin){
#define VALUE_MAX 30
	char path[VALUE_MAX];
	char value_str[3];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);
	if (ERROR == fd) {
		fprintf(stderr, "Failed to open gpio value for reading!\n");
		return(ERROR);
	}

	if (ERROR == read(fd, value_str, 3)) {
		fprintf(stderr, "Failed to read value!\n");
		return(ERROR);
	}

	close(fd);

	return(atoi(value_str));
}

/**
	method to write a value on a 
	BCM port, it may be HIGH=1 or LOW=0.
*/
int digitalWrite(int pin, int value){
	 const char s_values_str[] = "01";

	char path[VALUE_MAX];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if (ERROR == fd) {
		fprintf(stderr, "Failed to open gpio value for writing!\n");
		return(ERROR);
	}

	if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
		fprintf(stderr, "Failed to write value!\n");
		return(ERROR);
	}

	close(fd);
	return(PASS);
}

/**
	method to write on a port to make oscilate a signal, this
	can be used to debug a led
	It needs to be define the 
				->	BCM pin number
				->	Frequency of oscilation
				->	Duration which is half of the cycles
							 set by the frequency.
*/
int blink(int pin, int freq, int duration){
	int deb;
	for(int i=1; i<duration+1; i++){
		deb=digitalWrite(pin, i%2); //if (i%2)==0 -> LOW; else -> HIGH
		if(deb==ERROR){
			return(ERROR);
		}
		usleep(freq);
	}
	return (PASS);
}
