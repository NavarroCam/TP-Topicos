#ifndef ESCUDERIAS_H_INCLUDED
#define ESCUDERIAS_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include "tipos.h"
#include "comunes.h"

#define ESCUDERIAS_DAT "../Archivos/escuderias.dat"
#define ESCUDERIAS_TXT "../Archivos/escuderias.txt"


void menuEscuderias(FILE* pilotos, FILE* escuderias);
void listarEscuderias(FILE* escuderias);
void altaEscuderias(FILE* escuderias);
int BuscarCodigoEscuderia(FILE* escuderias, const char* cod);
unsigned generarNuevoIdEscuderias(FILE* escuderias);
void bajaEscuderia(FILE* escuderias);
//int compararIdEscuderia(const void* a, const void* b);
//int escuderiaValida(unsigned idEsc, const char* escuderias);
//int compararCodEscuderia(const void* a, const void* b);


#endif // ESCUDERIAS_H_INCLUDED
