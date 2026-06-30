#ifndef INTERFAZ_H_INCLUDED
#define INTERFAZ_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define COLOR_TITULO    11
#define COLOR_MENU_PRINCIPAL      14
#define COLOR_TEXTO     15
#define COLOR_EXITO     10
#define COLOR_ERROR     12
#define COLOR_NORMAL     7

void color(int c);
void color(int c);
void limpiarPantalla(void);
void tituloSistema(void);
void tituloMenu(const char* titulo);
void restaurarColor(void);
#endif // INTERFAZ_H_INCLUDED
