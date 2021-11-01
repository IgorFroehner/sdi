#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// externos a biblioteca tools
double wtime();

// Biblioteca p/ vetores
char *readline (FILE *file);
int writeline (char *msg, FILE *file);
char *ltrim(char *s);
char *rtrim(char *s);
char *trim(char *s);