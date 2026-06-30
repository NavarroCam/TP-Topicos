#include "carreras.h"

void menuCarreras(FILE* archCarreras, FILE* archPilotos,FILE* archPuntajes, FILE* archEstadisticas)
{
    int op;

    do
    {
        limpiarPantalla();
        tituloSistema();
        tituloMenu("GESTIÓN DE CARRERAS");
        color(COLOR_TEXTO);

        printf("   [1] Registrar carrera (manual)\n");
        printf("   [2] Simular carrera\n");
        printf("   [3] Baja de carrera\n");
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
            registrarCarreraManual(archCarreras, archPilotos, archPuntajes, archEstadisticas);
            system("pause");
            break;
        case 2:
            registrarCarreraSimulada(archCarreras, archPilotos, archPuntajes, archEstadisticas);
            system("pause");
            break;
        case 3:
            bajaCarrera(archCarreras, archPilotos);
            system("pause");
            break;
        case 0:
            break;
        default:
            printf("Opción inválida.\n");
        }
    }
    while(op != 0);
}

void guardarCarreraCompleta(FILE* archCarreras, FILE* archPilotos, FILE* archEstadisticas, tda_vector* pilotos, t_carrera* carrera)
{
    actualizarPuntosPilotos(pilotos,carrera,"SUMA");
    guardarPilotos(archPilotos,pilotos);
    actualizarEstadisticas(archEstadisticas,carrera);
    guardarCarrera(archCarreras,carrera);
}

int registrarCarreraManual(FILE* archCarreras, FILE* archPilotos,FILE* archPuntajes, FILE* archEstadisticas)
{
    t_carrera c;
    tda_vector puntos;
    tda_vector pilotos;

    crear_Vector(&puntos,sizeof(t_puntajes));
    crear_Vector(&pilotos,sizeof(t_piloto));

    cargarVectorPuntos(&puntos,archPuntajes);

    cargarPilotosAMemoria(&pilotos,archPilotos);

    memset(&c,0,sizeof(t_carrera));

    c.id = generarIdCarrera(archCarreras);

    cargarDatosCarrera(&c);

    listarPilotosCarrera(archPilotos);

    cargarResultadosCarrera(&c,&puntos);

    mostrarResultadosCarrera(&c);

    guardarCarreraCompleta(archCarreras, archPilotos, archEstadisticas, &pilotos, &c);

    destruir_Vector(&puntos);
    destruir_Vector(&pilotos);
    return TODOOK;
}

int registrarCarreraSimulada(FILE* archCarreras, FILE* archPilotos, FILE* archPuntajes, FILE* archEstadisticas)
{
    t_carrera c;
    tda_vector puntos;
    tda_vector pilotos;

    int op;

    crear_Vector(&puntos,sizeof(t_puntajes));
    crear_Vector(&pilotos,sizeof(t_piloto));

    cargarVectorPuntos(&puntos,archPuntajes);
    cargarPilotosAMemoria(&pilotos,archPilotos);

    memset(&c,0,sizeof(t_carrera));

    c.id = generarIdCarrera(archCarreras);

    cargarDatosCarrera(&c);

    simularResultadoCarrera(&c, &pilotos, &puntos);

    mostrarResultadosCarrera(&c);

    printf("\n¿Desea guardar esta simulación?\n");
    printf("[1] Sí\n");
    printf("[2] No\n");
    printf("\nOpción: ");
    scanf("%d",&op);

    if(op == 1)
    {
        guardarCarreraCompleta(archCarreras, archPilotos, archEstadisticas, &pilotos, &c);
        printf("\nCarrera guardada correctamente.\n");
    }

    else
    {
        printf("\nLa simulación fue descartada.\n");
    }

    destruir_Vector(&puntos);
    destruir_Vector(&pilotos);

    return TODOOK;
}

//GENERAR ID
int generarIdCarrera(FILE* archCarreras)
{
    t_carrera auxCarrera;
    fseek(archCarreras,0,SEEK_END);

    if(ftell(archCarreras) > 0)
    {
        fseek(archCarreras,-(long)sizeof(t_carrera),SEEK_END);
        fread(&auxCarrera,sizeof(t_carrera),1,archCarreras);
        return auxCarrera.id + 1;
    }
    else
    {
        return 1;
    }
}
//CARGA BASICA
void cargarDatosCarrera(t_carrera* c)
{
    char fechaStr[20];
    char *p;
    //CIRCUITO
    printf("Circuito: ");
    fflush(stdin);
    fgets(c->circuito, sizeof(c->circuito), stdin);
    if((p = strchr(c->circuito, '\n')) != NULL)
        *p = '\0';
    else while(getchar() != '\n');
    //FECHA
    do
    {
        printf("Fecha (AAAAMMDD): ");
        fgets(fechaStr, sizeof(fechaStr), stdin);
        if((p = strchr(fechaStr, '\n')) != NULL)
        {
            *p = '\0';
        }
        else
        {
            while(getchar() != '\n');
        }
    }
    while (ValidarFecha(fechaStr) != TODOOK);
    sscanf(fechaStr, "%I64u", &c->fecha);
    //ESTADO
    c->estado = 1;

    do
    {
        printf("Cuantos pilotos finalizaron la carrera?: ");
        scanf("%d", &c->cant_resultados);
    }
    while (c->cant_resultados < 1 || c->cant_resultados > MAX_RESULTADOS);
}
//MOSTRAR PILOTOS CON ID
void listarPilotosCarrera(FILE* archPilotos)
{
    t_piloto p;
    fseek(archPilotos, 0, SEEK_SET);
    limpiarPantalla();
    tituloSistema();
    tituloMenu("PILOTOS ACTIVOS");

    fread(&p,sizeof(t_piloto),1,archPilotos);
    while(!feof(archPilotos))
    {
        mostrarPilotoCarrera(&p);
        fread(&p,sizeof(t_piloto),1,archPilotos);
    }
    color(COLOR_MENU_PRINCIPAL);
    printf("  ------------------------------------------------\n");
    restaurarColor();
}
//CARGAR RESULTADOS
int cargarResultadosCarrera(t_carrera* c, const tda_vector* puntos)
{
    t_puntajes* puntaje;
    t_posicion* puesto;
    int i;

    printf("\nColocar Id del piloto para registrar su posición: \n");
    for(i=0; i<c->cant_resultados; i++)
    {
        puesto = c->resultados + i;
        printf("%u° ", i + 1);
        puesto->posicion = i + 1;

        do
        {
            scanf("%u", &puesto->id_piloto);
            if(pilotoYaIngresado(c,puesto->id_piloto,i))
                printf("Ese piloto ya fue asignado a otra posición. Ingrese otro ID: ");
        }
        while(pilotoYaIngresado(c,puesto->id_piloto,i));


        puntaje = obtenerPuntaje(puntos,i);
        if(!puntaje)
            printf("Error obteniendo el puntaje\n");
        puesto->puntos = puntaje->puntos;
    }
    return TODOOK;
}

int pilotoYaIngresado(const t_carrera* c, unsigned idPiloto, int cantCargados)
{
    t_posicion* act = (t_posicion*)c->resultados;
    t_posicion* fin = (t_posicion*)(c->resultados + cantCargados);

    while(act < fin)
    {
        if(act->id_piloto == idPiloto)
            return ENCONTRADO;
        act++;
    }
    return NO_ENCONTRADO;
}
void mostrarResultadosCarrera(const t_carrera* c)
{
    int i;
    t_posicion* aux;
    limpiarPantalla();
    tituloSistema();
    tituloMenu("RESULTADOS");
    color(COLOR_MENU_PRINCIPAL);

    printf("  ------------------------------------------------\n");
    printf("  | %13s | %15s | %10s |\n", "PUESTO","PILOTO", "PUNTOS");
    printf("  ------------------------------------------------\n");
    restaurarColor();
    for(i=0; i< c->cant_resultados; i++)
    {
        aux = (t_posicion*)c->resultados + i;
        printf("  | %12u° | %15u | %10u |\n", aux->posicion, aux->id_piloto, aux->puntos);
    }
    color(COLOR_MENU_PRINCIPAL);
    printf("  ------------------------------------------------\n");
    restaurarColor();
}
int cargarPilotosAMemoria(tda_vector* pilotos, FILE* archPilotos)
{
    t_piloto p;
    rewind(archPilotos);

    fread(&p,sizeof(t_piloto),1,archPilotos);
    while(!feof(archPilotos))
    {
        insertarAlFinal_Vector(pilotos,&p);
        fread(&p,sizeof(t_piloto),1,archPilotos);
    }
    return TODOOK;
}

void actualizarPuntosPilotos(tda_vector* pil, const t_carrera* c, const char* tipo)
{
    t_piloto* piloto;
    t_posicion* resultado;
    int i;

    for(i=0; i< c->cant_resultados; i++)
    {
        resultado = (t_posicion*)c->resultados + i;

        piloto = bSearch(&(resultado->id_piloto),pil->vec,pil->ce,pil->tam,compararIdPiloto);
        if(piloto)
        {
            if(strcmp(tipo,"SUMA")==0)
                piloto->puntos_acumulados += resultado->puntos;
            if(strcmp(tipo,"RESTA")==0)
                piloto->puntos_acumulados -= resultado->puntos;
        }
    }
}

void guardarPilotos(FILE* archPilotos, const tda_vector* pilotos)
{
    rewind(archPilotos);
    fwrite(pilotos->vec,pilotos->tam, pilotos->ce,archPilotos);
    fflush(archPilotos);
}

int guardarCarrera(FILE* archCarreras, const t_carrera* c)
{
    fseek(archCarreras,0,SEEK_END);
    if(fwrite(c,sizeof(t_carrera),1,archCarreras) != 1)
        return ERROR_;
    return TODOOK;
}

void* obtenerPuntaje(const tda_vector* v, unsigned pos)
{
    if(pos >= v->ce)
        return NULL;
    return (char*)v->vec + pos * v->tam;
}

int cargarVectorPuntos(tda_vector* v,FILE* pf)
{
    char cad[TAMCADENA];
    t_puntajes p;

    rewind(pf);

    while(fgets(cad,TAMCADENA,pf))
    {
        trozarPuntajes(cad,&p);
        insertarAlFinal_Vector(v,&p);
    }

    return TODOOK;
}

int trozarPuntajes(const char* cad,t_puntajes* puntos)
{
    char* p;
    p = strrchr(cad,'\n');
    if(!p)
        return ERROR_;
    *p = '\0';

    p = strrchr(cad,'|');
    if(!p)
        return ERROR_;
    sscanf(p + 1,"%u",&puntos->puntos);
    *p = '\0';
    sscanf(cad,"%u",&puntos->posicion);
    return TODOOK;
}

int listarCarrerasActivas(FILE* archCarreras)
{
    t_carrera carrera;
    t_fecha fecha;
    int cantidad = 0;

    rewind(archCarreras);
    limpiarPantalla();
    tituloSistema();
    tituloMenu("CARRERAS ACTIVAS");
    color(COLOR_MENU_PRINCIPAL);
    printf("  ------------------------------------------------\n");
    printf("  | %-6s | %-20s | %-12s |\n", "ID", "CIRCUITO", "FECHA");
    printf("  ------------------------------------------------\n");
    restaurarColor();
    while(fread(&carrera,sizeof(t_carrera),1,archCarreras)==1)
    {
        if(carrera.estado == 1)
        {
            cantidad++;
            Separar_a_tfecha(carrera.fecha,&fecha);
            printf("  | %-6d | %-20s |  %02u/%02u/%04u  |\n", carrera.id, carrera.circuito, fecha.dia, fecha.mes, fecha.anio);
        }
    }
    color(COLOR_MENU_PRINCIPAL);
    printf("  ------------------------------------------------\n");
    restaurarColor();
    return cantidad;
}

int bajaCarrera(FILE* archCarreras, FILE* archPilotos)
{
    int idbuscado;
    t_carrera car;
    FILE* fbajas;
    int encontrado=0;
    tda_vector pilotos;

    if(listarCarrerasActivas(archCarreras)==0)
    {
        printf("\nNo hay carreras activas para dar de baja.\n");
        return ERROR_;
    }

    printf("\nID de la carrera a dar de baja: ");
    scanf("%d",&idbuscado);

    rewind(archCarreras);
    while(!encontrado && fread(&car,sizeof(t_carrera),1,archCarreras)==1)
    {
        if(car.id==idbuscado)
        {
            encontrado=1;
            if(car.estado == 0)
            {
                printf("La carrera ya fue dado de baja.\n");
                return ERROR_;
            }
            else
            {
                car.estado=0;
                fseek(archCarreras, -(long)sizeof(t_carrera),SEEK_CUR);
                fwrite(&car,sizeof(t_carrera),1,archCarreras);

                crear_Vector(&pilotos,sizeof(t_piloto));
                cargarPilotosAMemoria(&pilotos,archPilotos);
                actualizarPuntosPilotos(&pilotos,&car,"RESTA");
                guardarPilotos(archPilotos,&pilotos);
                destruir_Vector(&pilotos);

                fbajas = fopen(BAJAS_CAR_DAT, "ab");
                if(!fbajas)
                {
                    printf("Error al abrir el archivo");
                    return ERROR_ARCH;
                }
                else
                {
                    fwrite(&car,sizeof(t_carrera),1,fbajas);
                    fclose(fbajas);
                }
                printf("\nLa carrera '%s' (ID %d) fue dada de baja correctamente.\n", car.circuito, car.id);
            }
        }
    }
    if(!encontrado)
        printf("Carrera no encontrada.\n");

    return encontrado ? TODOOK : ERROR_;
}

int simularResultadoCarrera(t_carrera* c, const tda_vector* pilotos, const tda_vector* puntos)
{
    int i,cantactivos=0,indicealeatorio;
    unsigned auxId,*pIds;
    t_piloto* pil;
    t_posicion* puesto;
    t_puntajes* puntaje;

    for(i = 0; i < pilotos->ce; i++)
    {
        pil = (t_piloto*)((char*)pilotos->vec+i*pilotos->tam);
        if(pil->estado=='A')
            cantactivos++;
    }

    if(cantactivos==0)
        return ERROR_;
    pIds=(unsigned*)malloc(cantactivos*sizeof(unsigned));

    if(!pIds)
        return ERROR_MEMORIA;

    cantactivos=0;

    for(i=0; i<pilotos->ce; i++)
    {
        pil=(t_piloto*)((char*)pilotos->vec+i*pilotos->tam);
        if(pil->estado=='A')
        {
            *(pIds+cantactivos) = pil->id;
            cantactivos++;
        }
    }
    if(c->cant_resultados>cantactivos)
        c->cant_resultados = cantactivos;

    srand(time(NULL));

    for(i = cantactivos-1; i > 0; i--)
    {
        indicealeatorio=rand()%(i+1);
        auxId=*(pIds+i);
        *(pIds+i)=*(pIds+indicealeatorio);
        *(pIds+indicealeatorio)= auxId;
    }
    for(i=0; i<c->cant_resultados; i++)
    {
        puesto = (t_posicion*)c->resultados+i;
        puesto->posicion = i+1;
        puesto->id_piloto =*(pIds+i);
        puntaje = obtenerPuntaje(puntos,i);

        if(puntaje)
            puesto->puntos=puntaje->puntos;
        else
            puesto->puntos=0;
    }
    free(pIds);
    printf("Carrera simulada con %d pilotos.\n\n",c->cant_resultados);
    return TODOOK;
}
