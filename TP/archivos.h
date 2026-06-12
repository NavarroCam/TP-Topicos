#ifndef ARCHIVOS_H_INCLUDED
#define ARCHIVOS_H_INCLUDED

#include "comunes.h"
#include "pilotos.h"
#include "escuderias.h"
#include "carreras.h"

#define TAMCADENA 256
#define SEPARADOR '|'

int cargarPilotosTxtABin(const char *txtPath, const char *binPath);
int cargarEscuderiasTxtABin(const char *txtPath, const char *binPath);
int exportarPilotosTxt(const char *binPath, const char *txtPath);
int exportarEscuderiasTxt(const char *binPath, const char *txtPath);
int generarArchivoOrdenado(const char* nomArch,size_t tamRegistro, int cmp(const void*, const void*));
///////////////
int trozarPilotos(char* cad, t_piloto* p1);
int trozarEscuderias(char* cad, t_escuderia* e1);
#endif // ARCHIVOS_H_INCLUDED
