#ifndef CARRERAS_H_INCLUDED
#define CARRERAS_H_INCLUDED

#include "pilotos.h"
#define MAX_PUNTAJES 10
#define CANT_CAMPOS_POSICION 3
#define CARRERAS_DAT "../Archivos/carreras.dat"
#define PUNTAJES_TXT "../Archivos/puntajes.txt"

void menuCarreras(FILE* archCarreras, FILE* archPilotos,FILE* archPuntajes);

int registrarCarrera(FILE* archCarreras, FILE* archPilotos,FILE* archPuntajes);
void mostrarPuntos(void* elem);
int cargarVectorPuntos(tda_vector* v,FILE* pf);
int trozarPuntajes(const char* cad,t_puntajes* puntos);
void* obtenerPuntaje(const tda_vector* v, unsigned pos);
void mostrarResultados(void* elem);

int generarIdCarrera(FILE* archCarreras);
void cargarDatosCarrera(t_carrera* c);
void listarPilotosCarrera(FILE* archPilotos);
int cargarResultadosCarrera(t_carrera* c, const tda_vector* puntos);
void mostrarResultadosCarrera(const t_carrera* c);
int guardarCarrera(FILE* archCarreras, const t_carrera* c);
int cargarPilotosAMemoria(tda_vector* pilotos, FILE* archPilotos);
int cargarPilotosAMemoria(tda_vector* pilotos, FILE* archPilotos);
void guardarPilotos(FILE* archPilotos, const tda_vector* pilotos);
void actualizarPuntosPilotos(tda_vector* pil, const t_carrera* c);

#endif // CARRERAS_H_INCLUDED
