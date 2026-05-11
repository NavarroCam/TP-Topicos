#ifndef PILOTOS_H_INCLUDED
#define PILOTOS_H_INCLUDED

typedef struct{
    unsigned id;
    char nombre[30];
    char nacionalidad[30];
    unsigned id_escuderia;
    unsigned puntos_acumulados;
    char estado;
    unsigned long long fechaNacimiento;
}t_piloto;


#endif // PILOTOS_H_INCLUDED
