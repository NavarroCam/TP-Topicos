#ifndef TIPOS_H_INCLUDED
#define TIPOS_H_INCLUDED

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

#endif // TIPOS_H_INCLUDED
