#pragma once

#define MAX 10
#define REGISTROS 32
#define MEMORIA 16384
#define CANTCELDAS 4
#define SEGMENTOS 6
typedef struct {
    int reg[REGISTROS];           // Cantidad de registros
    unsigned char *mem;           // Cantidad de memoria
    short tabla_seg[8][2]; // tabla_seg[SEGMENTOS][ENTRADAS] 6 filas para los segmentos y 2 columnas para donde empieza y cuanto mide 
    int flag;
    int tamanoRAM;
    int breakpoint;
}TipoMKV;