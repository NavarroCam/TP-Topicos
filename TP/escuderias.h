#ifndef ESCUDERIAS_H_INCLUDED
#define ESCUDERIAS_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include "tipos.h"
#include "comunes.h"

#define ESCUDERIAS_DAT "../Archivos/escuderias.dat"
#define ESCUDERIAS_TXT "../Archivos/escuderias.txt"


void menuEscuderias(const char* escuderias);
void listarEscuderias(const char* binPath);
int compararIdEscuderia(const void* a, const void* b);
int escuderiaValida(unsigned idEsc, const char* escuderias);
int altaEscuderias(const char* esc);
int compararCodEscuderia(const void* a, const void* b);
int BuscarCodigoEscuderia(const char* escuderias, char* cod);
unsigned generarNuevoIdEscuderias(const char* escuderias);

#endif // ESCUDERIAS_H_INCLUDED
