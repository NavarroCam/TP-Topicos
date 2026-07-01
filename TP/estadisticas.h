#ifndef ESTADISTICAS_H_INCLUDED
#define ESTADISTICAS_H_INCLUDED

#include "archivos.h"

#define ESTADISTICAS_DAT "../Archivos/estadisticas.dat"

void menuEstadisticas(FILE* archEstadisticas);
void actualizarEstadisticas(FILE* Estadisticas, const t_carrera* c);
int EstadisticasPiloto(FILE* pf);
int pilotoMasVictorias(FILE* pf, t_estadistica* mejor);
int top5MasVictorias(FILE* pf);
int mejorPromedioPosicion(FILE* pf);

#endif // ESTADISTICAS_H_INCLUDED
