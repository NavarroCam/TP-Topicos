#include "estadisticas.h"

void menuEstadisticas(FILE* Estadisticas)
{
    int op;
    t_estadistica mejorPiloto;

    do
    {
        limpiarPantalla();
        tituloSistema();
        tituloMenu("      ESTADÍSTICAS");

        color(COLOR_TEXTO);

        printf("   [1] Estadisticas de un piloto\n");
        printf("   [2] Top 5 pilotos con mas victorias\n");
        printf("   [3] Mejor promedio de posicion\n");
        printf("   [4] Piloto con mas victorias\n");
        printf("   [0] Volver\n");

        color(COLOR_TITULO);
        printf("\n====================================================\n\n");
        restaurarColor();
        printf("Seleccione una opción > ");
        scanf("%d", &op);
        while (getchar() != '\n');

        switch (op)
        {
            case 1:
                EstadisticasPiloto(Estadisticas);
                system("pause");
                break;
            case 2:
                top5MasVictorias(Estadisticas);
                system("pause");
                break;
            case 3:
                mejorPromedioPosicion(Estadisticas);
                system("pause");
                break;
            case 4:
                limpiarPantalla();
                tituloSistema();
                if(pilotoMasVictorias(Estadisticas,&mejorPiloto)==TODOOK)
                {
                    color(COLOR_TEXTO);
                    tituloMenu("   RÉCORD DE VICTORIAS");
                    restaurarColor();
                    printf("ID del Piloto: %u\n", mejorPiloto.id_piloto);
                    printf("Cantidad de Victorias: %u\n", mejorPiloto.victorias);
                }
                else
                    printf("No se pudieron calcular las estadísticas.\n");
                system("pause");
                break;
            case 0:
                break;
            default:
                printf("Opción inválida.\n");
        }
    }while(op != 0);
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

    limpiarPantalla();
    tituloSistema();

    color(COLOR_MENU_PRINCIPAL);
    tituloMenu("ESTADISTICAS PILOTO");
    restaurarColor();

    printf("\nIngrese ID del piloto: ");
    if(!scanf("%u", &idBuscado))
    {
        while(getchar() != '\n');
        printf("ID inválido.\n");
        return ERROR_;
    }
    rewind(pf);
    encontrado = 0;
    fread(&est, sizeof(t_estadistica), 1, pf);

    while(!feof(pf) && !encontrado)
    {
        if(est.id_piloto == idBuscado)
        {
            encontrado = 1;
        }
        else
        {
            fread(&est, sizeof(t_estadistica), 1, pf);
        }
    }
    limpiarPantalla();
    tituloSistema();
    if(!encontrado)
    {
        est.id_piloto = idBuscado;
        est.carreras_corridas = 0;
        est.victorias = 0;
        est.podios = 0;
        est.total_puntos = 0;
        est.mejor_posicion = 0;
        est.peor_posicion = 0;
        est.suma_posiciones = 0;
    }
    color(COLOR_MENU_PRINCIPAL);
    printf("         -------------------------------\n");
    printf("         | ESTADISTICAS PILOTO ID: %u |\n", est.id_piloto);
    printf("         -------------------------------\n");
    restaurarColor();
    printf("         | Carreras corridas : %7u |\n",  est.carreras_corridas);
    printf("         | Victorias         : %7u |\n",  est.victorias);
    printf("         | Podios            : %7u |\n",  est.podios);
    printf("         | Puntos totales    : %7u |\n",  est.total_puntos);
    printf("         | Mejor posicion    : %7u |\n",  est.mejor_posicion);
    printf("         | Peor posicion     : %7u |\n",  est.peor_posicion);

    if(est.carreras_corridas > 0)
    {
        printf("         | Prom. posicion    : %7.2f |\n", (float)est.suma_posiciones / est.carreras_corridas);
        printf("         | Prom. puntos/carr : %7.2f |\n", (float)est.total_puntos / est.carreras_corridas);
    }
    else
    {
        printf("         | Prom. posicion    : %7.2f |\n", 0.0);
        printf("         | Prom. puntos/carr : %7.2f |\n", 0.0);
    }

    color(COLOR_MENU_PRINCIPAL);
    printf("         -------------------------------\n");
    restaurarColor();

    if(!encontrado)
    {
        printf("\n[Nota]: El piloto no registra carreras disputadas en el historial.\n");
    }

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
    limpiarPantalla();
    tituloSistema();
    fread(&est, sizeof(t_estadistica), 1, pf);
    while(!feof(pf))
    {
        if(cant < 5)
        {
            top[cant] = est;
            cant++;
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
    color(COLOR_MENU_PRINCIPAL);
    printf("      -----------------------------------\n");
    printf("      | TOP %d PILOTOS CON MAS VICTORIAS |\n", cant);
    printf("      -----------------------------------\n");
    restaurarColor();

    for(i = 0; i < cant; i++)
    {
        printf("      | %d°   ID: %u   | Victorias: %u   |\n", i+1, top[i].id_piloto, top[i].victorias);
    }

    color(COLOR_MENU_PRINCIPAL);
    printf("      -----------------------------------\n");
    restaurarColor();

    return TODOOK;
}

int mejorPromedioPosicion(FILE* pf)
{
    t_estadistica est;
    t_estadistica mejor;
    float promMejor;
    float promActual;
    int primero;

    limpiarPantalla();
    tituloSistema();
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

    color(COLOR_MENU_PRINCIPAL);
    tituloMenu("MEJOR PROMEDIO DE POSICIÓN");
    restaurarColor();

    printf("ID: %u \nPromedio: %.2f\n", mejor.id_piloto, promMejor);

    return TODOOK;
}
