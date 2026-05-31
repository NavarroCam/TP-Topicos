#ifndef TIPOS_H_INCLUDED
#define TIPOS_H_INCLUDED

#define MAX_RESULTADOS 22

typedef struct{
    unsigned id;
    char nombre[30];
    char nacionalidad[30];
    unsigned id_escuderia;
    unsigned puntos_acumulados;
    char estado;
    unsigned long long fechaNacimiento;
}t_piloto;

typedef struct{
    unsigned id;
    char codigo[4];
    char nombre[30];
    char pais[50];
    int estado;
}t_escuderia;

typedef struct{
    unsigned posicion;
    unsigned id_piloto;
    unsigned puntos;
}t_posicion;

typedef struct{
    unsigned posicion;
    unsigned puntos;
}t_puntajes;

typedef struct{
    int id;
    char circuito[20];
    unsigned long long fecha;
    int estado;
    int cant_resultados;
    t_posicion resultados[MAX_RESULTADOS];
}t_carrera;

#endif // TIPOS_H_INCLUDED
