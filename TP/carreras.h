#ifndef CARRERAS_H_INCLUDED
#define CARRERAS_H_INCLUDED

typedef struct{
    unsigned posicion;
    unsigned id_piloto;
    unsigned puntos;
}t_posicion;

typedef struct{
    int id;
    char circuito[20];
    unsigned long long fecha;
    int estado;
    int cant_resultados;
    t_posicion** matResultados;
}t_carrera;

#endif // CARRERAS_H_INCLUDED
