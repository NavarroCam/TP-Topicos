#include "estadisticas.h"

void menuEstadisticas(FILE* Estadisticas)
{
    int op;

    do
    {
        puts("\n\n===============================================");
        printf("\t\t   CARRERAS\n");
        puts("===============================================");
        printf("1. Estadísticas de un piloto\n");
        printf("2. Top 5 pilotos con más victorias\n");
        printf("3. Piloto con mejor promedio de posición\n");
        printf("0. Volver\n");
        printf("\nOpción: ");
        scanf("%d", &op);
        while (getchar() != '\n');
        switch (op)
        {
        case 1:
            EstadisticasPiloto(Estadisticas);
            break;
        case 2:
            break;
        case 3:
            break;
        case 0:
            break;
        default:
            printf("Opción inválida.\n");
        }
    }
    while(op != 0);
}

void actualizarEstadisticas(FILE* Estadisticas, const t_carrera* c)
{
    t_estadistica est;
    t_posicion* result;
    int i;
    int encontrado;
    rewind(Estadisticas);
    for(i=0; i<c->cant_resultados; i++)
    {
        result = (t_posicion*)(c->resultados + i);

        rewind(Estadisticas);
        encontrado = 0;

        fread(&est,sizeof(t_estadistica),1,Estadisticas);
        while(!feof(Estadisticas))
        {
            if(est.id_piloto == result->id_piloto)
            {
                encontrado = 1;

                est.carreras_corridas++;
                est.total_puntos += result->puntos;

                if(result->posicion == 1)
                    est.victorias++;

                if(result->posicion <= 3)
                    est.podios++;

                if(result->posicion < est.mejor_posicion)
                    est.mejor_posicion = result->posicion;

                if(result->posicion > est.peor_posicion)
                    est.peor_posicion = result->posicion;

                fseek(Estadisticas, -(long)sizeof(t_estadistica),SEEK_CUR);

                fwrite(&est, sizeof(t_estadistica), 1, Estadisticas);

                break;
            }
            fread(&est,sizeof(t_estadistica),1,Estadisticas);
        }

        if(!encontrado)
        {
            est.id_piloto = result->id_piloto;
            est.carreras_corridas = 1;
            est.victorias = result->posicion == 1;
            est.podios = result->posicion <= 3;
            est.total_puntos = result->puntos;
            est.mejor_posicion = result->posicion;
            est.peor_posicion = result->posicion;

            fseek(Estadisticas,0,SEEK_END);

            fwrite(&est,sizeof(t_estadistica),1,Estadisticas);
        }
    }

    fflush(Estadisticas);
}
int EstadisticasPiloto(FILE* pf)
{
    t_estadistica mejor;
    //PILOTOS CON MAS VICTORIAS
    pilotoMasVictorias(pf,&mejor);
    printf("%4u %4u", mejor.id_piloto,mejor.victorias);
    ///FALTAN MAS
    return TODOOK;
}

int pilotoMasVictorias(FILE* pf, t_estadistica* mejor)
{
    t_estadistica est;

    rewind(pf);

    if(!fread(mejor,sizeof(t_estadistica),1,pf))
        return ERROR_ARCH;
    fread(&est,sizeof(t_estadistica),1,pf);
    while(!feof(pf))
    {
        if(est.victorias > mejor->victorias)
            *mejor = est;
        fread(&est,sizeof(t_estadistica),1,pf);
    }

    return TODOOK;
}
