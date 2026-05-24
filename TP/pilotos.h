#ifndef PILOTOS_H_INCLUDED
#define PILOTOS_H_INCLUDED

#include <stdio.h>
#include <string.h>

#include "archivos.h"
#include "escuderias.h"
#include "tipos.h"
#include "comunes.h"

#define PILOTOS_DAT "pilotos.dat"
#define PILOTOS_TXT "../Archivos/pilotos.txt"

void menuPilotos(const char *binPath, const char *bajasPath);

void mostrarPiloto(void* pilotos);
int cargarEnTDA(const char* archNom, tda_vector* v, size_t tamElem);
void __menuPilotos(tda_vector* pilotos, tda_vector* escuderias);
void listarPilotos(tda_vector* v);
int compararPuntos(const void* a, const void* b);

unsigned generarNuevoId(tda_vector* v);
int altaPiloto(tda_vector* v, tda_vector* esc);

#endif // PILOTOS_H_INCLUDED
