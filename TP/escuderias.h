#ifndef ESCUDERIAS_H_INCLUDED
#define ESCUDERIAS_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include "tipos.h"
#include "comunes.h"


#define ESCUDERIAS_DAT "../Archivos/escuderias.dat"
#define ESCUDERIAS_TXT "../Archivos/escuderias.txt"
#define ESCUDERIAS_ACT_TXT "../Archivos/escuderiasActualizado.txt"

#define BAJAS_ESCUDERIAS_DAT "../Archivos/bajas_escuderias.dat"
#define BAJAS_ESCUDERIAS_TXT "../Archivos/bajas_escuderias.txt"


void menuEscuderias(FILE* pilotos, FILE* escuderias);
void listarEscuderias(FILE* escuderias);
void altaEscuderias(FILE* escuderias);
int BuscarCodigoEscuderia(FILE* escuderias, const char* cod);
unsigned generarNuevoIdEscuderias(FILE* escuderias);
int exportarBajasEscuderiasTxt(const char* binPath, const char* txtPath);
int bajaEscuderia(FILE* escuderias);
//int compararIdEscuderia(const void* a, const void* b);
int escuderiaValida(unsigned idEsc, FILE* escuderias);
//int compararCodEscuderia(const void* a, const void* b);
int modificarEscuderia(FILE* escuderias);

#endif // ESCUDERIAS_H_INCLUDED
