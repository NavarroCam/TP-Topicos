#ifndef PILOTOS_H_INCLUDED
#define PILOTOS_H_INCLUDED

#include <stdio.h>
#include <string.h>

#include "archivos.h"
#include "tipos.h"

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

void mostrarPiloto(void* pilotos);
int cargarEnTDA(const char* archNom, tda_vector* v, size_t tamElem);
void __menuPilotos();
void _listarPilotos(tda_vector* v);
#endif // PILOTOS_H_INCLUDED
