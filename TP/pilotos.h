#ifndef PILOTOS_H_INCLUDED
#define PILOTOS_H_INCLUDED

#include <stdio.h>
#include <string.h>

#include "tipos.h"
#include "archivos.h"

#define PILOTOS_DAT "pilotos.dat"
#define PILOTOS_TXT "../Archivos/pilotos.txt"


void listarPilotos(const char *binPath);

int altaPiloto(const char *binPath);
int bajaPiloto(const char *binPath, const char *bajasPath);
int confirmarModificacion(const char *mensaje);
int modificarPiloto(const char *binPath);
void listarAltas(const char *binPath);
void listarBajas(const char *bajasPath);

void menuPilotos(const char *binPath, const char *bajasPath);

#endif // PILOTOS_H_INCLUDED
