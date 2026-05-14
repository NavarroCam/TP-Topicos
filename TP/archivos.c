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
        return ERROR;
    }

    fbin = fopen(binPath, "wb");
    if (fbin == NULL)
    {
        printf("Error al abrir %s\n", binPath);
        fclose(ftxt);
        return ERROR;
    }

    while (fgets(cad, TAMCADENA, ftxt))
    {
        trozarPilotos(cad,&p1);
        fwrite(&p1, sizeof(t_piloto), 1, fbin);
    }

    fclose(ftxt);
    fclose(fbin);

    return OK;
}

int trozarPilotos(char* cad, t_piloto* p1)
{
    char* p;
    p = strchr(cad,'\n');
    if(!p)
        return ERROR;
    *p = '\0';
    p = strrchr(cad,SEPARADOR);
    if(!p)
        return ERROR;
    sscanf(p+1,"%I64u",&p1->fechaNacimiento);
    *p = '\0';
    p = strrchr(cad,SEPARADOR);
    if(!p)
        return ERROR;
    sscanf(p+1,"%c",&p1->estado);
    *p = '\0';
    p = strrchr(cad,SEPARADOR);
    if(!p)
        return ERROR;
    sscanf(p+1,"%u",&p1->puntos_acumulados);
    *p = '\0';
    p = strrchr(cad,SEPARADOR);
    if(!p)
        return ERROR;
    sscanf(p+1,"%u",&p1->id_escuderia);
    *p = '\0';
    p = strrchr(cad,SEPARADOR);
    if(!p)
        return ERROR;
    strcpy(p1->nacionalidad,p+1);
    *p = '\0';
    p = strrchr(cad,SEPARADOR);
    if(!p)
        return ERROR;
    strcpy(p1->nombre,p+1);
    *p = '\0';

    sscanf(cad,"%u",&p1->id);
    return OK;
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
        return ERROR;
    }

    fbin = fopen(binPath, "wb");
    if (fbin == NULL)
    {
        printf("Error al abrir %s\n", binPath);
        fclose(ftxt);
        return ERROR;
    }

    while (fgets(cad, sizeof(cad), ftxt))
    {
        trozarEscuderias(cad,&e1);
        fwrite(&e1, sizeof(t_escuderia), 1, fbin);
    }

    fclose(ftxt);
    fclose(fbin);

    return OK;
}

int trozarEscuderias(char* cad, t_escuderia* e1)
{
    char* p;
    p = strchr(cad,'\n');
    if(!p)
        return ERROR;
    *p = '\0';
    p = strrchr(cad,SEPARADOR);
    if(!p)
        return ERROR;
    sscanf(p+1,"%d",&e1->estado);
    *p = '\0';
    p = strrchr(cad,SEPARADOR);
    if(!p)
        return ERROR;
    strcpy(e1->pais,p+1);
    *p = '\0';
    p = strrchr(cad,SEPARADOR);
    if(!p)
        return ERROR;
    strcpy(e1->nombre,p+1);
    *p = '\0';
    p = strrchr(cad,SEPARADOR);
    if(!p)
        return ERROR;
    strcpy(e1->codigo,p+1);
    *p = '\0';

    sscanf(cad,"%u",&e1->id);
    return OK;
}

/* ---------------------------------------------------------------
   EXPORTAR PILOTOS: dat -> txt
   --------------------------------------------------------------- */
int exportarPilotosTxt(const char *binPath, const char *txtPath)
{
    t_piloto p1;
    t_piloto *piloto = &p1;
    FILE *fbin;
    FILE *ftxt;

    fbin = fopen(binPath, "rb");
    if (fbin == NULL)
    {
        printf("Error al abrir %s\n", binPath);
        return ERROR;
    }

    ftxt = fopen(txtPath, "wt");
    if (ftxt == NULL)
    {
        printf("Error al abrir %s\n", txtPath);
        fclose(fbin);
        return ERROR;
    }

    while (fread(piloto, sizeof(t_piloto), 1, fbin) == 1)
    {
        fprintf(ftxt,
                "%u;%s;%s;%u;%u;%c;%I64u\n",
                piloto->id,
                piloto->nombre,
                piloto->nacionalidad,
                piloto->id_escuderia,
                piloto->puntos_acumulados,
                piloto->estado,
                piloto->fechaNacimiento);
    }

    fclose(fbin);
    fclose(ftxt);

    return OK;
}
