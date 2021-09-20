/* main.c */
/*
Program to print factorials of all integrers between 0 and 20.
This part od the program is contained in the file main.c. The
function factorial() is in the file factorial.c.

To compile: gcc -c ./main.c
            gcc -c ./factorial.c
            gcc ./main.o./factorial.o -> ./a.out
*/
#include <stdio.h>

/* Prototype for function factorial() */
void factorial();

/* Global variable declarations */
int j;
double fact;
int main()
   {
   FILE *op = fopen("factorial.txt", "wt");
   /* Print factorials of all integrers between 0 and 20 */
   for (j = 0; j <= 20; ++j)
      {
      factorial();
      printf(" j = %3d  factorial(j) = %12.3e \n", j, fact);
      fprintf(op," j = %3d  factorial(j) = %12.3e \n", j, fact);
}
fclose(op);
return 0;
}