/*****************************************************************************
* FILE: dotprod_mutex.c
* DESCRIPTION:
*   This example program illustrates the use of mutex variables 
*   in a threads program. This version was obtained by modifying the
*   serial version of the program (dotprod_serial.c) which performs a 
*   dot product. The main data is made available to all threads through 
*   a globally accessible  structure. Each thread works on a different 
*   part of the data. The main thread waits for all the threads to complete 
*   their computations, and then it prints the resulting sum.
* SOURCE: Vijay Sonnad, IBM
* LAST REVISED: 01/29/09 Blaise Barney
******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
pid_t pid1, pid2;

 int a = 10;
 int b = 100;

 printf("init: a = %d b = %d.\n ", a, b);

 if((pid1 = fork()) <0)
 {
   printf("fork error");
   sleep(60);
   exit(-1);

 }
 else if (pid1 == 0)
 {
   a = a + 10;
   printf("first: a = %d b = %d.\n ", a, b);
 }

 if((pid2 = fork()) < 0)
 {
   printf("fork error");
   exit(-1);
 }
else if (pid2 == 0 )
{
   b = b + 200;
   printf("second: a = %d b = %d.\n ", a, b);
}
sleep(60);
}