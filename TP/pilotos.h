#ifndef PILOTOS_H_INCLUDED
#define PILOTOS_H_INCLUDED

#include <stdio.h>
#include <string.h>

#include "archivos.h"
#include "escuderias.h"
#include "tipos.h"
#include "comunes.h"

#define PILOTOS_DAT "../Archivos/pilotos.dat"
#define PILOTOS_TXT "../Archivos/pilotos.txt"  //cambio por txt de Caro
#define PILOTOS_ACT_TXT "../Archivos/pilotosActualizados.txt"


#define BAJAS_PILOTOS_DAT "../Archivos/bajas_pilotos.dat"
#define BAJAS_PILOTOS_TXT "../Archivos/bajas_pilotos.txt"

void menuPilotos(const char *binPath, const char *bajasPath);

void mostrarPiloto(void* pilotos);
void __menuPilotos(FILE* pilotos, FILE* escuderias);
void listarPilotos(FILE* pilotos);

int compararPuntos(const void* a, const void* b);
int compararIdPiloto(const void* a, const void* b);
unsigned generarNuevoIdPilotos(FILE* pilotos);
int altaPiloto(FILE* pilotos, FILE* escuderias);

int exportarBajasPilotosTxt(const char* binPath, const char* txtPath);
int bajaPiloto(FILE* pilotos);

int modificarPiloto(FILE* pilotos, FILE* escuderias);

void mostrarRanking(FILE* pilotos);

void listarPilotosPorEscuderia_Op1(tda_vector* pilotos, tda_vector* escuderias);

void mostrarPilotoCarrera(void* pilotos);
void listarPilotosPorEscuderia(FILE* pilotos, FILE* escuderias);

#endif // PILOTOS_H_INCLUDED
