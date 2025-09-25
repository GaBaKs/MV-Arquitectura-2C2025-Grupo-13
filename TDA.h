#pragma once

#define MAX 10
#define REGISTROS 32
#define MEMORIA 16384
#define CANTCELDAS 4
typedef struct TipoMKV{
    int reg[REGISTROS];           // Cantidad de registros
    unsigned char mem[MEMORIA];            // Cantidad de memoria
    unsigned short tabla_seg[4];
    int flag;
}TipoMKV;


