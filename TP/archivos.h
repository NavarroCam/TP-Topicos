#ifndef ARCHIVOS_H_INCLUDED
#define ARCHIVOS_H_INCLUDED

#include "pilotos.h"
#include "escuderias.h"
#include "carreras.h"

#define OK    0
#define ERROR -1

int cargarPilotosTxtABin(const char *txtPath, const char *binPath, const char *errorPath);
int cargarEscuderiasTxtABin(const char *txtPath, const char *binPath, const char *errorPath);
int exportarPilotosTxt(const char *binPath, const char *txtPath);

#endif // ARCHIVOS_H_INCLUDED
