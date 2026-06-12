#include "archivos.h"

/* PILOTOS: txt -> dat
   Formato linea: id;nombre;nacionalidad;id_escuderia;puntos;estado;fechaNacimiento*/
int cargarPilotosTxtABin(const char *txtPath, const char *binPath)
{
    char cad[TAMCADENA];
    t_piloto p1;
    FILE *ftxt;
    FILE *fbin;

    ftxt = fopen(txtPath, "rt");
    if (ftxt == NULL)
    {
        printf("Error al abrir %s\n", txtPath);
        return ERROR_ARCH;
    }

    fbin = fopen(binPath, "wb");
    if (fbin == NULL)
    {
        printf("Error al abrir %s\n", binPath);
        fclose(ftxt);
        return ERROR_ARCH;
    }

    while (fgets(cad, TAMCADENA, ftxt))
    {
        trozarPilotos(cad,&p1);
        fwrite(&p1, sizeof(t_piloto), 1, fbin);
    }

    fclose(ftxt);
    fclose(fbin);

    return TODOOK;
}

int trozarPilotos(char* cad, t_piloto* p1)
{
    char* p;
    p = strchr(cad,'\n');
    if(p)
        *p = '\0';

    p = strrchr(cad,SEPARADOR);
    if(!p)
        return ERROR_;
    sscanf(p+1,"%I64u",&p1->fechaNacimiento);
    *p = '\0';
    p = strrchr(cad,SEPARADOR);
    if(!p)
        return ERROR_;
    sscanf(p+1,"%c",&p1->estado);
    *p = '\0';
    p = strrchr(cad,SEPARADOR);
    if(!p)
        return ERROR_;
    sscanf(p+1,"%u",&p1->puntos_acumulados);
    *p = '\0';
    p = strrchr(cad,SEPARADOR);
    if(!p)
        return ERROR_;
    sscanf(p+1,"%u",&p1->id_escuderia);
    *p = '\0';
    p = strrchr(cad,SEPARADOR);
    if(!p)
        return ERROR_;
    strcpy(p1->nacionalidad,p+1);
    *p = '\0';
    p = strrchr(cad,SEPARADOR);
    if(!p)
        return ERROR_;
    strcpy(p1->nombre,p+1);
    *p = '\0';

    sscanf(cad,"%u",&p1->id);
    return TODOOK;
}

/* ESCUDERIAS: txt -> dat
   Formato linea: id;codigo;nombre;pais;estado*/
int cargarEscuderiasTxtABin(const char *txtPath, const char *binPath)
{
    char cad[TAMCADENA];
    t_escuderia e1;
    FILE *ftxt;
    FILE *fbin;

    ftxt = fopen(txtPath, "rt");
    if (ftxt == NULL)
    {
        printf("Error al abrir %s\n", txtPath);
        return ERROR_ARCH;
    }

    fbin = fopen(binPath, "wb");
    if (fbin == NULL)
    {
        printf("Error al abrir %s\n", binPath);
        fclose(ftxt);
        return ERROR_ARCH;
    }

    while (fgets(cad, sizeof(cad), ftxt))
    {
        trozarEscuderias(cad,&e1);

        fwrite(&e1, sizeof(t_escuderia), 1, fbin);
    }

    fclose(ftxt);
    fclose(fbin);

    return TODOOK;
}

int trozarEscuderias(char* cad, t_escuderia* e1)
{
    char* p;
    p = strchr(cad,'\n');
    if(p)
        *p = '\0';

    p = strrchr(cad,SEPARADOR);
    if(!p)
        return ERROR_;
    sscanf(p+1,"%d",&e1->estado);
    *p = '\0';

    p = strrchr(cad,SEPARADOR);
    if(!p)
        return ERROR_;
    strcpy(e1->pais,p+1);
    *p = '\0';

    p = strrchr(cad,SEPARADOR);
    if(!p)
        return ERROR_;
    strcpy(e1->nombre,p+1);
    *p = '\0';

    p = strrchr(cad,SEPARADOR);
    if(!p)
        return ERROR_;
    strcpy(e1->codigo,p+1);
    *p = '\0';
    sscanf(cad,"%u",&e1->id);

    return TODOOK;
}

/* ---------------------------------------------------------------
   EXPORTAR PILOTOS: dat -> txt
   --------------------------------------------------------------- */
int exportarPilotosTxt(const char *binPath, const char *txtPath)
{
    t_piloto piloto;
    FILE *fbin;
    FILE *ftxt;

    fbin = fopen(binPath, "rb");
    if (fbin == NULL)
    {
        printf("Error al abrir %s\n", binPath);
        return ERROR_ARCH;
    }

    ftxt = fopen(txtPath, "wt");
    if (ftxt == NULL)
    {
        printf("Error al abrir %s\n", txtPath);
        fclose(fbin);
        return ERROR_ARCH;
    }
    fread(&piloto, sizeof(t_piloto), 1, fbin);
    while ( !feof(fbin))
    {
        fprintf(ftxt,"%u|%s|%s|%u|%u|%c|%I64u\n",piloto.id,piloto.nombre,piloto.nacionalidad,piloto.id_escuderia,piloto.puntos_acumulados,
                piloto.estado,piloto.fechaNacimiento);
        fread(&piloto, sizeof(t_piloto), 1, fbin);
    }

    fclose(fbin);
    fclose(ftxt);

    return TODOOK;
}

int exportarEscuderiasTxt(const char *binPath, const char *txtPath)
{
    t_escuderia esc;
    FILE *fbin;
    FILE *ftxt;

    fbin = fopen(binPath, "rb");
    if (fbin == NULL)
    {
        printf("Error al abrir %s\n", binPath);
        return ERROR_ARCH;
    }

    ftxt = fopen(txtPath, "wt");
    if (ftxt == NULL)
    {
        printf("Error al abrir %s\n", txtPath);
        fclose(fbin);
        return ERROR_ARCH;
    }
    fread(&esc, sizeof(t_escuderia), 1, fbin);
    while ( !feof(fbin))
    {
        fprintf(ftxt,"%u|%s|%s|%s|%d\n",esc.id,esc.codigo,esc.nombre,esc.pais,esc.estado);
        fread(&esc, sizeof(t_escuderia), 1, fbin);
    }

    fclose(fbin);
    fclose(ftxt);

    return TODOOK;
}

int generarArchivoOrdenado(const char* nomArch,size_t tamRegistro, int cmp(const void*, const void*))
{
    FILE* pf;
    size_t cantRegistros;
    long tamArchivo;
    void* vec;
    pf = fopen(nomArch,"r+b");
    if(!pf)
    {
        return ERROR_ARCH;
    }

    fseek(pf,0,SEEK_END);
    tamArchivo = ftell(pf);
    cantRegistros = tamArchivo / tamRegistro;
    vec = malloc(cantRegistros * tamRegistro);
    if(!vec)
    {
        fclose(pf);
        return ERROR_MEMORIA;
    }
    rewind(pf);
    fread(vec,tamRegistro,cantRegistros,pf);
    sSort(vec,cantRegistros,tamRegistro,cmp);
    rewind(pf);
    fwrite(vec,tamRegistro,cantRegistros,pf);

    free(vec);
    fclose(pf);
    return TODOOK;
}
