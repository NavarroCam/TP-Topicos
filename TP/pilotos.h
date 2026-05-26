#ifndef PILOTOS_H_INCLUDED
#define PILOTOS_H_INCLUDED

#include <stdio.h>
#include <string.h>

#include "archivos.h"
#include "escuderias.h"
#include "tipos.h"
#include "comunes.h"

#define PILOTOS_DAT "../Archivos/pilotos.dat"
#define PILOTOS_TXT "../Archivos/pilotos.txt"

#define BAJAS_PILOTOS_DAT "bajas_pilotos.dat"
#define BAJAS_PILOTOS_TXT "bajas_pilotos.txt"

void menuPilotos(const char *binPath, const char *bajasPath);

void mostrarPiloto(void* pilotos);
void __menuPilotos(tda_vector* pilotos, tda_vector* escuderias);
void listarPilotos(tda_vector* v);

int compararPuntos(const void* a, const void* b);
int compararIdPiloto(const void* a, const void* b);

unsigned generarNuevoId(tda_vector* v);
int altaPiloto(tda_vector* v, tda_vector* esc);

int exportarBajasPilotosTxt(const char* binPath, const char* txtPath);
int bajaPiloto(tda_vector* v);

int confirmarModificacion(const char *mensaje);
int modificarPiloto(tda_vector* v, tda_vector* esc);


void mostrarRanking(tda_vector* v);
char* obtenerNombre(const void* p);
unsigned obtenerPuntos(const void* p);
#endif // PILOTOS_H_INCLUDED
