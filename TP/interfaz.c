#include "interfaz.h"

static HANDLE consola;

void color(int c)
{
    consola = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(consola, c);
}

void restaurarColor(void)
{
    color(COLOR_TEXTO);        // Gris claro
}

void limpiarPantalla(void)
{
    system("cls");
}

void tituloSistema(void)
{
    color(COLOR_TITULO);   // Cian brillante

    printf("====================================================\n");
    printf("              SISTEMA DE GESTION F1\n");
    printf("====================================================\n");

    restaurarColor();
}

void tituloMenu(const char* titulo)
{
    color(COLOR_MENU_PRINCIPAL);   // Amarillo

    printf("\n                 %s\n\n", titulo);

    restaurarColor();
}
