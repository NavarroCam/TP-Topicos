#ifndef ESCUDERIAS_H_INCLUDED
#define ESCUDERIAS_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include "tipos.h"
#include "comunes.h"

#define ESCUDERIAS_DAT "escuderias.dat"
#define ESCUDERIAS_TXT "../Archivos/escuderias.txt"


void listarEscuderias(const char *binPath);
int compararIdEscuderia(const void* a, const void* b);
int escuderiaValida(unsigned idEsc, tda_vector* v);

#endif // ESCUDERIAS_H_INCLUDED
