#include "estadisticas.h"

void menuEstadisticas(FILE* Estadisticas)
{
    int op;

    do
    {
        puts("\n===============================================");
        printf("\t\t   ESTADISTICAS\n");
        puts("===============================================");
        printf("1. Estadísticas de un piloto\n");
        printf("2. Top 5 pilotos con más victorias\n");
        printf("3. Piloto con mejor promedio de posición\n");
        printf("0. Volver\n");
        printf("\nOpción: ");
        scanf("%d", &op);
        while (getchar() != '\n');
        system("cls");
        switch (op)
        {
            case 1:
                EstadisticasPiloto(Estadisticas);
                break;
            case 2:
                top5MasVictorias(Estadisticas);
                break;
            case 3:
                mejorPromedioPosicion(Estadisticas);
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
        while(!feof(Estadisticas) && !encontrado)
        {
            if(est.id_piloto == result->id_piloto)
            {
                encontrado = 1;
                est.carreras_corridas++;
                est.total_puntos += result->puntos;
                est.suma_posiciones += result->posicion;
                if(result->posicion == 1)
                    est.victorias++;
                if(result->posicion <= 3)
                    est.podios++;
                if(result->posicion < est.mejor_posicion)
                    est.mejor_posicion = result->posicion;
                if(result->posicion > est.peor_posicion)
                    est.peor_posicion = result->posicion;
                fseek(Estadisticas, -(long)sizeof(t_estadistica), SEEK_CUR);
                fwrite(&est, sizeof(t_estadistica), 1, Estadisticas);
            }
            else
                fread(&est, sizeof(t_estadistica), 1, Estadisticas);
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
            est.suma_posiciones = result->posicion;
            fseek(Estadisticas,0,SEEK_END);
            fwrite(&est,sizeof(t_estadistica),1,Estadisticas);
        }
    }
    fflush(Estadisticas);
}

int EstadisticasPiloto(FILE* pf)
{
    t_estadistica est;
    unsigned idBuscado;
    int encontrado;

    printf("\nIngrese ID del piloto: ");
    scanf("%u", &idBuscado);

    rewind(pf);
    encontrado = 0;
    fread(&est, sizeof(t_estadistica), 1, pf);
    while(!feof(pf) && !encontrado)
    {
        if(est.id_piloto == idBuscado)
        {
            encontrado = 1;
            printf("\n--- ESTADISTICAS PILOTO ID: %u ---\n", est.id_piloto);
            printf("Carreras corridas : %u\n",  est.carreras_corridas);
            printf("Victorias         : %u\n",  est.victorias);
            printf("Podios            : %u\n",  est.podios);
            printf("Puntos totales    : %u\n",  est.total_puntos);
            printf("Mejor posicion    : %u\n",  est.mejor_posicion);
            printf("Peor posicion     : %u\n",  est.peor_posicion);
            printf("Prom. posicion    : %.2f\n", (float)est.suma_posiciones / est.carreras_corridas);
            printf("Prom. puntos/carr : %.2f\n", (float)est.total_puntos / est.carreras_corridas);
        }
        fread(&est, sizeof(t_estadistica), 1, pf);
    }
    if(!encontrado)
        printf("Piloto no encontrado.\n");

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

int top5MasVictorias(FILE* pf)
{
    t_estadistica est;
    t_estadistica top[5];
    int cant = 0;
    int i, j;

    rewind(pf);
    fread(&est, sizeof(t_estadistica), 1, pf);
    while(!feof(pf))
    {
        if(cant < 5)
        {
            top[cant] = est;
            cant++;
            // inserción para mantener ordenado por victorias desc
            j = cant - 1;
            while(j > 0 && top[j].victorias > top[j-1].victorias)
            {
                t_estadistica tmp = top[j];
                top[j] = top[j-1];
                top[j-1] = tmp;
                j--;
            }
        }
        else if(est.victorias > top[4].victorias)
        {
            top[4] = est;
            j = 4;
            while(j > 0 && top[j].victorias > top[j-1].victorias)
            {
                t_estadistica tmp = top[j];
                top[j] = top[j-1];
                top[j-1] = tmp;
                j--;
            }
        }
        fread(&est, sizeof(t_estadistica), 1, pf);
    }

    if(cant == 0)
    {
        printf("Sin estadísticas cargadas.\n");
        return TODOOK;
    }

    printf("\n--- TOP %d PILOTOS CON MAS VICTORIAS ---\n", cant);
    for(i = 0; i < cant; i++)
        printf("%d. ID: %u | Victorias: %u\n", i+1, top[i].id_piloto, top[i].victorias);

    return TODOOK;
}

int mejorPromedioPosicion(FILE* pf)
{
    t_estadistica est;
    t_estadistica mejor;
    float promMejor;
    float promActual;
    int primero;

    rewind(pf);
    primero = 1;
    fread(&est, sizeof(t_estadistica), 1, pf);
    while(!feof(pf))
    {
        if(est.carreras_corridas > 0)
        {
            promActual = (float)est.suma_posiciones / est.carreras_corridas;
            if(primero || promActual < promMejor)
            {
                mejor = est;
                promMejor = promActual;
                primero = 0;
            }
        }
        fread(&est, sizeof(t_estadistica), 1, pf);
    }

    if(primero)
    {
        printf("Sin estadísticas cargadas.\n");
        return TODOOK;
    }

    printf("Piloto con mejor promedio de posicion:\n");
    printf("ID: %u | Promedio: %.2f\n", mejor.id_piloto, promMejor);

    return TODOOK;
}
