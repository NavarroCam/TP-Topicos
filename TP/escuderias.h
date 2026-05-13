#ifndef ESCUDERIAS_H_INCLUDED
#define ESCUDERIAS_H_INCLUDED

#include <stdio.h>
#include <string.h>

typedef struct{
    unsigned id;
    char codigo[4];
    char nombre[30];
    char pais[50];
    int estado;
}t_escuderia;

void listarEscuderias(const char *binPath);

#endif // ESCUDERIAS_H_INCLUDED
