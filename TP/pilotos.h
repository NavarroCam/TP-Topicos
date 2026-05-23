#ifndef PILOTOS_H_INCLUDED
#define PILOTOS_H_INCLUDED

#include <stdio.h>
#include <string.h>

#include "archivos.h"
#include "tipos.h"

#define PILOTOS_DAT "pilotos.dat"
#define PILOTOS_TXT "../Archivos/pilotos.txt"

void menuPilotos(const char *binPath, const char *bajasPath);

void mostrarPiloto(void* pilotos);
int cargarEnTDA(const char* archNom, tda_vector* v, size_t tamElem);
void __menuPilotos();
void listarPilotos(tda_vector* v);
int compararPuntos(const void* a, const void* b);
#endif // PILOTOS_H_INCLUDED
