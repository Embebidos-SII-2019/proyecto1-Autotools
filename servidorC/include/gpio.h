#ifndef _GPIO_H
#define _GPIO_H

//public
int export(int pin);
int unexport(int pin);

int pinMode(int pin, int mode);
int digitalRead(int pin);
int digitalWrite(int pin, int value);
int blink(int pin, int freq, int duration);

#endif
