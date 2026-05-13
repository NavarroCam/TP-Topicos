#include "archivos.h"

/* PILOTOS: txt -> dat
   Formato linea: id;nombre;nacionalidad;id_escuderia;puntos;estado;fechaNacimiento*/
int cargarPilotosTxtABin(const char *txtPath, const char *binPath, const char *errorPath)
{
    char cad[256];
    char *ptr_fin;
    t_piloto p1;
    t_piloto *piloto = &p1;
    FILE *ftxt;
    FILE *fbin;
    FILE *ferror;
    int leidos;

    ftxt = fopen(txtPath, "rt");
    if (ftxt == NULL)
    {
        printf("Error al abrir %s\n", txtPath);
        return ERROR;
    }

    ferror = fopen(errorPath, "wt");
    if (ferror == NULL)
    {
        printf("Error al abrir %s\n", errorPath);
        fclose(ftxt);
        return ERROR;
    }

    fbin = fopen(binPath, "wb");
    if (fbin == NULL)
    {
        printf("Error al abrir %s\n", binPath);
        fclose(ftxt);
        fclose(ferror);
        return ERROR;
    }

    while (fgets(cad, sizeof(cad), ftxt))
    {
        ptr_fin = strpbrk(cad, "\r\n");
        if (ptr_fin)
            *ptr_fin = '\0';

        leidos = sscanf(cad, "%u;%29[^;];%29[^;];%u;%u;%c;%I64u",
                        &piloto->id,
                        piloto->nombre,
                        piloto->nacionalidad,
                        &piloto->id_escuderia,
                        &piloto->puntos_acumulados,
                        &piloto->estado,
                        &piloto->fechaNacimiento);

        if (leidos == 7)
        {
            fwrite(piloto, sizeof(t_piloto), 1, fbin);
        }
        else
        {
            fprintf(ferror, "%s|Error en formato de linea\n", cad);
        }
    }

    fclose(ftxt);
    fclose(fbin);
    fclose(ferror);

    return OK;
}

/* ESCUDERIAS: txt -> dat
   Formato linea: id;codigo;nombre;pais;estado*/
int cargarEscuderiasTxtABin(const char *txtPath, const char *binPath, const char *errorPath)
{
    char cad[256];
    char *ptr_fin;
    t_escuderia e1;
    t_escuderia *escuderia = &e1;
    FILE *ftxt;
    FILE *fbin;
    FILE *ferror;
    int leidos;

    ftxt = fopen(txtPath, "rt");
    if (ftxt == NULL)
    {
        printf("Error al abrir %s\n", txtPath);
        return ERROR;
    }

    ferror = fopen(errorPath, "wt");
    if (ferror == NULL)
    {
        printf("Error al abrir %s\n", errorPath);
        fclose(ftxt);
        return ERROR;
    }

    fbin = fopen(binPath, "wb");
    if (fbin == NULL)
    {
        printf("Error al abrir %s\n", binPath);
        fclose(ftxt);
        fclose(ferror);
        return ERROR;
    }

    while (fgets(cad, sizeof(cad), ftxt))
    {
        ptr_fin = strpbrk(cad, "\r\n");
        if (ptr_fin)
        {
            *ptr_fin = '\0';
        }

        leidos = sscanf(cad, "%u;%3[^;];%29[^;];%49[^;];%d",
        &escuderia->id,
        escuderia->codigo,
        escuderia->nombre,
        escuderia->pais,
        &escuderia->estado);

        if (leidos == 5)
        {
            fwrite(escuderia, sizeof(t_escuderia), 1, fbin);
        }
        else
        {
            fprintf(ferror, "%s|Error en formato de linea\n", cad);
        }
    }

    fclose(ftxt);
    fclose(fbin);
    fclose(ferror);

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
