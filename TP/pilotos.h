#ifndef PILOTOS_H_INCLUDED
#define PILOTOS_H_INCLUDED

#include <stdio.h>
#include <string.h>

#include "archivos.h"


typedef struct{
    unsigned id;
    char nombre[30];
    char nacionalidad[30];
    unsigned id_escuderia;
    unsigned puntos_acumulados;
    char estado;
    unsigned long long fechaNacimiento;
}t_piloto;

void listarPilotos(const char *binPath);

int altaPiloto(const char *binPath);
int bajaPiloto(const char *binPath, const char *bajasPath);
int confirmarModificacion(const char *mensaje);
int modificarPiloto(const char *binPath);
void listarAltas(const char *binPath);
void listarBajas(const char *bajasPath);

void menuPilotos(const char *binPath, const char *bajasPath);

#endif // PILOTOS_H_INCLUDED
