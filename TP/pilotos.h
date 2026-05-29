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
void __menuPilotos(const char* pilotos, const char* escuderias);
void listarPilotos(const char* pilotos);

int compararPuntos(const void* a, const void* b);
int compararIdPiloto(const void* a, const void* b);

unsigned generarNuevoId(const char* v);
int altaPiloto(const char* pilotos, const char* escuderias);

int exportarBajasPilotosTxt(const char* binPath, const char* txtPath);
int bajaPiloto(const char* pilotos);

int confirmarModificacion(const char *mensaje);
int modificarPiloto(const char* pilotos, const char* escuderias);


void mostrarRanking(const char* pilotos);
char* obtenerNombre(const void* p);
unsigned obtenerPuntos(const void* p);

void listarPilotosPorEscuderia_Op1(tda_vector* pilotos, tda_vector* escuderias);
void listarPilotosPorEscuderia_Op2(const char* pilotos, const char* escuderias);

#endif // PILOTOS_H_INCLUDED
