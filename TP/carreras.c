#include "carreras.h"

void menuCarreras(FILE* archCarreras, FILE* archPilotos,FILE* archPuntajes, FILE* archEstadisticas)
{
    int op;

    do{
        puts("\n\n===============================================");
        printf("\t\t   CARRERAS\n");
        puts("===============================================");
        printf("1. Registrar carrera\n");
        printf("2. Dar de baja carrera\n");
        printf("3. Actualizar puntos (recalcular)\n");
        printf("4. Simular carrera [EXTRA C]\n");
        printf("5. Combinar temporadas [EXTRA E]\n");
        printf("6. Exportar carreras\n");
        printf("0. Volver\n");
        printf("\nOpción: ");
        scanf("%d", &op);
        while (getchar() != '\n');
        switch (op)
        {
            case 1: registrarCarrera(archCarreras, archPilotos, archPuntajes, archEstadisticas);
                break;
            case 2: bajaCarrera(archCarreras, archPilotos);
                break;
//            case 4: simularCarrera(CARRERAS_DAT, PILOTOS_DAT, PILOTOS_IDX); break;
//            case 5: combinarTemporadas(CARRERAS_DAT, TEMPORADA_2, COMBINADA); break;
            case 0:
                break;
            default:
                printf("Opción inválida.\n");
        }
    }while(op != 0);
}

int registrarCarrera(FILE* archCarreras, FILE* archPilotos,FILE* archPuntajes, FILE* archEstadisticas)
{
    t_carrera c;
    tda_vector puntos;
    tda_vector pilotos;

    crear_Vector(&puntos,sizeof(t_puntajes));
    crear_Vector(&pilotos,sizeof(t_piloto));
    cargarVectorPuntos(&puntos,archPuntajes);

    memset(&c,0,sizeof(t_carrera));//setea la memoria de c en 0
    //ID
    c.id = generarIdCarrera(archCarreras);
    //DATOS CARRERA
    cargarDatosCarrera(&c);
    //MOSTRAR PILOTOS
    listarPilotosCarrera(archPilotos);
    //CARGAR RESULTADOS
    cargarResultadosCarrera(&c,&puntos);
    //MUESTRO COMO QUEDO(TEST)
    mostrarResultadosCarrera(&c);
    //ACTUALIZAR PILOTOS
    cargarPilotosAMemoria(&pilotos,archPilotos);
    actualizarPuntosPilotos(&pilotos,&c,"SUMA");
    guardarPilotos(archPilotos,&pilotos);
    //ACTUALIZAR ESTADISTICAS
    actualizarEstadisticas(archEstadisticas, &c);
    //GUARDAR CARRERA
    guardarCarrera(archCarreras,&c);

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
    do {
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
    } while (ValidarFecha(fechaStr) != TODOOK);
    sscanf(fechaStr, "%I64u", &c->fecha);
    //ESTADO
    c->estado = 1;

    do {
        printf("Cuantos pilotos finalizaron la carrera?: ");
        scanf("%d", &c->cant_resultados);
    } while (c->cant_resultados < 1 || c->cant_resultados > MAX_RESULTADOS);
}
//MOSTRAR PILOTOS CON ID
void listarPilotosCarrera(FILE* archPilotos)
{
    t_piloto p;
    fseek(archPilotos, 0, SEEK_SET);

    printf("========== PILOTOS ===========\n");
    fread(&p,sizeof(t_piloto),1,archPilotos);
    while(!feof(archPilotos))
    {
        mostrarPilotoCarrera(&p);
        fread(&p,sizeof(t_piloto),1,archPilotos);
    }
}
//CARGAR RESULTADOS
int cargarResultadosCarrera(t_carrera* c, const tda_vector* puntos)
{
    t_puntajes* puntaje;
    t_posicion* puesto;
    int i;

    printf("Colocar Id del piloto para registrar su posición.\n");
    for(i=0; i<c->cant_resultados; i++)
    {
        puesto = c->resultados + i;
        printf("%u° ", i + 1);
        puesto->posicion = i + 1;

        do{
            scanf("%u", &puesto->id_piloto);
            if(pilotoYaIngresado(c,puesto->id_piloto,i))
                printf("Ese piloto ya fue asignado a otra posición. Ingrese otro ID: ");
        }while(pilotoYaIngresado(c,puesto->id_piloto,i));


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
    puts("========== RESULTADOS ==========");
    puts("| PUESTO | PILOTO | PUNTOS |");
    for(i=0; i< c->cant_resultados; i++)
    {
        aux = (t_posicion*)c->resultados + i;
        printf("%4u° %8u %8u\n", aux->posicion, aux->id_piloto, aux->puntos);
    }
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


void mostrarResultados(void* elem)
{
    t_posicion aux = *(t_posicion*)elem;
    printf("%-8u° %-8u %-8u\n", aux.posicion, aux.id_piloto, aux.puntos);
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

void mostrarPuntos(void* elem)
{
    t_puntajes aux = *(t_puntajes*)elem;
    printf("POS: %u     PUNTOS: %u\n", aux.posicion, aux.puntos);
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

int bajaCarrera(FILE* archCarreras, FILE* archPilotos)
{
    int idbuscado;
    t_carrera car;
    FILE* fbajas;
    int encontrado=0;
    tda_vector pilotos;

    printf("\n==== BAJA DE CARRERA ====\n");
    printf("ID de la carrera a dar de baja: ");
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
                printf("Carrera %s dada de baja.\n", car.circuito);
            }
        }
    }
    if(!encontrado)
        printf("Carrera no encontrada.\n");

    return encontrado ? TODOOK : ERROR_;
}

void MostrarCarrera(FILE* archCarreras)
{
    t_carrera carreras;
    rewind(archCarreras);
    printf("ID\tNOMBRE\t\tESTADO\n");
    while(fread(&carreras, sizeof(t_carrera),1,archCarreras) == 1)
    {
        printf("%4d%20s%4d\n",carreras.id, carreras.circuito, carreras.estado);
    }
}
