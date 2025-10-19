#pragma once

#define MAX 10
#define REGISTROS 32
#define MEMORIA 16384
#define CANTCELDAS 4
#define SEGMENTOS 6
#define ENTRADAS
typedef struct TipoMKV{
    int reg[REGISTROS];           // Cantidad de registros
    unsigned char *mem;            // Cantidad de memoria
    unsigned short tabla_seg[4]; // tabla_seg[SEGMENTOS][ENTRADAS] 6 filas para los segmentos y 2 columnas para donde empieza y cuanto mide habria que cambiar bastante
    int flag;
    int tamanoRAM;
}TipoMKV;


